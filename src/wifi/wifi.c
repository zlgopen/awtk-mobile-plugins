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

ret_t wifi_get_info(platform_request_on_result_t on_result, void* on_result_ctx) {
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);

  return platform_request_send("wifi", "get_info", "{}", on_result, on_result_ctx);
}
