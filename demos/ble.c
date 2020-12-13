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

static ret_t ble_on_result(void *ctx, const char *data) {
    widget_t *result_label = WIDGET(ctx);
    conf_doc_t *doc = conf_doc_load_json(data, strlen(data));
    int32_t level = conf_doc_get_int(doc, "level", 0);

    widget_set_text_utf8(result_label, data);
    log_debug("ble:%s level=%d\n", data, level);
    conf_doc_destroy(doc);

    return RET_OK;
}

static ret_t ble_on_notify(void *ctx, const char *data) {
    widget_t *result_label = WIDGET(ctx);
    conf_doc_t *doc = conf_doc_load_json(data, strlen(data));
    int32_t level = conf_doc_get_int(doc, "level", 0);

    widget_set_text_utf8(result_label, data);
    log_debug("ble:%s level=%d\n", data, level);
    conf_doc_destroy(doc);

    return RET_OK;
}

static ret_t on_start_clicked(void *ctx, event_t *e) {
    ble_start(ble_on_result, ctx);
    return RET_OK;
}

static ret_t on_reg_clicked(void *ctx, event_t *e) {
    ble_register(ble_on_notify, ctx);
    return RET_OK;
}

static ret_t on_unreg_clicked(void *ctx, event_t *e) {
    ble_unregister();
    return RET_OK;
}

static ret_t on_connect_clicked(void *ctx, event_t *e) {
    char addr[64];
    widget_t *win = widget_get_window((WIDGET(e->target)));
    widget_t* widget = widget_lookup(win, "addr", TRUE);

    memset(addr, 0x00, sizeof(addr));
    widget_get_text_utf8(widget, addr, sizeof(addr) - 1);

    if (strlen(addr) > 10) {
        ble_connect(addr, ble_on_result, ctx);
    }

    return RET_OK;
}

static ret_t on_disconnect_clicked(void *ctx, event_t *e) {
    char addr[64];
    widget_t *win = widget_get_window((WIDGET(e->target)));
    widget_t* widget = widget_lookup(win, "addr", TRUE);

    memset(addr, 0x00, sizeof(addr));
    widget_get_text_utf8(widget, addr, sizeof(addr) - 1);

    if (strlen(addr) > 10) {
        ble_disconnect(addr, ble_on_result, ctx);
    }

    return RET_OK;
}


static ret_t on_start_scan_clicked(void *ctx, event_t *e) {
    ble_start_scan(ble_on_result, ctx);
    return RET_OK;
}

static ret_t on_stop_scan_clicked(void *ctx, event_t *e) {
    ble_stop_scan(ble_on_result, ctx);
    return RET_OK;
}

static ret_t on_info_clicked(void *ctx, event_t *e) {
    ble_get_info(ble_on_result, ctx);
    return RET_OK;
}

ret_t application_init() {
    widget_t *win = window_create(NULL, 0, 0, 0, 0);
    widget_t *result = label_create(win, 0, 0, 0, 0);
    widget_t *addr = edit_create(win, 0, 0, 0, 0);
    widget_t *button_group = view_create(win, 0, 0, 0, 0);

    widget_t *start = button_create(button_group, 0, 0, 0, 0);
    widget_t *reg = button_create(button_group, 0, 0, 0, 0);
    widget_t *unreg = button_create(button_group, 0, 0, 0, 0);
    widget_t *conn = button_create(button_group, 0, 0, 0, 0);
    widget_t *disconn = button_create(button_group, 0, 0, 0, 0);

    widget_t *start_scan = button_create(button_group, 0, 0, 0, 0);
    widget_t *stop_scan = button_create(button_group, 0, 0, 0, 0);
    widget_t *info = button_create(button_group, 0, 0, 0, 0);

    widget_set_text(result, L"none");
    widget_set_self_layout_params(result, "0", "0", "100%", "-80");
    widget_set_self_layout_params(addr, "center", "midlle", "50%", "30");
    widget_set_name(addr, "addr");
    edit_set_input_tips(addr, "device address");
    widget_set_text(addr, L"5C:E5:0C:90:50:8B");

    widget_set_self_layout_params(button_group, "0", "bottom", "100%", "128");
    widget_set_children_layout(button_group, "default(r=2,c=4,m=2,s=2)");

    widget_set_text(start, L"Start");
    widget_on(start, EVT_CLICK, on_start_clicked, result);

    widget_set_text(reg, L"Reg");
    widget_on(reg, EVT_CLICK, on_reg_clicked, result);

    widget_set_text(unreg, L"Unreg");
    widget_on(unreg, EVT_CLICK, on_unreg_clicked, result);

    widget_set_text(conn, L"Connect");
    widget_on(conn, EVT_CLICK, on_connect_clicked, result);

    widget_set_text(disconn, L"Disconnect");
    widget_on(disconn, EVT_CLICK, on_disconnect_clicked, result);

    widget_set_text(start_scan, L"Start Scan");
    widget_on(start_scan, EVT_CLICK, on_start_scan_clicked, result);

    widget_set_text(stop_scan, L"Stop Scan");
    widget_on(stop_scan, EVT_CLICK, on_stop_scan_clicked, result);

    widget_set_text(info, L"Info");
    widget_on(info, EVT_CLICK, on_info_clicked, result);

    widget_layout(win);

    return RET_OK;
}

ret_t application_exit() {
    log_debug("application_exit\n");
    return RET_OK;
}

#include "awtk_main.inc"
