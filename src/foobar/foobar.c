/**
 * File:   foobar.c
 * Author: AWTK Develop Team
 * Brief:  foobar
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
 * 2020-11-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "foobar/foobar.h"
#include "common/platform_request.h"

ret_t foobar_add(int a, int b, platform_request_on_result_t on_result, void* on_result_ctx) {
  ret_t ret = 0;
  char args[64];
  return_value_if_fail(on_result, RET_BAD_PARAMS);
  tk_snprintf(args, sizeof(args)-1, "{\"a\":%d, \"b\":%d}", a, b);
  ret = platform_request_send("foobar", "add", args, on_result, on_result_ctx);

  return ret;
}
