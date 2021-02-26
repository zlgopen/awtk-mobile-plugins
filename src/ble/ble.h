/**
 * File:   ble.h
 * Author: AWTK Develop Team
 * Brief:  ble api
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BLE_H
#define TK_BLE_H

#include "tkc/types_def.h"
#include "common/platform_request.h"
#include "ble/ble_device_manager.h"

BEGIN_C_DECLS

typedef ret_t (*ble_on_device_found_t)(void* ctx, ble_device_t* device);

typedef ret_t (*ble_on_device_connected_t)(void* ctx, ble_device_t* device);

typedef ret_t (*ble_on_device_disconnected_t)(void* ctx, ble_device_t* device);

typedef ret_t (*ble_on_device_services_discovered_t)(void* ctx, ble_device_t* device);

typedef ret_t (*ble_on_characteristic_changed_t)(void* ctx, const char* address, const char* uuid,
                                                 const char* data);

typedef ret_t (*ble_on_characteristic_read_t)(void* ctx, const char* address, const char* uuid,
                                              const char* data);

typedef ret_t (*ble_on_descriptor_read_t)(void* ctx, const char* address, const char* uuid,
                                          const char* data);

typedef ret_t (*ble_on_characteristic_write_t)(void* ctx, const char* address, const char* uuid);

typedef ret_t (*ble_on_descriptor_write_t)(void* ctx, const char* address, const char* uuid);

/**
 * @class ble_t
 * BLE无线网络。
 * 
 */
struct _ble_t {
  ble_device_manager_t* devices;

  /*private*/
  bool_t scanning;
  ble_on_device_found_t on_device_found;
  void* on_device_found_ctx;
  ble_on_device_connected_t on_device_connected;
  void* on_device_connected_ctx;
  ble_on_device_disconnected_t on_device_disconnected;
  void* on_device_disconnected_ctx;
  ble_on_device_services_discovered_t on_device_services_discovered;
  void* on_device_services_discovered_ctx;
  ble_on_characteristic_read_t on_characteristic_read;
  void* on_characteristic_read_ctx;
  ble_on_characteristic_changed_t on_characteristic_changed;
  void* on_characteristic_changed_ctx;
  ble_on_descriptor_read_t on_descriptor_read;
  void* on_descriptor_read_ctx;
  ble_on_characteristic_write_t on_characteristic_write;
  void* on_characteristic_write_ctx;
  ble_on_descriptor_write_t on_descriptor_write;
  void* on_descriptor_write_ctx;
};

/**
 * @method ble_create
 * 创建ble对象并开启蓝牙。
 * @annotation ["constructor"]
 *
 * @return {ble_t*} 返回ble对象。
 */
ble_t* ble_create(void);

