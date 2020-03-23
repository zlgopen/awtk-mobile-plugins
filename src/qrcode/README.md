# 二维码识别插件

用法示例：

* 包含头文件

```c
#include "qrcode/qrcode.h"
```

* 调用方法

```c
static ret_t qrcode_on_result(void* ctx, const char* data){
  log_debug("qrcode:%s\n", data);

  return RET_OK;
}

qrcode_scan("scan qrcode", qrcode_on_result, ctx);
```

> 本插件基于[ZXingLite](https://github.com/jenly1314/ZXingLite)实现。