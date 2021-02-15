
/**
 * File:   ble_char.h
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

#ifndef TK_BLE_CHAR_H
#define TK_BLE_CHAR_H

#include "ble_types.h"
#include "common/platform_request.h"

BEGIN_C_DECLS
/**
 * @class ble_char_t
 * BLE设备的特性。
 * 
 */
struct _ble_char_t {
  /**
   * @property {char*} id
   * @annotation ["readable"]
   * UUID
   */
  char* id;
  /**
   * @property {ble_service_t*} service
   * @annotation ["readable"]
   * 服务对象。
   */
  ble_service_t* service;
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
   * @property {darray_t} descs
   * @annotation ["readable"]
   * descriptor 列表。
   */
  darray_t descs;
};

/**
 * @method ble_char_create
 * 创建 characteristic 对象。
 * @annotation ["constructor"]
 * 
 * @param {const char*} id UUID。
 * @param {ble_service_t*} service service 对象。
 * 
 * @return {ble_char_t*} 返回 characteristic 对象。
 */
ble_char_t* ble_char_create(const char* id, ble_service_t* service);

/**
 * @method ble_char_add_desc
 * 
 * 增加 descriptor 对象。
 * 
 * @param {ble_char_t*} achar characteristic 对象
 * @param {ble_desc_t*} desc descriptor 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_char_add_desc(ble_char_t* achar, ble_desc_t* desc);

/**
 * @method ble_char_get_desc
 * 
 * 获取 descriptor 对象。
 * 
 * @param {ble_char_t*} achar characteristic 对象。
 * @param {uint32_t} index descriptor 索引。
 *
 * @return {ble_desc_t*} 返回 descriptor 对象。
 */
ble_desc_t* ble_char_get_desc(ble_char_t* achar, uint32_t index);

/**
 * @method ble_char_get_desc_by_id
 * 
 * 获取 descriptor 对象。
 * 
 * @param {ble_char_t*} achar characteristic 对象。
 * @param {const char*} id  descriptor ID。
 *
 * @return {ble_desc_t*} 返回 descriptor 对象。
 */
ble_desc_t* ble_char_get_desc_by_id(ble_char_t* achar, const char* id);

/**
 * @method ble_char_destroy
 * 
 * 销毁 characteristic 对象。
 * 
 * @param {ble_char_t*} achar characteristic 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_char_destroy(ble_char_t* achar);

END_C_DECLS

#endif /*TK_BLE_CHAR_H*/
