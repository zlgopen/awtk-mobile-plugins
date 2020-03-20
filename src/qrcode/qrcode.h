/**
 * File:   qrcode.h
 * Author: AWTK Develop Team
 * Brief:  qrcode api
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
 * 2020-03-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_QRCODE_H
#define TK_QRCODE_H

#include "tkc/types_def.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class qrcode_t
 * @annotation ["fake"]
 * 二维码识别。
 */

/**
 * @method qrcode_scan
 * 请求扫描二维码。
 *
 * @annotation ["static"]
 * @param {const char*} title 标题。
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t qrcode_scan(const char* title, platform_request_on_result_t on_result, void* on_result_ctx);

END_C_DECLS

#endif /*TK_QRCODE_H*/
