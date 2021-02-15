
/**
 * File:   ble_desc.c
 * Author: AWTK Develop Team
 * Brief:  ble desc
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

ble_desc_t* ble_desc_create(const char* id, ble_char_t* achar) {
  ble_desc_t* desc = NULL;
  return_value_if_fail(id != NULL && achar != NULL, NULL);
  desc = TKMEM_ZALLOC(ble_desc_t);
  return_value_if_fail(desc != NULL, NULL);
  desc->id = tk_strdup(id);
  desc->achar = achar;

  return desc;
}

ret_t ble_desc_destroy(ble_desc_t* desc) {
  return_value_if_fail(desc != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(desc->id);
  memset(desc, 0x00, sizeof(ble_desc_t));
  TKMEM_FREE(desc);

  return RET_OK;
}
