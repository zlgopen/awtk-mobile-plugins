
/**
 * File:   ble_char.c
 * Author: AWTK Develop Team
 * Brief:  ble achar
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

#include "ble_desc.h"
#include "ble_char.h"

BEGIN_C_DECLS

static int ble_desc_compare_by_id(const void* a, const void* b) {
  const ble_desc_t* desc = (const ble_desc_t*)a;
  return strcmp(desc->id, (const char*)b);
}

ble_char_t* ble_char_create(const char* id, ble_service_t* service) {
  ble_char_t* achar = NULL;
  return_value_if_fail(id != NULL && service != NULL, NULL);
  achar = TKMEM_ZALLOC(ble_char_t);
  return_value_if_fail(achar != NULL, NULL);

  achar->id = tk_strdup(id);
  achar->service = service;
  darray_init(&(achar->descs), 5, (tk_destroy_t)ble_desc_destroy,
              (tk_compare_t)ble_desc_compare_by_id);

  return achar;
}

ret_t ble_char_add_desc(ble_char_t* achar, ble_desc_t* desc) {
  return_value_if_fail(achar != NULL && desc != NULL, RET_BAD_PARAMS);

  return darray_push(&(achar->descs), desc);
}

ble_desc_t* ble_char_get_desc(ble_char_t* achar, uint32_t index) {
  return_value_if_fail(achar != NULL && index < achar->descs.size, NULL);

  return (ble_desc_t*)(achar->descs.elms[index]);
}

ble_desc_t* ble_char_get_desc_by_id(ble_char_t* achar, const char* id) {
  ble_desc_t* desc = NULL;
  return_value_if_fail(achar != NULL && id != NULL, NULL);

  return (ble_desc_t*)darray_find(&(achar->descs), (void*)id);
}

ret_t ble_char_destroy(ble_char_t* achar) {
  return_value_if_fail(achar != NULL, RET_BAD_PARAMS);
  darray_deinit(&(achar->descs));
  TKMEM_FREE(achar);

  return RET_OK;
}
