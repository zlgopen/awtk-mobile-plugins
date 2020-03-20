/**
 * File:   share.h
 * Author: AWTK Develop Team
 * Brief:  share api
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
 * 2020-03-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SHARE_H
#define TK_SHARE_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class share_t
 * @annotation ["fake"]
 * 分享接口。
 */

/**
 * @method share_request
 * 请求分享数据。
 *
 * @annotation ["static"]
 * @param {const char*} text text。
 * @param {const char*} subject subject。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t share_request(const char* text, const char* subject);

END_C_DECLS

#endif /*TK_SHARE_H*/
