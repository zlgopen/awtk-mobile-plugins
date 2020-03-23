# 二维码识别插件

用法示例：

* 包含头文件

```c
#include "image_picker/image_picker.h
```

* 调用方法

```c
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
```

> 本插件基于[android-image-picker](https://github.com/esafirm/android-image-picker)实现。