/**
 * @method ble_start_scan
 * 扫描蓝牙设备。扫描到的蓝牙设备通过on_device_found回调函数返回。
 * 
 * @param {ble_t*} ble ble对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_start_scan(ble_t* ble);

/**
 * @method ble_stop_scan
 * 停止扫描蓝牙设备。
 * @param {ble_t*} ble ble对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_stop_scan(ble_t* ble);

/**
 * @method ble_get_connected_devices
 * 获取已经连接的蓝牙设备。蓝牙设备通过on_device_found回调函数返回。
 * 
 * @param {ble_t*} ble ble对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_get_connected_devices(ble_t* ble);

/**
 * @method ble_disconnect
 * 断开连接。
 *
 * @param {ble_t*} ble ble对象。
 * @param {const char*} address 设备的地址。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_disconnect(ble_t* ble, const char* address);

/**
 * @method ble_connect_to
 * 连接到指定的蓝牙设备。连接成功后会触发on_device_connected回调函数。
 * 但是只有on_device_services_discovered完成后才能收发数据。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {const char*} address 设备的地址。
 * @param {uint32_t} mtu 最大传输单元长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_connect_to(ble_t* ble, const char* address, uint32_t mtu);

/**
 * @method ble_write_characteristic
 * 写数据。
 * @param {ble_t*} ble ble对象。
 * @param {const char*} address 设备的地址。
 * @param {const char*} uuid 特性的UUID。
 * @param {const char*} data 要写的数据。
 * @param {bool_t} is_hex 为FALSE时表示data是普通字符串，否则表示data是将二进制数据编码成hex的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_write_characteristic(ble_t* ble, const char* address, const char* uuid, const char* data,
                               bool_t is_hex);

/**
 * @method ble_write_descriptor
 * 写数据。
 * @param {ble_t*} ble ble对象。
 * @param {const char*} address 设备的地址。
 * @param {const char*} uuid 描述的UUID。
 * @param {const char*} data 要写的数据。
 * @param {bool_t} is_hex 为FALSE时表示data是普通字符串，否则表示data是将二进制数据编码成hex的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_write_descriptor(ble_t* ble, const char* address, const char* uuid, const char* data,
                           bool_t is_hex);

/**
 * @method ble_read_characteristic
 * 发起读数据的请求。读取的数据通过on_characteristic_read回调函数返回。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {const char*} address 设备的地址。
 * @param {const char*} uuid 特性的UUID。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_read_characteristic(ble_t* ble, const char* address, const char* uuid);

/**
 * @method ble_read_descriptor
 * 发起读数据的请求。读取的数据通过on_descriptor_read回调函数返回。
 * @param {ble_t*} ble ble对象。
 * @param {const char*} address 设备的地址。
 * @param {const char*} uuid 描述的UUID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_read_descriptor(ble_t* ble, const char* address, const char* uuid);

/**
 * @method ble_set_on_device_found
 * 设置 on_device_found 回调函数，该函数在扫描到设备时被调用。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {ble_on_device_found_t} on_device_found 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_set_on_device_found(ble_t* ble, ble_on_device_found_t on_device_found, void* ctx);

/**
 * @method ble_set_on_device_connected
 * 设置 on_device_connected 回调函数，该函数在设备连接成功时被调用。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {ble_on_device_connected_t} on_device_connected 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_set_on_device_connected(ble_t* ble, ble_on_device_connected_t on_device_connected,
                                  void* ctx);

/**
 * @method ble_set_on_device_disconnected
 * 设置 on_device_disconnected 回调函数，该函数在设备连接断开时被调用。
 *
 * @param {ble_t*} ble ble对象。
 * @param {ble_on_device_disconnected_t} on_device_disconnected 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_set_on_device_disconnected(ble_t* ble, ble_on_device_disconnected_t on_device_disconnected,
                                     void* ctx);

/**
 * @method ble_set_on_device_services_discovered
 * 设置 on_device_services_discovered 回调函数，该函数在服务发现完成时被调用。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {ble_on_device_services_discovered_t} on_device_services_discovered 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_set_on_device_services_discovered(
    ble_t* ble, ble_on_device_services_discovered_t on_device_services_discovered, void* ctx);

/**
 * @method ble_set_on_characteristic_changed
 * 设置 on_characteristic_changed 回调函数，该函数在 characteristic_changed 时被调用。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {ble_on_characteristic_changed_t} on_characteristic_changed 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_set_on_characteristic_changed(ble_t* ble,
                                        ble_on_characteristic_changed_t on_characteristic_changed,
                                        void* ctx);

/**
 * @method ble_set_on_characteristic_read
 * 设置 on_characteristic_read 回调函数，该函数在 characteristic 数据读取完成时被调用。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {ble_on_characteristic_read_t} on_characteristic_read 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_set_on_characteristic_read(ble_t* ble,
                                     ble_on_characteristic_read_t on_characteristic_read,
                                     void* ctx);

/**
 * @method ble_set_on_descriptor_read
 * 设置 on_descriptor_read 回调函数，该函数在 descriptor 数据读取完成时被调用。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {ble_on_descriptor_read_t} on_descriptor_read 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_set_on_descriptor_read(ble_t* ble, ble_on_descriptor_read_t on_descriptor_read,
                                 void* ctx);

/**
 * @method ble_set_on_characteristic_write
 * 设置 on_characteristic_write 回调函数，该函数在 characteristic 数据写完成时被调用。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {ble_on_characteristic_write_t} on_characteristic_write 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_set_on_characteristic_write(ble_t* ble,
                                      ble_on_characteristic_write_t on_characteristic_write,
                                      void* ctx);

/**
 * @method ble_set_on_descriptor_write
 * 设置 on_descriptor_write 回调函数，该函数在 descriptor 数据写完成时被调用。
 * 
 * @param {ble_t*} ble ble对象。
 * @param {ble_on_descriptor_write_t} on_descriptor_write 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_set_on_descriptor_write(ble_t* ble, ble_on_descriptor_write_t on_descriptor_write,
                                  void* ctx);

/**
 * @method ble_destroy
 * 
 * 销毁ble对象。
 * 
 * @param {ble_t*} ble ble对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_destroy(ble_t* ble);

END_C_DECLS

#endif /*TK_BLE_H*/
