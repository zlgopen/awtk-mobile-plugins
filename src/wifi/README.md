# WIFI

用法示例：

* 包含头文件

```c
#include "wifi/wifi.h"
```

* 调用方法

```c
static ret_t wifi_on_result(void* ctx, const char* data){
  log_debug("wifi:%s\n", data);

  return RET_OK;
}

wifi_scan(wifi_on_result, ctx);
```

