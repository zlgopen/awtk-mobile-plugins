/**
 * File:   platform_request.h
 * Author: AWTK Develop Team
 * Brief:  platform_request api
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
 * 2020-03-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PLATFORM_REQUEST_H
#define TK_PLATFORM_REQUEST_H

#include "tkc/str.h"

BEGIN_C_DECLS

typedef ret_t (*platform_request_on_result_t)(void* ctx, const char* result_data);

/**
 * @class platform_request_t
 * @annotation ["fake"]
 * 请求执行平台相关的功能，比如相机、分享和二维码等等。
 * 
 */

/**
 * @method platform_request_init
 * @annotation ["static"]
 *
 * 初始化。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t platform_request_init(void);

/**
 * @method platform_request_send
 * @annotation ["static"]
 *
 * 发送请求。
 * @param {const char*} target 目标名称。如"camera"代表相机。
 * @param {const char*} action 动作名称。
 * @param {const char*} args 参数。
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t platform_request_send(const char* target, const char* action, const char* args,
    platform_request_on_result_t on_result, void* on_result_ctx);

/**
 * @method platform_request_recv
 * @annotation ["static"]
 *
 * 读取一个请求。
 * @param {str_t*} str 用于返回请求数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t platform_request_recv(str_t* str);

/**
 * @method platform_request_deinit
 * @annotation ["static"]
 *
 * ~初始化。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t platform_request_deinit(void);

END_C_DECLS

#endif /*TK_PLATFORM_REQUEST_H*/
