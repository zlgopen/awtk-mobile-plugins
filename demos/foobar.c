/**
 * File:   foobar.c
 * Author: AWTK Develop Team
 * Brief:  foobar demo
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
 * 2020-11-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "foobar/foobar.h"
#include "conf_io/conf_json.h"

static ret_t foobar_on_result(void* ctx, const char* data){
  widget_t* result_label = WIDGET(ctx);
  conf_doc_t* doc = conf_doc_load_json(data, strlen(data));
  int result = conf_doc_get_int(doc, "result", 0);
  log_debug("foobar:%s\n result:%d\n", data, result);
  conf_doc_destroy(doc);

  widget_set_text_utf8(result_label, data);
  widget_invalidate_force(result_label, NULL);

  return RET_OK;
}

static ret_t on_click(void* ctx, event_t* e) {
  foobar_add(100, 200, foobar_on_result, ctx);
  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* ok = button_create(win, 0, 0, 0, 0);
  widget_t* result = label_create(win, 0, 0, 0, 0);

  widget_set_self_layout_params(result, "center", "middle:60", "100%", "30");

  widget_set_text(ok, L"Add");
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
