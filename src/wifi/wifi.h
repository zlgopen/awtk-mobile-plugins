/**
 * File:   wifi.h
 * Author: AWTK Develop Team
 * Brief:  wifi api
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

#ifndef TK_WIFI_H
#define TK_WIFI_H

#include "tkc/types_def.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class wifi_t
 * @annotation ["fake"]
 * WIFI无线网络。
 */

/**
 * @method wifi_scan
 * 请求扫描热点。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wifi_scan(platform_request_on_result_t on_result, void *on_result_ctx);

/**
 * @method wifi_get_info
 * 获取当前连接信息。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wifi_get_info(platform_request_on_result_t on_result, void *on_result_ctx);

END_C_DECLS

#endif /*TK_WIFI_H*/
