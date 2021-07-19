# 用于禁止或允许设备睡眠。

用法示例：

* 包含头文件

```c
#include "wake_lock/wake_lock.h"
```

* 调用方法

> 禁止睡眠。
```c
wake_lock_require();
```

> 允许睡眠。
```c
wake_lock_release();
```
