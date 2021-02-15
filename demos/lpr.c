/**
 * File:   lpr.c
 * Author: AWTK Develop Team
 * Brief:  lpr demo
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-09-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "lpr/lpr.h"

static ret_t lpr_on_result(void* ctx, const char* data) {
  widget_t* result_label = WIDGET(ctx);

  widget_set_text_utf8(result_label, data);
  log_debug("lpr:%s\n", data);

  return RET_OK;
}

static ret_t on_click(void* ctx, event_t* e) {
  lpr_scan("hello", lpr_on_result, ctx);
  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* ok = button_create(win, 0, 0, 0, 0);
  widget_t* result = label_create(win, 0, 0, 0, 0);

  widget_set_text(result, L"none");
  widget_set_self_layout_params(result, "center", "middle:60", "100%", "30");

  widget_set_text(ok, L"Scan");
  widget_set_self_layout_params(ok, "center", "middle", "50%", "30");
  widget_on(ok, EVT_CLICK, on_click, result);

  widget_layout(win);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
