/**
 * File:   lpr.h
 * Author: AWTK Develop Team
 * Brief:  lpr api
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
 * 2020-09-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LPR_H
#define TK_LPR_H

#include "tkc/types_def.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class lpr_t
 * @annotation ["fake"]
 * 车牌识别。
 */

/**
 * @method lpr_scan
 * 请求扫描车牌。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {const char*} title 标题。
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lpr_scan(const char* title, platform_request_on_result_t on_result, void* on_result_ctx);

END_C_DECLS

#endif /*TK_LPR_H*/
