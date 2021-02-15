#include "awtk.h"
#include "ble/ble.h"
#include "ble/ble_uuid.h"
#include "ble_win_device.h"
#include "ble_win_service.h"

static ret_t ble_device_on_destroy(void* ctx, event_t* e) {
  ble_device_t* device = (ble_device_t*)ctx;
  ble_set_on_device_services_discovered(device->ble, NULL, NULL);

  return RET_REMOVE;
}

static ret_t ble_device_on_item_clicked(void* ctx, event_t* e) {
  ble_service_t* service = (ble_service_t*)ctx;

  ble_win_service(service);

  return RET_OK;
}

static ret_t ble_device_update_service(widget_t* win, ble_device_t* device) {
  char text[256];
  uint32_t i = 0;
  uint32_t nr = device->services.size;
  widget_t* services_view = widget_lookup(win, "services", TRUE);
  widget_t* name_view = widget_lookup(win, "name", TRUE);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);
  widget_destroy_children(services_view);

  tk_snprintf(text, sizeof(text) - 1, "%s(address=%s rssi=%d)", device->name, device->id,
              device->rssi);
  widget_set_text_utf8(name_view, text);

  for (i = 0; i < nr; i++) {
    ble_service_t* iter = ble_device_get_service(device, i);
    widget_t* item = list_item_create(services_view, 0, 0, 0, 0);
    const char* style = i % 2 == 0 ? "even_clickable" : "odd_clickable";
    const char* uuid_desc = ble_uuid_to_desc(iter->id);

    tk_snprintf(text, sizeof(text) - 1, "%s(%u)", uuid_desc != NULL ? uuid_desc : iter->id,
                iter->chars.size);
    widget_set_text_utf8(item, text);
    widget_use_style(item, style);
    widget_on(item, EVT_CLICK, ble_device_on_item_clicked, iter);
  }

  return RET_OK;
}

static ret_t ble_deivce_on_device_services_discovered(void* ctx, ble_device_t* device) {
  widget_t* win = WIDGET(ctx);

  ble_device_update_service(win, device);

  return RET_OK;
}

ret_t ble_win_device(ble_device_t* device) {
  widget_t* win = NULL;
  return_value_if_fail(device != NULL, RET_BAD_PARAMS);
  win = window_open("ble_device");
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  ble_set_on_device_services_discovered(device->ble, ble_deivce_on_device_services_discovered, win);
  ble_device_update_service(win, device);
  widget_on(win, EVT_DESTROY, ble_device_on_destroy, device);

  return RET_OK;
}
