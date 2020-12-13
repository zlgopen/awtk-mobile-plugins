# BLE

> Not work yet

用法示例：

* 包含头文件

```c
#include "ble/ble.h"
```

* 调用方法

```c
static ret_t ble_on_result(void* ctx, const char* data){
  log_debug("ble:%s\n", data);

  return RET_OK;
}

ble_start(ble_on_result, ctx);
```

