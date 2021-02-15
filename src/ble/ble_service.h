
/**
 * File:   ble_service.h
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

#ifndef TK_BLE_SERVICE_H
#define TK_BLE_SERVICE_H

#include "ble_types.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class ble_service_t
 * BLE设备的服务。
 * 
 */
struct _ble_service_t {
  /**
   * @property {char*} id
   * @annotation ["readable"]
   * UUID
   */
  char* id;

  /**
   * @property {ble_device_t*} device
   * @annotation ["readable"]
   * 设备对象
   */
  ble_device_t* device;

  /**
   * @property {darray_t} chars
   * @annotation ["readable"]
   * characteristic 特性列表。
   */
  darray_t chars;
};

/**
 * @method ble_service_create
 * 创建 service 对象。
 * @annotation ["constructor"]
 * @param {const char*} id UUID。
 * @param {ble_device_t*} device device对象。
 * 
 * @return {ble_service_t*} 返回 service 对象。
 */
ble_service_t* ble_service_create(const char* id, ble_device_t* device);

/**
 * @method ble_service_add_char
 * 
 * 增加 characteristic 对象。
 * 
 * @param {ble_service_t*} aservice service 对象。
 * @param {ble_char_t*} achar characteristic 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_service_add_char(ble_service_t* aservice, ble_char_t* achar);

/**
 * @method ble_service_get_char
 * 
 * 获取 characteristic 对象。
 * 
 * @param {ble_service_t*} aservice service 对象。
 * @param {uint32_t} index characteristic 索引。
 *
 * @return {ble_char_t*} 返回 characteristic 对象。
 */
ble_char_t* ble_service_get_char(ble_service_t* aservice, uint32_t index);

/**
 * @method ble_service_get_char_by_id
 * 
 * 获取 characteristic 对象。
 * 
 * @param {ble_service_t*} aservice service 对象。
 * @param {const char*} id  characteristic ID。
 *
 * @return {ble_char_t*} 返回 characteristic 对象。
 */
ble_char_t* ble_service_get_char_by_id(ble_service_t* aservice, const char* id);

/**
 * @method ble_service_destroy
 * 
 * 销毁 service 对象。
 * 
 * @param {ble_service_t*} aservice service 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_service_destroy(ble_service_t* aservice);

END_C_DECLS

#endif /*TK_BLE_SERVICE_H*/
