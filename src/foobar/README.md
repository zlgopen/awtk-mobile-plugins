# 演示调用另外一个app中的服务。

> Android服务app由misc/FooBarServer提供。

用法示例：

* 包含头文件

```c
#include "foobar/foobar.h"
```

* 调用方法

```c
static ret_t foobar_on_result(void* ctx, const char* data){
  log_debug("foobar:%s\n", data);

  return RET_OK;
}

foobar_add(100, 200, foobar_on_result, ctx);
```
