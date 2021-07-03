/**
 * File:   awake.c
 * Author: AWTK Develop Team
 * Brief:  awake demo
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
 * 2021-07-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "awake/awake.h"

static ret_t on_value_changed(void* ctx, event_t* e) {
  value_change_event_t* evt = value_change_event_cast(e);
  bool_t value = value_bool(&(evt->new_value));

  log_debug("value=%d\n", (int)value);

  awake_keep_screen_on(value);

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* ok = check_button_create(win, 0, 0, 0, 0);

  widget_set_text(ok, L"Keep Screen On");
  widget_set_self_layout_params(ok, "center", "middle", "50%", "30");
  widget_on(ok, EVT_VALUE_CHANGED, on_value_changed, NULL);

  widget_layout(win);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
