#include "awtk.h"
#include "ble_win_char.h"
#include "ble/ble.h"
#include "ble/ble_uuid.h"
#include "ble/ble_service.h"
#include "ble/ble_device.h"
#include "ble/ble_char.h"

static const char* ble_decode_data(str_t* str, const char* data) {
  uint8_t text[256];
  uint32_t i = 0;
  uint32_t nr = strlen(data) / 3;

  str_set(str, data);
  memset(text, 0x00, sizeof(text));
  str_decode_hex(str, text, sizeof(text) - 1);
  str_append_char(str, '(');

  for (i = 0; i < nr; i++) {
    if (isprint(text[i])) {
      str_append_char(str, text[i]);
    } else {
      str_append_char(str, '*');
    }
  }
  str_append_char(str, ')');

  return str->str;
}

static ret_t ble_on_characteristic_changed(void* ctx, const char* address, const char* uuid,
                                           const char* data) {
  str_t str;
  widget_t* win = widget_get_window(WIDGET(ctx));
  widget_t* notify_data = widget_lookup(win, "notify_data", TRUE);

  str_init(&str, 100);
  widget_set_text_utf8(notify_data, ble_decode_data(&str, data));
  str_reset(&str);

  return RET_OK;
}

static ret_t ble_on_characteristic_read(void* ctx, const char* address, const char* uuid,
                                        const char* data) {
  str_t str;
  widget_t* win = widget_get_window(WIDGET(ctx));
  widget_t* read_data = widget_lookup(win, "read_data", TRUE);

  str_init(&str, 100);
  widget_set_text_utf8(read_data, ble_decode_data(&str, data));
  str_reset(&str);

  return RET_OK;
}

static ret_t ble_on_characteristic_write(void* ctx, const char* address, const char* uuid) {
  return RET_OK;
}

static ret_t ble_char_on_read_clicked(void* ctx, event_t* e) {
  ble_char_t* achar = (ble_char_t*)ctx;
  ble_service_t* service = achar->service;
  ble_device_t* device = service->device;

  ble_read_characteristic(device->ble, device->id, achar->id);
  //ble_read_descriptor(device->ble, device->id, "00002902-0000-1000-8000-00805f9b34fb", FALSE);
  //ble_read_descriptor(device->ble, device->id, "00002901-0000-1000-8000-00805f9b34fb", FALSE);

  return RET_OK;
}

static ret_t ble_char_on_write_clicked(void* ctx, event_t* e) {
  char data[256];
  ble_char_t* achar = (ble_char_t*)ctx;
  ble_service_t* service = achar->service;
  ble_device_t* device = service->device;
  widget_t* win = widget_get_window(WIDGET(e->target));
  widget_t* write_data = widget_lookup(win, "write_data", TRUE);
  widget_get_text_utf8(write_data, data, sizeof(data) - 1);

  ble_write_characteristic(device->ble, device->id, achar->id, data, FALSE);

  return RET_OK;
}

static ret_t ble_char_on_destroy(void* ctx, event_t* e) {
  ble_char_t* achar = (ble_char_t*)ctx;
  ble_service_t* service = achar->service;
  ble_device_t* device = service->device;

  ble_set_on_characteristic_changed(device->ble, NULL, NULL);
  ble_set_on_characteristic_write(device->ble, NULL, NULL);
  ble_set_on_characteristic_read(device->ble, NULL, NULL);

  return RET_REMOVE;
}

ret_t ble_win_char(ble_char_t* achar) {
  char text[256];
  widget_t* win = NULL;
  ble_service_t* service = NULL;
  ble_device_t* device = NULL;
  const char* uuid_desc = NULL;

  return_value_if_fail(achar != NULL, RET_BAD_PARAMS);
  service = achar->service;
  device = service->device;

  win = window_open("ble_char");
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  uuid_desc = ble_uuid_to_desc(achar->id);
  tk_snprintf(text, sizeof(text) - 1, "%s(%u)", uuid_desc != NULL ? uuid_desc : achar->id,
              achar->descs.size);
  widget_set_text_utf8(widget_lookup(win, "name", TRUE), text);

  widget_on(win, EVT_DESTROY, ble_char_on_destroy, achar);
  widget_child_on(win, "read", EVT_CLICK, ble_char_on_read_clicked, achar);
  widget_child_on(win, "write", EVT_CLICK, ble_char_on_write_clicked, achar);

  widget_set_enable(widget_lookup(win, "read", TRUE), achar->readable);
  widget_set_enable(widget_lookup(win, "write", TRUE), achar->writable);

  ble_set_on_characteristic_changed(device->ble, ble_on_characteristic_changed, win);
  ble_set_on_characteristic_write(device->ble, ble_on_characteristic_write, win);
  ble_set_on_characteristic_read(device->ble, ble_on_characteristic_read, win);

  return RET_OK;
}
