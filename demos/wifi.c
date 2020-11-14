/**
 * File:   wifi.c
 * Author: AWTK Develop Team
 * Brief:  wifi demo
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
 * 2020-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "wifi/wifi.h"
#include "conf_io/conf_json.h"

static ret_t wifi_on_result(void* ctx, const char* data){
  widget_t* result_label = WIDGET(ctx);
  conf_doc_t* doc = conf_doc_load_json(data, strlen(data));
  int32_t level = conf_doc_get_int(doc, "level", 0);

  widget_set_text_utf8(result_label, data);
  log_debug("wifi:%s level=%d\n", data, level);
  conf_doc_destroy(doc);

  return RET_OK;
}

static ret_t on_scan_clicked(void* ctx, event_t* e) {
  wifi_scan(wifi_on_result, ctx);
  return RET_OK;
}

static ret_t on_info_clicked(void* ctx, event_t* e) {
  wifi_get_info(wifi_on_result, ctx);
  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* scan = button_create(win, 0, 0, 0, 0);
  widget_t* info = button_create(win, 0, 0, 0, 0);
  widget_t* result = label_create(win, 0, 0, 0, 0);

  widget_set_text(result, L"none");
  widget_set_self_layout_params(result, "0", "0", "100%", "-60");

  widget_set_text(scan, L"Scan");
  widget_set_self_layout_params(scan, "center:-60", "bottom:10", "80", "30");
  widget_on(scan, EVT_CLICK, on_scan_clicked, result);
  
  widget_set_text(info, L"Info");
  widget_set_self_layout_params(info, "center:60", "bottom:10", "80", "30");
  widget_on(info, EVT_CLICK, on_info_clicked, result);
  
  widget_layout(win);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
