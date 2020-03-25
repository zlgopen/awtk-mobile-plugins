/**
 * File:   battery.h
 * Author: AWTK Develop Team
 * Brief:  battery api
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
 * 2020-03-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BATTERY_H
#define TK_BATTERY_H

#include "tkc/types_def.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class battery_t
 * @annotation ["fake"]
 * 电池电量信息与状态。
 */

/**
 * @method battery_register
 * 注册电池变化事件。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {platform_on_event_t} on_event 事件处理函数。
 * @param {void*} on_result_ctx 事件处理函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t battery_register(platform_on_event_t on_event, void* on_event_ctx);

/**
 * @method battery_unregister
 * 注销事件变化通知。
 *
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t battery_unregister(void);

/**
 * @method battery_register
 * 注册电池变化事件。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
int battery_get_info(platform_request_on_result_t on_result, void* on_result_ctx);

END_C_DECLS

#endif /*TK_BATTERY_H*/
