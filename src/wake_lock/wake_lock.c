/**
 * File:   wake_lock.c
 * Author: AWTK Develop Team
 * Brief:  wake_lock
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
 * 2021-07-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "wake_lock/wake_lock.h"
#include "common/platform_request.h"

static ret_t wake_lock_on_result(void* ctx, const char* result_data) {
  log_debug("wake_lock_on_result:%s\n", result_data);

  return RET_OK;
}

ret_t wake_lock_require(void) {
  return platform_request_send("wake_lock", "require", "{}", wake_lock_on_result, NULL);
}

ret_t wake_lock_release(void) {
  return platform_request_send("wake_lock", "release", "{}", wake_lock_on_result, NULL);
}
