/**
 * File:   url_view.h
 * Author: AWTK Develop Team
 * Brief:  url_view api
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
 * 2021-03-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_URL_VIEW_H
#define TK_URL_VIEW_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class url_view_t
 * @annotation ["fake"]
 * 指定的URL。
 */

/**
 * @method url_view_request
 * 请求打开指定的URL。
 *
 * @annotation ["static"]
 * @param {const char*} url url。
 * @param {const char*} mime_type mime_type。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t url_view_request(const char* url, const char* mime_type);

END_C_DECLS

#endif /*TK_URL_VIEW_H*/
