
/**
 * File:   ble_device_manager.h
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

#ifndef TK_BLE_DEVICE_MANAGER_H
#define TK_BLE_DEVICE_MANAGER_H

#include "conf_io/conf_json.h"
#include "tkc/data_reader_mem.h"

#include "ble_device.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class ble_device_manager_t
 * BLE设备管理器。
 * 
 */
typedef struct _ble_device_manager_t {
  /**
   * @property {ble_t*} ble
   * @annotation ["readable"]
   * ble对象。
   */
  ble_t* ble;
  /**
   * @property {darray_t} devices
   * @annotation ["readable"]
   * ble设备集合。
   */
  darray_t devices;
} ble_device_manager_t;

/**
 * @method ble_device_manager_create
 * 创建device manager对象。
 * @annotation ["constructor"]
 * @param {ble_t*} ble ble对象。
 * 
 * @return {ble_device_manager_t*} 返回device manager对象。
 */
ble_device_manager_t* ble_device_manager_create(ble_t* ble);

/**
 * @method ble_device_manager_clear
 * 
 * 清除全部设备列表。
 * 
 * @param {ble_device_manager_t*} adevice_manager device manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_device_manager_clear(ble_device_manager_t* adevice_manager);

/**
 * @method ble_device_manager_add_device
 * 
 * 增加设备。
 * 
 * @param {ble_device_manager_t*} adevice_manager device manager对象。
 * @param {ble_device_t*} device device对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_device_manager_add_device(ble_device_manager_t* adevice_manager, ble_device_t* device);

/**
 * @method ble_device_manager_add_device_json
 * 
 * 通过json增加设备。
 * 
 * @param {ble_device_manager_t*} adevice_manager device manager对象。
 * @param {object_t*} json json对象。
 *
 * @return {ble_device_t*} 返回 device对象。
 */
ble_device_t* ble_device_manager_add_device_json(ble_device_manager_t* adevice_manager,
                                                 object_t* json);

/**
 * @method ble_device_manager_get_device
 * 
 * 获取设备。
 * 
 * @param {ble_device_manager_t*} adevice_manager device manager对象。
 * @param {uint32_t} index 设备的索引。
 *
 * @return {ble_device_t*} 返回 device对象。
 */
ble_device_t* ble_device_manager_get_device(ble_device_manager_t* adevice_manager, uint32_t index);

/**
 * @method ble_device_manager_get_device_by_id
 * 
 * 获取设备。
 * 
 * @param {ble_device_manager_t*} adevice_manager device manager对象。
 * @param {const char*} id 设备ID。
 *
 * @return {ble_device_t*} 返回 device对象。
 */
ble_device_t* ble_device_manager_get_device_by_id(ble_device_manager_t* adevice_manager,
                                                  const char* id);

/**
 * @method ble_device_manager_destroy
 * 
 * 销毁device manager对象。
 * 
 * @param {ble_device_manager_t*} adevice_manager device manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_device_manager_destroy(ble_device_manager_t* adevice_manager);

ret_t ble_device_manager_test(void);

END_C_DECLS

#endif /*TK_BLE_DEVICE_MANAGER_H*/
