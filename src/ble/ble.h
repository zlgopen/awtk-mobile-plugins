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

BEGIN_C_DECLS

/**
 * @class ble_t
 * @annotation ["fake"]
 * BLE无线网络。
 */

/**
 * @method ble_start
 * 开启蓝牙(先确保本函数调用成功，再调用其它函数)。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 *
 * @annotation ["static"]
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_start(platform_request_on_result_t on_result, void *on_result_ctx);

/**
 * @method ble_register
 * 注册ble各种事件和执行结果。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 *
 * @annotation ["static"]
 * @param {platform_on_event_t} on_event 事件处理函数。
 * @param {void*} on_result_ctx 事件处理函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_register(platform_on_event_t on_event, void* on_event_ctx);

/**
 * @method ble_unregister
 * 注销事件变化通知。
 *
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_unregister(void);

/**
 * @method ble_start_scan
 * 请求扫描蓝牙设备。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_start_scan(platform_request_on_result_t on_result, void *on_result_ctx);

/**
 * @method ble_stop_scan
 * 停止扫描蓝牙设备。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_stop_scan(platform_request_on_result_t on_result, void *on_result_ctx);

/**
 * @method ble_get_info
 * 获取当前扫描到的设备。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_get_info(platform_request_on_result_t on_result, void *on_result_ctx);

/**
 * @method ble_connect
 * 连接到指定的设备。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 *
 * @annotation ["static"]
 * @param {const char*} address 设备地址。
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_connect(const char* address, platform_request_on_result_t on_result, void *on_result_ctx);

/**
 * @method ble_disconnect
 *  断开指定的设备。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 *
 * @annotation ["static"]
 * @param {const char*} address 设备地址。
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ble_disconnect(const char* address, platform_request_on_result_t on_result, void *on_result_ctx);

END_C_DECLS

#endif /*TK_BLE_H*/
