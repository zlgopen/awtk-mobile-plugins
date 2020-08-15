/**
 * File:   platform_request.h
 * Author: AWTK Develop Team
 * Brief:  platform_request api
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
 * 2020-03-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/idle.h"
#include "tkc/socket_pair.h"
#include "tkc/socket_helper.h"
#include "platform_request.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32")
#endif/*WIN32*/

static int s_client_fd;
static int s_server_fd;

typedef struct _platform_request_result_t {
  char* result;
  void* on_result_ctx;
  platform_request_on_result_t on_result;
} platform_request_result_t;

platform_request_result_t* platform_request_result_create(platform_request_on_result_t on_result,
                                                          void* ctx);
ret_t platform_request_result_set_result(platform_request_result_t* result, const char* data);
ret_t platform_request_result_call(platform_request_result_t* result);
ret_t platform_request_result_destroy(platform_request_result_t* result);

static ret_t call_on_result_idle(const idle_info_t* info) {
  platform_request_result_t* result = (platform_request_result_t*)(info->ctx);
  platform_request_result_call(result);
  platform_request_result_destroy(result);

  return RET_REMOVE;
}

static ret_t platform_request_default_on_result(void* ctx, const char* data) {
  platform_request_result_t* result = (platform_request_result_t*)ctx;
  platform_request_result_set_result(result, data);

  idle_queue(call_on_result_idle, result);

  return RET_OK;
}

ret_t platform_request_init(void) {
  int socks[2] = {0, 0};
  return_value_if_fail(s_server_fd == 0, RET_BAD_PARAMS);

  if (tk_socketpair(socks) == RET_OK) {
    s_client_fd = socks[0];
    s_server_fd = socks[1];

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t platform_request_send(const char* target, const char* action, const char* args,
                            platform_request_on_result_t on_result, void* on_result_ctx) {
  int ret = 0;
  str_t str;
  char buff[32];
  str_t* s = &str;
  uint32_t size = 0;
  platform_request_result_t* result = NULL;
  return_value_if_fail(on_result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL && action != NULL && args != NULL, RET_BAD_PARAMS);

  size = strlen(target) + strlen(action) + strlen(args) + sizeof(on_result) +
         sizeof(on_result_ctx) + 3;

  result = platform_request_result_create(on_result, on_result_ctx);
  return_value_if_fail(result != NULL, RET_OOM);

  return_value_if_fail(str_init(s, size) != NULL, RET_BAD_PARAMS);

  tk_snprintf(buff, sizeof(buff), "%p:%p:", platform_request_default_on_result, result);
  str_append(s, buff);
  str_append(s, target);
  str_append_char(s, ':');
  str_append(s, action);
  str_append_char(s, ':');
  str_append(s, args);

#ifndef WITHOUT_PLUGINS
  return_value_if_fail(s_client_fd > 0, RET_BAD_PARAMS);

  ret = send(s_client_fd, s, sizeof(str), 0);
  assert(ret == sizeof(str));
  log_debug("send: %s\n", s->str);
/**
 * XXX: call str_reset by receiver thread
 * str_reset(&str);
 */
#else 
  (void)ret;
  log_debug("plugins not supported: %s\n", s->str);
  str_reset(&str);
#endif/*WITHOUT_PLUGINS*/

  return RET_OK;
}

ret_t platform_request_recv(str_t* str) {
  int ret = 0;
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(s_server_fd > 0, RET_BAD_PARAMS);
  ret = recv(s_server_fd, str, sizeof(str_t), 0);

  if (ret <= 0) {
    return RET_FAIL;
  }

  assert(ret == sizeof(str_t));
  log_debug("recv: %s\n", str->str);

  return ret == sizeof(str_t) ? RET_OK : RET_FAIL;
}

ret_t platform_request_deinit(void) {
  return_value_if_fail(s_server_fd > 0, RET_BAD_PARAMS);

  socket_close(s_client_fd);
  socket_close(s_server_fd);

  s_client_fd = 0;
  s_server_fd = 0;

  return RET_OK;
}

platform_request_result_t* platform_request_result_create(platform_request_on_result_t on_result,
                                                          void* ctx) {
  platform_request_result_t* result = NULL;
  return_value_if_fail(on_result != NULL, NULL);
  result = TKMEM_ZALLOC(platform_request_result_t);
  return_value_if_fail(result != NULL, NULL);
  result->on_result = on_result;
  result->on_result_ctx = ctx;

  return result;
}

ret_t platform_request_result_call(platform_request_result_t* result) {
  return_value_if_fail(result != NULL && result->on_result != NULL, RET_BAD_PARAMS);

  return result->on_result(result->on_result_ctx, result->result);
}

ret_t platform_request_result_set_result(platform_request_result_t* result, const char* data) {
  return_value_if_fail(result != NULL && data != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(result->result);
  result->result = tk_strdup(data);

  return RET_OK;
}

ret_t platform_request_result_destroy(platform_request_result_t* result) {
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(result->result);
  result->on_result = NULL;
  TKMEM_FREE(result);

  return RET_OK;
}
