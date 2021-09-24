# misc插件。

用于包装一些杂七杂八的原生函数，需要的时候加一个函数就行了，而不需要加一个插件。

用法示例：

* 包含头文件

```c
#include "misc/misc.h"
```

* 调用方法

> 通知Anadroid重新扫描文件。

```c
notifySystemToScan(filename);
```

## 增加新函数

* 修改misc/misc.h，定义函数。如：

```c
#define notifySystemToScan(filename) misc_call_func("notifySystemToScan", filename, NULL, NULL);
```

* 修改android/java/org/zlgopen/plugins/misc/MiscPlugin.java，实现函数。如：

```java
    public void notifySystemToScan(String filePath) {
        Intent intent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
        File file = new File(filePath);
        Uri uri = Uri.fromFile(file);

        intent.setData(uri);
        this.activity.sendBroadcast(intent);
    }

    void call(String name, String args) {
      if(name.equals("notifySystemToScan")) {
        this.notifySystemToScan(filePath);
        PluginManager.writeResult(this.callerInfo, "ok");
      } else {
        Log.v("AWTK", "Not supported function " + name);
        PluginManager.writeResult(this.callerInfo, "fail");
      }
    }
```
