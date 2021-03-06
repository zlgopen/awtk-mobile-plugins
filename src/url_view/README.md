# url view插件。

使用第三方应用程序打开指定的URL/文件，包括拨打电话和发送邮件。

用法示例：

* 包含头文件

```c
#include "url_view/url_view.h"
```

* 调用方法

> 打开文件

```c
  url_view_request("/sdcard/test.png", "image/png");
  url_view_request("/sdcard/test.txt", "text/plain");
  url_view_request("/sdcard/test.pdf", "application/pdf");
  url_view_request("/sdcard/test.xls", "application/vnd.ms-excel");
  url_view_request("/sdcard/test.xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
```
> 文件必须放到T卡中，否则第三方应用程序没有权限打开。

> 打开浏览器

```
  url_view_request("https://github.com/zlgopen/awtk", "*/*");
```

> 拨打电话

```
  url_view_request("tel://10086", "*/*");
```

> 发送邮件

``` 
  url_view_request("mailto:name@rapidtables.com", "*/*");
```


