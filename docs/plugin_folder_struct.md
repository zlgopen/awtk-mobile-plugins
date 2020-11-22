# 插件目录结构

每个插件都要提供下列文件：

* README.md。描述插件的功能和使用说明。

* 客户端的 .h/c 代码。供 AWTK 应用程序调用的。

* plugin.json 插件的配置文件。具体格式请参考 [plugin_config.md](plugin_config.md)

* android 目录下放 android 相关代码。主要存放 Android 需要的 aidl 文件和 java 文件，其路径格式和 java 保持一致。

* ios 目录下放 ios 相关代码。

示例：二维码插件目录结构：

```
│  plugin.json
│  qrcode.c
│  qrcode.h
│  README.md
│
└─android
    └─java
        └─org
            └─zlgopen
                └─plugins
                    └─qrcode
                            QrCodePlugin.java
```

> 为一个已经开发好的插件添加新的 API，通常需要修改客户端的 .h/c 代码和 Android 的 xxxxPlugin.java。
