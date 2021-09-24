/**
 * File:   misc.h
 * Author: AWTK Develop Team
 * Brief:  misc api
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
 * 2021-09-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MISC_H
#define TK_MISC_H

#include "tkc/types_def.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class misc_t
 * @annotation ["fake"]
 * 杂七杂八的原生函数。
 */

/**
 * @method misc_call_func
 * 用指定的参数调用指定的函数。
 *
 * @annotation ["static"]
 * @param {const char*} name 函数名。
 * @param {const char*} args 参数。
 * @param {platform_request_on_result_t} on_result 函数执行完成时的回调函数(可为NULL)。
 * @param {void*} on_result_ctx 回调函数的上下文(可为NULL)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t misc_call_func(const char* name, const char* args, platform_request_on_result_t on_result, void* on_result_ctx);

/**
 * https://blog.csdn.net/timelord_s/article/details/82023395
 */
#define notifySystemToScan(filename) misc_call_func("notifySystemToScan", filename, NULL, NULL);

END_C_DECLS

#endif /*TK_MISC_H*/
