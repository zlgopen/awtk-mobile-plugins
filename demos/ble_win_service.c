#include "awtk.h"
#include "ble/ble_char.h"
#include "ble/ble_uuid.h"
#include "ble/ble_service.h"
#include "ble_win_service.h"
#include "ble_win_char.h"

static ret_t ble_service_on_item_clicked(void* ctx, event_t* e) {
  ble_char_t* achar = (ble_char_t*)ctx;

  ble_win_char(achar);

  return RET_OK;
}

static ret_t ble_service_update_chars(widget_t* win, ble_service_t* service) {
  char text[256];
  uint32_t i = 0;
  uint32_t nr = service->chars.size;
  widget_t* chars_view = widget_lookup(win, "chars", TRUE);
  widget_t* name_view = widget_lookup(win, "name", TRUE);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);
  widget_destroy_children(chars_view);

  tk_snprintf(text, sizeof(text) - 1, "%s's chars", service->id);
  widget_set_text_utf8(name_view, text);

  for (i = 0; i < nr; i++) {
    ble_char_t* iter = ble_service_get_char(service, i);
    widget_t* item = list_item_create(chars_view, 0, 0, 0, 0);
    const char* style = i % 2 == 0 ? "even_clickable" : "odd_clickable";
    const char* uuid_desc = ble_uuid_to_desc(iter->id);

    tk_snprintf(text, sizeof(text) - 1, "%s(%u)", uuid_desc != NULL ? uuid_desc : iter->id,
                iter->descs.size);
    widget_set_text_utf8(item, text);
    widget_use_style(item, style);
    widget_on(item, EVT_CLICK, ble_service_on_item_clicked, iter);
  }

  return RET_OK;
}

ret_t ble_win_service(ble_service_t* service) {
  widget_t* win = NULL;
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  win = window_open("ble_service");
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  ble_service_update_chars(win, service);

  return RET_OK;
}
