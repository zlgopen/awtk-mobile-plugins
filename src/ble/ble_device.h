
/**
 * File:   ble_device.h
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

#ifndef TK_BLE_DEVICE_H
#define TK_BLE_DEVICE_H

#include "ble_types.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class ble_device_t
 * BLE设备。
 * 
 */
struct _ble_device_t {
  /**
   * @property {char*} id
   * @annotation ["readable"]
   * 设备的ID(即物理地址)
   */
  char* id;
  /**
   * @property {char*} name 
   * @annotation ["readable"]
   * 设备的名称。
   */
  char* name;
  /**
   * @property {int32_t} rssi
   * @annotation ["readable"]
   * 信号强度。
   */
  int32_t rssi;

    /**
   * @property {boo_t} connected
   * @annotation ["readable"]
   * 是否连接成功。
   */
    bool_t connected;

    /**
   * @property {darray_t} services
   * @annotation ["readable"]
   * 设备提供的服务。
   */
  darray_t services;

  /**
   * @property {ble_t*} ble
   * @annotation ["readable"]
   * ble对象。
   */
  ble_t* ble;
};

/**
 * @method ble_device_create
 * 创建device对象。
 * @annotation ["constructor"]
 * @param {const char*} id 设备ID。
 * @param {const char*} name 设备名称。
 * @param {int32_t} rssi 信号强度。
 * @param {ble_t*} ble ble对象。
 * 
 * @return {ble_device_t*} 返回device对象。
 */
ble_device_t* ble_device_create(const char* id, const char* name, int32_t rssi, ble_t* ble);

/**
 * @method ble_device_is_connected
 * 
 * 设备是否连接成功。
 * 
 * @param {ble_device_t*} adevice device对象。
 *
 * @return {bool_t} 返回TRUE表示已经连接，否则表示没连接。
 */
bool_t ble_device_is_connected(ble_device_t* adevice);

/**
 * @method ble_device_set_services
 * 
 * 通过json设置设备的服务。
 * 
 * @param {ble_device_t*} adevice device对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_device_set_services(ble_device_t* adevice, object_t* json);

/**
 * @method ble_device_get_service
 * 
 * 获取服务。
 * 
 * @param {ble_device_t*} adevice device对象。
 * @param {uint32_t} index 服务的索引。
 *
 * @return {ble_service_t*} 返回 service 对象。
 */
ble_service_t* ble_device_get_service(ble_device_t* adevice, uint32_t index);

/**
 * @method ble_device_get_service_by_id
 * 
 * 获取服务。
 * 
 * @param {ble_device_t*} adevice device对象。
 * @param {const char*} id 服务ID。
 *
 * @return {ble_service_t*} 返回 service 对象。
 */
ble_service_t* ble_device_get_service_by_id(ble_device_t* adevice, const char* id);

/**
 * @method ble_device_destroy
 * 
 * 销毁device对象。
 * 
 * @param {ble_device_t*} adevice device对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_device_destroy(ble_device_t* adevice);

ret_t ble_device_test(void);

END_C_DECLS

#endif /*TK_BLE_DEVICE_H*/
