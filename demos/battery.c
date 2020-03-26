/**
 * File:   battery.c
 * Author: AWTK Develop Team
 * Brief:  battery demo
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
 * 2020-03-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "battery/battery.h"

static ret_t battery_on_event(void* ctx, const char* data){
  widget_t* result_label = WIDGET(ctx);

  widget_set_text_utf8(result_label, data);
  log_debug("battery:%s\n", data);

  return RET_OK;
}

static ret_t battery_on_result(void* ctx, const char* data){
  widget_t* result_label = WIDGET(ctx);

  widget_set_text_utf8(result_label, data);
  widget_invalidate_force(result_label, NULL);
  log_debug("battery:%s\n", data);

  return RET_OK;
}

static ret_t on_click(void* ctx, event_t* e) {

  battery_get_info(battery_on_result, ctx);
  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* ok = button_create(win, 0, 0, 0, 0);
  widget_t* result = label_create(win, 0, 0, 0, 0);

  widget_set_text(result, L"none");
  widget_set_self_layout_params(result, "center", "middle:60", "100%", "30");

  widget_set_text(ok, L"Update");
  widget_set_self_layout_params(ok, "center", "middle", "50%", "30");
  widget_on(ok, EVT_CLICK, on_click, result);
  battery_register(battery_on_event, result);

  widget_layout(win);

  return RET_OK;
}

ret_t application_exit() {
  battery_unregister();
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
