# 二维码识别插件

用法示例：

* 包含头文件

```c
#include "image_picker/image_picker.h
```

* 调用方法

```c
static ret_t image_picker_on_result(void* ctx, const char* data){
  widget_t* result_label = WIDGET(ctx);

  widget_set_text_utf8(result_label, data);
  log_debug("image_picker:%s\n", data);

  return RET_OK;
}

static ret_t on_click(void* ctx, event_t* e) {
  image_picker_options_t options;

  memset(&options, 0x00, sizeof(options));
  image_picker_pick(&options, image_picker_on_result, ctx);

  return RET_OK;
}
```

> 本插件基于[android-image-picker](https://github.com/esafirm/android-image-picker)实现。
