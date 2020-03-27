/**
 * File:   wifi.c
 * Author: AWTK Develop Team
 * Brief:  wifi
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
 * 2020-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "wifi/wifi.h"
#include "common/platform_request.h"

ret_t wifi_scan(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("wifi", "scan", "{}", on_result, on_result_ctx);
}

ret_t wifi_connect(const char* ssid, platform_request_on_result_t on_result, void *on_result_ctx) {
  str_t str;
  ret_t ret = 0;
  return_value_if_fail(ssid != NULL && on_result != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_init(&str, strlen(ssid) + 20) != NULL, RET_OOM);

  str_append_char(&str, '{');
  str_append_json_str_pair(&str, "ssid", ssid);
  str_append_char(&str, '}');

  ret = platform_request_send("wifi", "connect", str.str, on_result, on_result_ctx);

  str_reset(&str);

  return ret;
}

