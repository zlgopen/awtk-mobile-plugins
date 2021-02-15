
/**
 * File:   ble_device.c
 * Author: AWTK Develop Team
 * Brief:  ble adevice
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-02-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ble_char.h"
#include "ble_desc.h"
#include "ble_device.h"
#include "ble_service.h"

BEGIN_C_DECLS

ret_t ble_device_add_service(ble_device_t* adevice, ble_service_t* service);

static int ble_service_compare_by_id(const void* a, const void* b) {
  const ble_device_t* service = (const ble_device_t*)a;

  return strcmp(service->id, (const char*)b);
}

ble_device_t* ble_device_create(const char* id, const char* name, int32_t rssi, ble_t* ble) {
  ble_device_t* adevice = NULL;
  return_value_if_fail(id != NULL, NULL);
  adevice = TKMEM_ZALLOC(ble_device_t);
  return_value_if_fail(adevice != NULL, NULL);

  adevice->ble = ble;
  adevice->rssi = rssi;
  adevice->id = tk_strdup(id);
  adevice->name = tk_strdup(name);
  darray_init(&(adevice->services), 5, (tk_destroy_t)ble_service_destroy,
              (tk_compare_t)ble_service_compare_by_id);

  return adevice;
}

bool_t ble_device_is_connected(ble_device_t* adevice) {
  return_value_if_fail(adevice != NULL, FALSE);

  return adevice->services.size > 0;
}

static ret_t ble_char_init(ble_char_t* achar, uint32_t service_index, uint32_t char_index,
                           object_t* json) {
  uint32_t i = 0;
  uint32_t nr = 0;
  char path[MAX_PATH];
  const char* uuid = NULL;
  ble_desc_t* desc = NULL;
  tk_snprintf(path, sizeof(path), "services.[%u].chars.[%u].descs.#size", service_index,
              char_index);
  nr = object_get_prop_int(json, path, 0);

  for (i = 0; i < nr; i++) {
    tk_snprintf(path, sizeof(path), "services.[%u].chars.[%u].descs.[%u].uuid", service_index,
                char_index, i);
    uuid = object_get_prop_str(json, path);
    desc = ble_desc_create(uuid, achar);
    return_value_if_fail(desc != NULL, RET_BAD_PARAMS);
    ble_char_add_desc(achar, desc);
  }

  return RET_OK;
}

static ret_t ble_service_init(ble_service_t* service, uint32_t index, object_t* json) {
  uint32_t i = 0;
  uint32_t nr = 0;
  char path[MAX_PATH];
  const char* uuid = NULL;
  ble_char_t* achar = NULL;
  tk_snprintf(path, sizeof(path), "services.[%u].chars.#size", index);
  nr = object_get_prop_int(json, path, 0);

  for (i = 0; i < nr; i++) {
    tk_snprintf(path, sizeof(path), "services.[%u].chars.[%u].uuid", index, i);
    uuid = object_get_prop_str(json, path);
    achar = ble_char_create(uuid, service);
    return_value_if_fail(achar != NULL, RET_BAD_PARAMS);
    ble_service_add_char(service, achar);
    ble_char_init(achar, index, i, json);
    tk_snprintf(path, sizeof(path), "services.[%u].chars.[%u].Read", index, i);
    achar->readable = object_get_prop_bool(json, path, FALSE);

    tk_snprintf(path, sizeof(path), "services.[%u].chars.[%u].Write", index, i);
    achar->writable = object_get_prop_bool(json, path, FALSE);

    tk_snprintf(path, sizeof(path), "services.[%u].chars.[%u].WriteWithoutResponse", index, i);
    achar->writable = achar->writable || object_get_prop_bool(json, path, FALSE);

    tk_snprintf(path, sizeof(path), "services.[%u].chars.[%u].Notify", index, i);
    achar->notifiable = object_get_prop_bool(json, path, FALSE);
  }

  return RET_OK;
}

ret_t ble_device_set_services(ble_device_t* adevice, object_t* json) {
  uint32_t i = 0;
  uint32_t nr = 0;
  char path[MAX_PATH];
  const char* uuid = NULL;
  ble_service_t* service = NULL;
  return_value_if_fail(adevice != NULL && json != NULL, RET_BAD_PARAMS);

  nr = object_get_prop_int(json, "services.#size", 0);
  if (nr > 0) {
    darray_clear(&(adevice->services));
    for (i = 0; i < nr; i++) {
      tk_snprintf(path, sizeof(path), "services.[%u].uuid", i);
      uuid = object_get_prop_str(json, path);
      service = ble_service_create(uuid, adevice);
      return_value_if_fail(service != NULL, RET_BAD_PARAMS);

      ble_device_add_service(adevice, service);
      ble_service_init(service, i, json);
    }
  }
  return RET_OK;
}

ret_t ble_device_add_service(ble_device_t* adevice, ble_service_t* service) {
  return_value_if_fail(adevice != NULL && service != NULL, RET_BAD_PARAMS);

  return darray_push(&(adevice->services), service);
}

ble_service_t* ble_device_get_service(ble_device_t* adevice, uint32_t index) {
  return_value_if_fail(adevice != NULL && index < adevice->services.size, NULL);

  return (ble_service_t*)(adevice->services.elms[index]);
}

ble_service_t* ble_device_get_service_by_id(ble_device_t* adevice, const char* id) {
  return_value_if_fail(adevice != NULL && id != NULL, NULL);

  return (ble_service_t*)darray_find(&(adevice->services), (void*)id);
}

ret_t ble_device_destroy(ble_device_t* adevice) {
  return_value_if_fail(adevice != NULL, RET_BAD_PARAMS);
  darray_deinit(&(adevice->services));
  TKMEM_FREE(adevice);

  return RET_OK;
}

ret_t ble_device_test(void) {
  char url[MAX_PATH];
  ble_service_t* service = NULL;
  ble_char_t* achar = NULL;
  ble_desc_t* desc = NULL;

  const char* str =
      "{\"type\":\"scan_result\",\"rssi\":0,\"name\":\"mi_mtk\",\"bluetooth_type\":2,\"addr\":\"5C:"
      "E5:0C:90:50:8B\",\"services\":[{\"uuid\":\"00001800-0000-1000-8000-00805f9b34fb\",\"chars\":"
      "[{\"uuid\":\"00002a00-0000-1000-8000-00805f9b34fb\",\"descs\":[{\"uuid\":\"111\"}, "
      "{\"uuid\":\"222\"}, {\"uuid\":\"333\"}]},{\"uuid\":\"00002a01-0000-"
      "1000-8000-00805f9b34fb\",\"descs\":[]},{\"uuid\":\"00002a04-0000-1000-8000-00805f9b34fb\","
      "\"descs\":[]},{\"uuid\":\"00002aa6-0000-1000-8000-00805f9b34fb\",\"descs\":[]}]},{\"uuid\":"
      "\"00001801-0000-1000-8000-00805f9b34fb\",\"chars\":[]},{\"uuid\":\"0000fe95-0000-1000-8000-"
      "00805f9b34fb\",\"chars\":[{\"uuid\":\"00000001-0000-1000-8000-00805f9b34fb\",\"descs\":[]},{"
      "\"uuid\":\"00000002-0000-1000-8000-00805f9b34fb\",\"descs\":[]},{\"uuid\":\"00000004-0000-"
      "1000-8000-00805f9b34fb\",\"descs\":[]},{\"uuid\":\"00000005-0000-1000-8000-00805f9b34fb\","
      "\"descs\":[]},{\"uuid\":\"00000010-0000-1000-8000-00805f9b34fb\",\"descs\":[]}]}]}";
  object_t* json = conf_json_load(data_reader_mem_build_url(str, strlen(str), url), FALSE);
  ble_device_t* dev = ble_device_create("5C:E5:0C:90:50:8B", "mi_mtk", 0, NULL);
  ble_device_set_services(dev, json);
  assert(dev->services.size == 3);
  service = ble_device_get_service(dev, 0);
  assert(service->chars.size == 4);
  achar = ble_service_get_char(service, 0);
  assert(achar->descs.size == 3);

  desc = ble_char_get_desc(achar, 0);
  assert(strcmp(desc->id, "111") == 0);

  desc = ble_char_get_desc(achar, 1);
  assert(strcmp(desc->id, "222") == 0);

  desc = ble_char_get_desc(achar, 2);
  assert(strcmp(desc->id, "333") == 0);

  ble_device_destroy(dev);
}