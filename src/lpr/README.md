# 车牌识别插件

## 用法示例：

* 包含头文件

```c
#include "lpr/lpr.h"
```

* 调用方法

```c
static ret_t lpr_on_result(void* ctx, const char* data){
  log_debug("lpr:%s\n", data);

  return RET_OK;
}

lpr_scan("scan lpr", lpr_on_result, ctx);
```

> 本插件基于[lpr](https://github.com/zlgopen/awtk-lpr)实现。


## 编译方法

请参考[awtk-lpr-android](https://github.com/zlgopen/awtk-lpr-android)
