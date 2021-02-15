
/**
 * File:   ble_desc.h
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

#ifndef TK_BLE_DESC_H
#define TK_BLE_DESC_H

#include "ble_types.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class ble_desc_t
 * BLE设备的特性描述。
 * 
 */
struct _ble_desc_t {
  /**
   * @property {char*} id
   * @annotation ["readable"]
   * UUID
   */
  char* id;
  /**
   * @property {bool_t} readable
   * @annotation ["readable"]
   * 可读。
   */
  bool_t readable;
  /**
   * @property {bool_t} writable
   * @annotation ["readable"]
   * 可写。
   */
  bool_t writable;
  /**
   * @property {bool_t} notifiable
   * @annotation ["readable"]
   * 可通知。
   */
  bool_t notifiable;
  /**
   * @property {ble_char_t*} service
   * @annotation ["readable"]
   * characteristic 对象。
   */
  ble_char_t* achar;
};

/**
 * @method ble_desc_create
 * 创建 descriptor 对象。
 * @annotation ["constructor"]
 * 
 * @param {const char*} id UUID。
 * @param {ble_char_t*} achar characteristic 对象。
 * 
 * @return {ble_desc_t*} 返回 descriptor 对象。
 */
ble_desc_t* ble_desc_create(const char* id, ble_char_t* achar);

/**
 * @method ble_desc_destroy
 * 
 * 销毁 descriptor 对象。
 * 
 * @param {ble_desc_t*} desc descriptor 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_desc_destroy(ble_desc_t* desc);

END_C_DECLS

#endif /*TK_BLE_DESC_H*/
