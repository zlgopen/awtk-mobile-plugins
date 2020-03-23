/**
 * File:   button.c
 * Author: AWTK Develop Team
 * Brief:  button demo
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
 * 2018-08-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "image_picker/image_picker.h"

static ret_t image_picker_on_result(void* ctx, const char* data){
  str_t str;
  widget_t* win = WIDGET(ctx);
  const char* p = strchr(data, ':');
  widget_t* image = widget_lookup(win, "image", TRUE);
  widget_t* result = widget_lookup(win, "result", TRUE);

  str_init(&str, MAX_PATH);
  str_set(&str, "file://");

  if (p != NULL) {
    str_append_with_len(&str, data, p - data);
  } else {
    str_append(&str, data);
  }

  widget_set_text_utf8(result, data);
  image_set_image(image, str.str);

  log_debug("image_picker:%s\n", data);

  return RET_OK;
}

static ret_t on_click(void* ctx, event_t* e) {
  image_picker_options_t options;
  widget_t* win = widget_get_window(e->target);
  widget_t* single = widget_lookup(win, "single", TRUE);
  widget_t* include_videos = widget_lookup(win, "include_videos", TRUE);

  memset(&options, 0x00, sizeof(options));

  options.single = widget_get_value(single);
  options.include_videos = widget_get_value(include_videos);

  image_picker_pick(&options, image_picker_on_result, ctx);

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* image = image_create(win, 0, 0, 0, 0);
  widget_t* ok = button_create(win, 0, 0, 0, 0);
  widget_t* single = check_button_create(win, 0, 0, 0, 0);
  widget_t* include_videos = check_button_create(win, 0, 0, 0, 0);
  widget_t* result = label_create(win, 0, 0, 0, 0);
  
  widget_set_name(single, "single");
  widget_set_name(result, "result");
  widget_set_name(image, "image");
  widget_set_name(include_videos, "include_videos");

  widget_set_text(result, L"none");
  widget_set_text(single, L"Single Mode");
  widget_set_text(include_videos, L"Include Videos");
  widget_set_self_layout_params(single, "center", "middle:-100", "50%", "30");
  widget_set_self_layout_params(include_videos, "center", "middle:-50", "50%", "30");
  widget_set_self_layout_params(result, "center", "middle:60", "50%", "30");
  widget_set_self_layout_params(image, "center", "middle", "100%", "100%");

  widget_set_text(ok, L"Pick");
  widget_set_self_layout_params(ok, "center", "middle", "50%", "30");
  widget_on(ok, EVT_CLICK, on_click, win);

  widget_layout(win);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
