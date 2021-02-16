/**
 * File:   ble.c
 * Author: AWTK Develop Team
 * Brief:  ble
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ===============================================================
 * 2020-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "ble/ble.h"
#include "ble_device_manager.h"
#include "common/platform_request.h"

static ret_t ble_low_on_result(void* ctx, const char* result_data) {
  log_debug("ble_low_on_result: %s\n", result_data);

  return RET_OK;
}

ret_t ble_low_register(platform_on_event_t on_event, void* on_event_ctx) {
  str_t str;
  ret_t ret = 0;
  char buff[32];
  return_value_if_fail(on_event != NULL && on_event_ctx != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_init(&str, 100) != NULL, RET_OOM);

  str_append_char(&str, '{');
  tk_snprintf(buff, sizeof(buff), "%p:%p", on_event, on_event_ctx);
  str_append_json_str_pair(&str, "onevent", buff);
  str_append_char(&str, '}');

  ret = platform_request_send("ble", "register", str.str, ble_low_on_result, "register");

  str_reset(&str);

  return ret;
}

ret_t ble_low_unregister(void) {
  return platform_request_send("ble", "unregister", "{}", ble_low_on_result, "unregister");
}

ret_t ble_low_start(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("ble", "start", "{}", on_result, on_result_ctx);
}

ret_t ble_low_start_scan(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("ble", "start_scan", "{}", on_result, on_result_ctx);
}

ret_t ble_low_get_connected_devices(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("ble", "get_connected_devices", "{}", on_result, on_result_ctx);
}

ret_t ble_low_stop_scan(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("ble", "stop_scan", "{}", on_result, on_result_ctx);
}

ret_t ble_low_connect(const char* address, uint32_t mtu, platform_request_on_result_t on_result,
                      void* on_result_ctx) {
  char args[128];
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  tk_snprintf(args, sizeof(args) - 1, "{\"address\":\"%s\",\"mtu\":%u}", address, mtu);
  return platform_request_send("ble", "connect", args, on_result, on_result_ctx);
}

ret_t ble_low_write_characteristic(const char* address, const char* uuid, const char* data,
                                   bool_t is_hex, platform_request_on_result_t on_result,
                                   void* on_result_ctx) {
  char args[512];
  const char* format = is_hex ? "hex" : "string";
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  tk_snprintf(args, sizeof(args) - 1,
              "{\"address\":\"%s\", \"type\":\"%s\", \"uuid\":\"%s\", \"data\":\"%s\"}", address,
              format, uuid, data);

  return platform_request_send("ble", "write_char", args, on_result, on_result_ctx);
}

ret_t ble_low_write_descriptor(const char* address, const char* uuid, const char* data,
                               bool_t is_hex, platform_request_on_result_t on_result,
                               void* on_result_ctx) {
  char args[512];
  const char* format = is_hex ? "hex" : "string";
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  tk_snprintf(args, sizeof(args) - 1,
              "{\"address\":\"%s\", \"type\":\"%s\", \"uuid\":\"%s\", \"data\":\"%s\"}", address,
              format, uuid, data);

  return platform_request_send("ble", "write_desc", args, on_result, on_result_ctx);
}

ret_t ble_low_read_characteristic(const char* address, const char* uuid,
                                  platform_request_on_result_t on_result, void* on_result_ctx) {
  char args[512];
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  tk_snprintf(args, sizeof(args) - 1, "{\"address\":\"%s\", \"uuid\":\"%s\"}", address, uuid);

  return platform_request_send("ble", "read_char", args, on_result, on_result_ctx);
}

ret_t ble_low_read_descriptor(const char* address, const char* uuid,
                              platform_request_on_result_t on_result, void* on_result_ctx) {
  char args[512];
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  tk_snprintf(args, sizeof(args) - 1, "{\"address\":\"%s\", \"uuid\":\"%s\"}", address, uuid);

  return platform_request_send("ble", "read_desc", args, on_result, on_result_ctx);
}

ret_t ble_low_disconnect(const char* address, platform_request_on_result_t on_result,
                         void* on_result_ctx) {
  char args[128];
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  tk_snprintf(args, sizeof(args) - 1, "{\"address\":\"%s\"}", address);
  return platform_request_send("ble", "disconnect", args, on_result, on_result_ctx);
}

static ret_t ble_default_on_notify(void* ctx, const char* data) {
  char url[MAX_PATH];
  ble_t* ble = (ble_t*)ctx;
  object_t* json = conf_json_load(data_reader_mem_build_url(data, strlen(data), url), FALSE);
  const char* type = object_get_prop_str(json, "type");
  if (tk_str_eq(type, "onScanResult")) {
    ble_device_t* device = ble_device_manager_add_device_json(ble->devices, json);
    return_value_if_fail(device != NULL, RET_BAD_PARAMS);
    if (ble->on_device_found != NULL) {
      ble->on_device_found(ble->on_device_found_ctx, device);
    }
  } else if (tk_str_eq(type, "onServicesDiscovered")) {
    ble_device_t* device = ble_device_manager_add_device_json(ble->devices, json);
    return_value_if_fail(device != NULL, RET_BAD_PARAMS);
    if (ble->on_device_services_discovered != NULL) {
      ble->on_device_services_discovered(ble->on_device_services_discovered_ctx, device);
    }
  } else if (tk_str_eq(type, "onDeviceConnected")) {
    ble_device_t* device = ble_device_manager_add_device_json(ble->devices, json);
    return_value_if_fail(device != NULL, RET_BAD_PARAMS);
    if (ble->on_device_connected != NULL) {
      ble->on_device_connected(ble->on_device_connected_ctx, device);
    }
  } else if (tk_str_eq(type, "onCharacteristicChanged")) {
    const char* address = object_get_prop_str(json, "address");
    const char* uuid = object_get_prop_str(json, "uuid");
    const char* data = object_get_prop_str(json, "hex");
    if (ble->on_characteristic_changed != NULL) {
      ble->on_characteristic_changed(ble->on_characteristic_changed_ctx, address, uuid, data);
    }
  } else if (tk_str_eq(type, "onCharacteristicRead")) {
    const char* address = object_get_prop_str(json, "address");
    const char* uuid = object_get_prop_str(json, "uuid");
    const char* data = object_get_prop_str(json, "hex");
    if (ble->on_characteristic_read != NULL) {
      ble->on_characteristic_read(ble->on_characteristic_read_ctx, address, uuid, data);
    }
  } else if (tk_str_eq(type, "onCharacteristicWrite")) {
    const char* address = object_get_prop_str(json, "address");
    const char* uuid = object_get_prop_str(json, "uuid");
    if (ble->on_characteristic_write != NULL) {
      ble->on_characteristic_write(ble->on_characteristic_write_ctx, address, uuid);
    }
  } else if (tk_str_eq(type, "onDescriptorRead")) {
    const char* address = object_get_prop_str(json, "address");
    const char* uuid = object_get_prop_str(json, "uuid");
    const char* data = object_get_prop_str(json, "hex");
    if (ble->on_descriptor_read != NULL) {
      ble->on_descriptor_read(ble->on_descriptor_read_ctx, address, uuid, data);
    }
  } else if (tk_str_eq(type, "onDescriptorWrite")) {
    const char* address = object_get_prop_str(json, "address");
    const char* uuid = object_get_prop_str(json, "uuid");
    if (ble->on_descriptor_write != NULL) {
      ble->on_descriptor_write(ble->on_descriptor_write_ctx, address, uuid);
    }
  } else {
    log_debug("not supported type %s\n", type);
  }

  OBJECT_UNREF(json);

  return RET_OK;
}

ret_t ble_default_on_result(void* ctx, const char* data) {
  log_debug("result:%s\n", data);

  return RET_OK;
}

ble_t* ble_create(void) {
  ble_t* ble = TKMEM_ZALLOC(ble_t);
  return_value_if_fail(ble != NULL, NULL);
  ble->devices = ble_device_manager_create(ble);
  ble_low_start(ble_default_on_result, ble);
  ble_low_register(ble_default_on_notify, ble);

  return ble;
}

ret_t ble_start_scan(ble_t* ble) {
  ret_t ret = RET_OK;
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  if (!ble->scanning) {
    ble->scanning = TRUE;
    ret = ble_low_start_scan(ble_default_on_result, ble);
  }

  return ret;
}

ret_t ble_stop_scan(ble_t* ble) {
  ret_t ret = RET_OK;
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  if (ble->scanning) {
    ble->scanning = FALSE;
    ret = ble_low_stop_scan(ble_default_on_result, ble);
  }

  return ret;
}

ret_t ble_connect_to(ble_t* ble, const char* address, uint32_t mtu) {
  return_value_if_fail(ble != NULL && address != NULL, RET_BAD_PARAMS);

  return ble_low_connect(address, mtu, ble_default_on_result, ble);
}

ret_t ble_write_characteristic(ble_t* ble, const char* address, const char* uuid, const char* data,
                               bool_t is_hex) {
  return_value_if_fail(ble != NULL && address != NULL && data != NULL, RET_BAD_PARAMS);

  return ble_low_write_characteristic(address, uuid, data, is_hex, ble_default_on_result, ble);
}

ret_t ble_write_descriptor(ble_t* ble, const char* address, const char* uuid, const char* data,
                           bool_t is_hex) {
  return_value_if_fail(ble != NULL && address != NULL && data != NULL, RET_BAD_PARAMS);

  return ble_low_write_descriptor(address, uuid, data, is_hex, ble_default_on_result, ble);
}

ret_t ble_read_characteristic(ble_t* ble, const char* address, const char* uuid) {
  return_value_if_fail(ble != NULL && address != NULL, RET_BAD_PARAMS);

  return ble_low_read_characteristic(address, uuid, ble_default_on_result, ble);
}

ret_t ble_read_descriptor(ble_t* ble, const char* address, const char* uuid) {
  return_value_if_fail(ble != NULL && address != NULL, RET_BAD_PARAMS);

  return ble_low_read_descriptor(address, uuid, ble_default_on_result, ble);
}

ret_t ble_set_on_device_found(ble_t* ble, ble_on_device_found_t on_device_found, void* ctx) {
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  ble->on_device_found = on_device_found;
  ble->on_device_found_ctx = ctx;
  return RET_OK;
}

ret_t ble_set_on_device_connected(ble_t* ble, ble_on_device_connected_t on_device_connected,
                                  void* ctx) {
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  ble->on_device_connected = on_device_connected;
  ble->on_device_connected_ctx = ctx;
  return RET_OK;
}

ret_t ble_set_on_device_services_discovered(
    ble_t* ble, ble_on_device_services_discovered_t on_device_services_discovered, void* ctx) {
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  ble->on_device_services_discovered = on_device_services_discovered;
  ble->on_device_services_discovered_ctx = ctx;
  return RET_OK;
}

ret_t ble_set_on_characteristic_changed(ble_t* ble,
                                        ble_on_characteristic_changed_t on_characteristic_changed,
                                        void* ctx) {
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  ble->on_characteristic_changed = on_characteristic_changed;
  ble->on_characteristic_changed_ctx = ctx;
  return RET_OK;
}

ret_t ble_set_on_characteristic_read(ble_t* ble,
                                     ble_on_characteristic_read_t on_characteristic_read,
                                     void* ctx) {
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  ble->on_characteristic_read = on_characteristic_read;
  ble->on_characteristic_read_ctx = ctx;
  return RET_OK;
}

ret_t ble_set_on_descriptor_read(ble_t* ble, ble_on_descriptor_read_t on_descriptor_read,
                                 void* ctx) {
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  ble->on_descriptor_read = on_descriptor_read;
  ble->on_descriptor_read_ctx = ctx;
  return RET_OK;
}

ret_t ble_set_on_characteristic_write(ble_t* ble,
                                      ble_on_characteristic_write_t on_characteristic_write,
                                      void* ctx) {
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  ble->on_characteristic_write = on_characteristic_write;
  ble->on_characteristic_write_ctx = ctx;
  return RET_OK;
}

ret_t ble_set_on_descriptor_write(ble_t* ble, ble_on_descriptor_write_t on_descriptor_write,
                                  void* ctx) {
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  ble->on_descriptor_write = on_descriptor_write;
  ble->on_descriptor_write_ctx = ctx;

  return RET_OK;
}

ret_t ble_destroy(ble_t* ble) {
  return_value_if_fail(ble != NULL, RET_BAD_PARAMS);
  ble_low_unregister();
  ble_device_manager_destroy(ble->devices);
  memset(ble, 0x00, sizeof(ble_t));
  TKMEM_FREE(ble);

  return RET_OK;
}
