/**
 * File:   foobar.h
 * Author: AWTK Develop Team
 * Brief:  foobar api
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
 * 2020-11-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FOOBAR_H
#define TK_FOOBAR_H

#include "tkc/types_def.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class foobar_t
 * @annotation ["fake"]
 * 演示调用foobar server中的服务。
 */

/**
 * @method foobar_add
 * 调用add函数。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {int} a a。
 * @param {int} b b。
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foobar_add(int a, int b, platform_request_on_result_t on_result, void* on_result_ctx);

END_C_DECLS

#endif /*TK_FOOBAR_H*/
