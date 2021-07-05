/**
 * File:   awake.c
 * Author: AWTK Develop Team
 * Brief:  awake
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
 * 2021-07-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "awake/awake.h"
#include "common/platform_request.h"

static ret_t awake_on_result(void* ctx, const char* result_data) {
  log_debug("awake_on_result:%s\n", result_data);

  return RET_OK;
}

ret_t awake_keep_screen_on(bool_t value) {
  ret_t ret = 0;

  if(value) {
    ret = platform_request_send("awake", "keep_screen_on", "{}", awake_on_result, NULL);
  } else {
    ret = platform_request_send("awake", "not_keep_screen_on", "{}", awake_on_result, NULL);
  }

  return ret;
}

ret_t awake_turn_screen_on(void) {
  return platform_request_send("awake", "turn_screen_on", "{}", awake_on_result, NULL);
}
