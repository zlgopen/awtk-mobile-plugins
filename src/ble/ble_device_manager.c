
/**
 * File:   ble_device_manager.c
 * Author: AWTK Develop Team
 * Brief:  ble adevice_manager
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

#include "ble_device_manager.h"
#include "ble_device.h"

BEGIN_C_DECLS

static int ble_device_compare_by_id(const void* a, const void* b) {
  const ble_device_t* device = (const ble_device_t*)a;

  return strcmp(device->id, (const char*)b);
}

ble_device_manager_t* ble_device_manager_create(ble_t* ble) {
  ble_device_manager_t* adevice_manager = NULL;
  adevice_manager = TKMEM_ZALLOC(ble_device_manager_t);
  return_value_if_fail(adevice_manager != NULL, NULL);

  darray_init(&(adevice_manager->devices), 5, (tk_destroy_t)ble_device_destroy,
              (tk_compare_t)ble_device_compare_by_id);
  adevice_manager->ble = ble;

  return adevice_manager;
}

ret_t ble_device_manager_clear(ble_device_manager_t* adevice_manager) {
  return_value_if_fail(adevice_manager != NULL, RET_BAD_PARAMS);
  return darray_clear(&(adevice_manager->devices));
}

ble_device_t* ble_device_manager_add_device_json(ble_device_manager_t* adevice_manager,
                                                 object_t* json) {
  ble_device_t* device = NULL;
  const char* name = object_get_prop_str(json, "name");
  const char* addr = object_get_prop_str(json, "addr");
  int32_t rssi = object_get_prop_int(json, "rssi", 0);
  bool_t connected = object_get_prop_int(json, "connected", FALSE);
  return_value_if_fail(adevice_manager != NULL && addr != NULL, NULL);

  device = ble_device_manager_get_device_by_id(adevice_manager, addr);
  if (device == NULL) {
    device = ble_device_create(addr, name, rssi, adevice_manager->ble);
  }
  return_value_if_fail(device != NULL, NULL);
  ble_device_set_services(device, json);
  ble_device_manager_add_device(adevice_manager, device);

  return device;
}

ret_t ble_device_manager_add_device(ble_device_manager_t* adevice_manager, ble_device_t* device) {
  return_value_if_fail(adevice_manager != NULL && device != NULL, RET_BAD_PARAMS);
  device->ble = adevice_manager->ble;

  return darray_push(&(adevice_manager->devices), device);
}

ble_device_t* ble_device_manager_get_device(ble_device_manager_t* adevice_manager, uint32_t index) {
  return_value_if_fail(adevice_manager != NULL && index < adevice_manager->devices.size, NULL);

  return (ble_device_t*)(adevice_manager->devices.elms[index]);
}

ble_device_t* ble_device_manager_get_device_by_id(ble_device_manager_t* adevice_manager,
                                                  const char* id) {
  ble_device_manager_t* device = NULL;
  return_value_if_fail(adevice_manager != NULL && id != NULL, NULL);

  return (ble_device_t*)darray_find(&(adevice_manager->devices), (void*)id);
}

ret_t ble_device_manager_destroy(ble_device_manager_t* adevice_manager) {
  return_value_if_fail(adevice_manager != NULL, RET_BAD_PARAMS);
  darray_deinit(&(adevice_manager->devices));
  TKMEM_FREE(adevice_manager);

  return RET_OK;
}

ret_t ble_device_manager_test(void) {
  char url[MAX_PATH];
  ble_device_t* device = NULL;
  const char* str =
      "{\"type\":\"scan_result\",\"name\":\"foobar\",\"rssi\":-82,\"bluetooth_type\":2,\"addr\":"
      "\"50:E5:0C:90:50:8B\"}";
  object_t* json = conf_json_load(data_reader_mem_build_url(str, strlen(str), url), FALSE);

  ble_device_manager_t* dm = ble_device_manager_create(NULL);
  ble_device_manager_add_device_json(dm, json);
  device = ble_device_manager_get_device(dm, 0);
  assert(device != NULL);
  assert(strcmp(device->id, "50:E5:0C:90:50:8B") == 0);
  assert(strcmp(device->name, "foobar") == 0);
  assert(device->rssi == -82);

  device = ble_device_manager_get_device_by_id(dm, "50:E5:0C:90:50:8B");
  assert(device != NULL);
  assert(strcmp(device->id, "50:E5:0C:90:50:8B") == 0);
  assert(strcmp(device->name, "foobar") == 0);
  assert(device->rssi == -82);
  ble_device_manager_clear(dm);

  device = ble_device_manager_get_device_by_id(dm, "50:E5:0C:90:50:8B");
  assert(device == NULL);

  ble_device_manager_destroy(dm);

  return RET_OK;
}
