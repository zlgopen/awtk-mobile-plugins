/**
 * File:   ble.c
 * Author: AWTK Develop Team
 * Brief:  ble demo
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
#include "ble/ble.h"
#include "conf_io/conf_json.h"

static ret_t ble_main_on_scan_clicked(void* ctx, event_t* e) {
  ble_t* ble = (ble_t*)ctx;
  widget_t* button = WIDGET(e->target);
  widget_t* win = widget_get_window(button);

  if (ble->scanning) {
    ble_stop_scan(ble);
    widget_set_text_utf8(button, "Scan");
  } else {
    ble_start_scan(ble);
    widget_destroy_children(widget_lookup(win, "devices", TRUE));
    widget_set_text_utf8(button, "Stop");
  }

  return RET_OK;
}

#include "ble_win_device.h"

static ret_t ble_main_on_item_clicked(void* ctx, event_t* e) {
  ble_device_t* device = (ble_device_t*)ctx;

  ble_connect_to(device->ble, device->id);
  ble_win_device(device);

  return RET_OK;
}

static ret_t ble_main_on_device_found(void* ctx, ble_device_t* device) {
  char text[256];
  widget_t* win = WIDGET(ctx);
  widget_t* devices_view = widget_lookup(win, "devices", TRUE);
  uint32_t nr = widget_count_children(devices_view);
  const char* style = nr % 2 == 0 ? "even_clickable" : "odd_clickable";
  if (device->rssi > -100) {
    widget_t* item = list_item_create(devices_view, 0, 0, 0, 0);
    tk_snprintf(text, sizeof(text) - 1, "%s(address=%s rssi=%d)", device->name, device->id,
                device->rssi);
    widget_set_text_utf8(item, text);
    widget_use_style(item, style);
    widget_on(item, EVT_CLICK, ble_main_on_item_clicked, device);
  }

  return RET_OK;
}

static ret_t ble_main_on_window_destroy(void* ctx, event_t* e) {
  ble_t* ble = (ble_t*)ctx;
  ble_destroy(ble);

  return RET_OK;
}

ret_t application_init() {
  ble_t* ble = NULL;

  widget_t* win = window_open("ble_main");

  ble = ble_create();

  ble_set_on_device_found(ble, ble_main_on_device_found, win);
  widget_on(win, EVT_DESTROY, ble_main_on_window_destroy, ble);
  widget_child_on(win, "scan", EVT_CLICK, ble_main_on_scan_clicked, ble);
  widget_destroy_children(widget_lookup(win, "devices", TRUE));

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
