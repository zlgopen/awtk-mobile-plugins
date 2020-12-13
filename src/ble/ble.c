/**
 * File:   ble.c
 * Author: AWTK Develop Team
 * Brief:  ble
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

#include "tkc/utils.h"
#include "ble/ble.h"
#include "common/platform_request.h"

static ret_t ble_on_result(void* ctx, const char* result_data) {
  log_debug("ble_on_result: %s\n", result_data);
  return RET_OK;
}

ret_t ble_register(platform_on_event_t on_event, void* on_event_ctx) {
  str_t str;
  ret_t ret = 0;
  char buff[32];
  return_value_if_fail(on_event != NULL && on_event_ctx != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_init(&str, 100) != NULL, RET_OOM);

  str_append_char(&str, '{');
  tk_snprintf(buff, sizeof(buff), "%p:%p", on_event, on_event_ctx);
  str_append_json_str_pair(&str, "onevent", buff);
  str_append_char(&str, '}');

  ret = platform_request_send("ble", "register", str.str, ble_on_result, "register");

  str_reset(&str);

  return ret;
}

ret_t ble_unregister(void) {
  return platform_request_send("ble", "unregister", "{}", ble_on_result, "unregister");
}

ret_t ble_start(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("ble", "start", "{}", on_result, on_result_ctx);
}

ret_t ble_start_scan(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("ble", "start_scan", "{}", on_result, on_result_ctx);
}

ret_t ble_stop_scan(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("ble", "stop_scan", "{}", on_result, on_result_ctx);
}

ret_t ble_get_info(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("ble", "get_info", "{}", on_result, on_result_ctx);
}

ret_t ble_connect(const char* address, platform_request_on_result_t on_result, void* on_result_ctx) {
  char args[128];
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  tk_snprintf(args, sizeof(args)-1, "{\"address\":\"%s\"}", address);
  return platform_request_send("ble", "connect", args, on_result, on_result_ctx);
}

ret_t ble_disconnect(const char* address, platform_request_on_result_t on_result, void* on_result_ctx) {
  char args[128];
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  tk_snprintf(args, sizeof(args)-1, "{\"address\":\"%s\"}", address);
  return platform_request_send("ble", "disconnect", args, on_result, on_result_ctx);
}
