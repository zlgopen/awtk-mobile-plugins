/**
 * File:   misc.c
 * Author: AWTK Develop Team
 * Brief:  misc
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

#include "misc/misc.h"
#include "common/platform_request.h"

static ret_t misc_on_result(void* ctx, const char* result_data) {
  log_debug("misc_on_result: %s\n", result_data);
  return RET_OK;
}

ret_t misc_call_func(const char* name, const char* args, 
  platform_request_on_result_t on_result, void* on_result_ctx) {
  str_t str;
  ret_t ret = 0;
  return_value_if_fail(name != NULL && args != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_init(&str, strlen(name) + strlen(args) + 50) != NULL, RET_OOM);

  str_append_char(&str, '{');
  str_append_json_str_pair(&str, "name", name);
  str_append_char(&str, ',');
  str_append_json_str_pair(&str, "args", args);
  str_append_char(&str, '}');

  if(on_result == NULL) {
    on_result = misc_on_result;
  }

  ret = platform_request_send("misc", "call", str.str, misc_on_result, NULL);

  str_reset(&str);

  return ret;
}
