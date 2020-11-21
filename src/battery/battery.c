/**
 * File:   battery.c
 * Author: AWTK Develop Team
 * Brief:  battery
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

#include "tkc/utils.h"
#include "battery/battery.h"
#include "common/platform_request.h"

static ret_t battery_on_result(void* ctx, const char* result_data) {
  log_debug("battery_on_result: %s\n", result_data);
  return RET_OK;
}

ret_t battery_register(platform_on_event_t on_event, void* on_event_ctx) {
  str_t str;
  ret_t ret = 0;
  char buff[32];
  return_value_if_fail(on_event != NULL && on_event_ctx != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_init(&str, 100) != NULL, RET_OOM);

  str_append_char(&str, '{');
  tk_snprintf(buff, sizeof(buff), "%p:%p", on_event, on_event_ctx);
  str_append_json_str_pair(&str, "onevent", buff);
  str_append_char(&str, '}');

  ret = platform_request_send("battery", "register", str.str, battery_on_result, "register");

  str_reset(&str);

  return ret;
}

ret_t battery_unregister(void) {
  return platform_request_send("battery", "unregister", "{}", battery_on_result, "unregister");
}

ret_t battery_get_info(platform_request_on_result_t on_result, void* on_result_ctx) {
  ret_t ret = 0;
  return_value_if_fail(on_result, RET_BAD_PARAMS);

  ret = platform_request_send("battery", "get_info", "{}", on_result, on_result_ctx);

  return ret;
}
