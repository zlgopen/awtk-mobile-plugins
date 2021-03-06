/**
 * File:   url_view.c
 * Author: AWTK Develop Team
 * Brief:  url_view
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

#include "url_view/url_view.h"
#include "common/platform_request.h"

static ret_t url_view_on_result(void* ctx, const char* result_data) {
  log_debug("url_view_on_result: %s\n", result_data);
  return RET_OK;
}

ret_t url_view_request(const char* url, const char* mime_type) {
  str_t str;
  ret_t ret = 0;
  return_value_if_fail(url != NULL && mime_type != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_init(&str, strlen(url) + strlen(mime_type) + 50) != NULL, RET_OOM);

  str_append_char(&str, '{');
  str_append_json_str_pair(&str, "url", url);
  str_append_char(&str, ',');
  str_append_json_str_pair(&str, "mime_type", mime_type);
  str_append_char(&str, '}');

  ret = platform_request_send("url_view", "view", str.str, url_view_on_result, NULL);

  str_reset(&str);

  return ret;
}
