/**
 * File:   share.c
 * Author: AWTK Develop Team
 * Brief:  share
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

#include "share/share.h"
#include "common/platform_request.h"

static ret_t share_on_result(void* ctx, const char* result_data) {
  log_debug("share_on_result: %s\n", result_data);
  return RET_OK;
}

ret_t share_request(const char* text, const char* subject) {
  str_t str;
  ret_t ret = 0;
  return_value_if_fail(text != NULL && subject != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_init(&str, strlen(text) + strlen(subject) + 50) != NULL, RET_OOM);

  str_append_char(&str, '{');
  str_append_json_str_pair(&str, "text", text);
  str_append_char(&str, ',');
  str_append_json_str_pair(&str, "subject", subject);
  str_append_char(&str, '}');

  ret = platform_request_send("share", "share", str.str, share_on_result, NULL);

  str_reset(&str);

  return ret;
}
