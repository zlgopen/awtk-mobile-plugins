
/**
 * File:   ble_service.c
 * Author: AWTK Develop Team
 * Brief:  ble aservice
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

BEGIN_C_DECLS

struct _ble_service_t {
  char* id;
  ble_device_t* device;
  darray_t chars;
};

static int ble_char_compare_by_id(const void* a, const void* b) {
  const ble_char_t* achar = (const ble_char_t*)a;
  return strcmp(achar->id, (const char*)b);
}

ble_service_t* ble_service_create(const char* id, ble_device_t* device) {
  ble_service_t* aservice = NULL;
  return_value_if_fail(id != NULL && device != NULL, NULL);
  aservice = TKMEM_ZALLOC(ble_service_t);
  return_value_if_fail(aservice != NULL, NULL);

  aservice->id = tk_strdup(id);
  aservice->device = device;
  darray_init(&(aservice->chars), 5, (tk_destroy_t)ble_char_destroy,
              (tk_compare_t)ble_char_compare_by_id);

  return aservice;
}

ret_t ble_service_add_char(ble_service_t* aservice, ble_char_t* achar) {
  return_value_if_fail(aservice != NULL && achar != NULL, RET_BAD_PARAMS);

  return darray_push(&(aservice->chars), achar);
}

ble_char_t* ble_service_get_char(ble_service_t* aservice, uint32_t index) {
  return_value_if_fail(aservice != NULL && index < aservice->chars.size, NULL);

  return (ble_char_t*)(aservice->chars.elms[index]);
}

ble_char_t* ble_service_get_char_by_id(ble_service_t* aservice, const char* id) {
  return_value_if_fail(aservice != NULL && id != NULL, NULL);

  return (ble_char_t*)darray_find(&(aservice->chars), (void*)id);
}

ret_t ble_service_destroy(ble_service_t* aservice) {
  return_value_if_fail(aservice != NULL, RET_BAD_PARAMS);
  darray_deinit(&(aservice->chars));
  TKMEM_FREE(aservice);

  return RET_OK;
}
