/**
 * File:   url_view.c
 * Author: AWTK Develop Team
 * Brief:  url_view demo
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
 * 2021-03-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "url_view/url_view.h"

static ret_t on_file_click(void* ctx, event_t* e) {
  char filename[MAX_PATH+1];
  const char* data = "hello awtk\n";
#ifdef ANDROID
  extern const char * SDL_AndroidGetExternalStoragePath(void);
  const char* dir = SDL_AndroidGetExternalStoragePath();
  path_build(filename, sizeof(filename)-1, dir, "test.txt", NULL);
#else
  char home[MAX_PATH+1];
  fs_get_user_storage_path(os_fs(), home);
  path_build(filename, sizeof(filename)-1, home, "test.txt", NULL);
#endif

  file_write(filename, data, strlen(data));

  url_view_request(filename, "text/plain");

  return RET_OK;
}

static ret_t on_http_click(void* ctx, event_t* e) {
    url_view_request("https://github.com/zlgopen/awtk", "*/*");

    return RET_OK;
}

static ret_t on_tel_click(void* ctx, event_t* e) {
    url_view_request("tel://10086", "*/*");

    return RET_OK;
}

static ret_t on_mail_click(void* ctx, event_t* e) {
    url_view_request("mailto:name@rapidtables.com", "*/*");
    return RET_OK;
}


ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* file = button_create(win, 0, 0, 0, 0);
  widget_t* http = button_create(win, 0, 0, 0, 0);
  widget_t* mail = button_create(win, 0, 0, 0, 0);
  widget_t* tel = button_create(win, 0, 0, 0, 0);

  widget_set_text(file, L"File");
  widget_set_self_layout_params(file, "center", "middle:-40", "50%", "30");
  widget_on(file, EVT_CLICK, on_file_click, NULL);


    widget_set_text(http, L"HTTP");
    widget_set_self_layout_params(http, "center", "middle", "50%", "30");
    widget_on(http, EVT_CLICK, on_http_click, NULL);


    widget_set_text(mail, L"Mail");
    widget_set_self_layout_params(mail, "center", "middle:40", "50%", "30");
    widget_on(mail, EVT_CLICK, on_mail_click, NULL);


    widget_set_text(tel, L"Telephone");
    widget_set_self_layout_params(tel, "center", "middle:80", "50%", "30");
    widget_on(tel, EVT_CLICK, on_tel_click, NULL);

  widget_layout(win);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
