# 电池电量信息与状态插件

用法示例：

* 包含头文件

```c
#include "battery/battery.h"
```

* 调用方法

```c
static ret_t battery_on_result(void* ctx, const char* data){
  log_debug("battery:%s\n", data);

  return RET_OK;
}

battery_get_info(battery_on_result, ctx);
```
