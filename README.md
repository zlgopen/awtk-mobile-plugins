# awtk-mobile-plugins

用于访问手机平台 (android/ios) 原生服务的插件(如分享、登录、相机、扫描二维码和蓝牙等)

>本项目目前处于实验阶段

## 准备

1. 获取 awtk 并编译

```
git clone https://github.com/zlgopen/awtk.git
cd awtk; scons; cd -
```

2. 获取 awtk-mobile-plugins

```
git clone https://github.com/zlgopen/awtk-mobile-plugins.git
```

3. 获取 awtk-android

```
git clone https://github.com/zlgopen/awtk-android.git
```

4. 获取 awtk-ios

```
git clone https://github.com/zlgopen/awtk-ios.git
```

> 请把以上项目并列放在同一个目录。

## 编译 android 版本示例 (qrcode demo)

> 确保 android 的 sdk、ndk 和 gradle 都正确安装。

* 进入 awtk-android 目录。

```
cd awtk-android 
```

* 创建工程

```
python create_project.py ../awtk-mobile-plugins/qrcode_build.json
```

* 编译工程

```
cd build/qrcodedemo && ./gradlew build
```

* 安装到手机

```
adb install -r ./app/build/outputs/apk/debug/app-debug.apk
```

> 不同平台有些差异，请酌情修改。建议使用 bash，Windows 下可以使用 git bash。

## 创建并编译自己的工程

创建并编译自己的工程需要先写一个配置文件，请参考 [Android 平台编译配置](https://github.com/zlgopen/awtk/blob/master/docs/build_config.md)

App 引用的插件放到对应平台下的 plugins 键下。如：

```
  "android" : { 
    "plugins":["qrcode"],
    "app_name":"org.zlgopen.qrcodedemo",
    "sources":["demos/qrcode.c",  "demos/assets*.c", "demos/assets*.inc"]
  }, 
```


## 编译 ios 版本（暂未支持）

## 插件列表

* [share 分享插件](src/share/README.md)

* [qrcode 二维码识别插件](src/qrcode/README.md)

* [image_picker 图片/视频选择插件](src/image_picker/README.md)

* [battery 电池电量插件](src/battery/README.md)

* [wifi 无线网络插件](src/wifi/README.md)

## 文档

* [系统架构](docs/arch.md)
* [插件描述文件](docs/plugin_config.md)
* [实现插件客户端](docs/plugin_client.md)
* [实现插件服务端(android)](docs/plugin_android.md)
* [实现插件服务端(ios)](docs/plugin_ios.md)
* [插件目录结构](docs/plugin_folder_struct.md)

