# 插件描述文件

每个插件都需要提供一个 JSON 格式的描述文件 plugin.json，并放插件根目录中。

## 基本信息

* name 名称。
* version 版本。
* author 作者。
* desc 描述信息。

## android 平台相关的信息

* class 类的名称。用于注册时创建对象。
* permissions 权限列表。
* activities 包含的 activities列表。
* dependencies 对第三方库的依赖列表。

示例：二维码描述文件。

```json
{
  "name":"qrcode",
  "version":1.0,
  "author":"awtk team",
  "desc":"qrcode scan",
  "android" : {
    "class":"QrCode",
    "permissions" : ["<uses-permission android:name=\"android.permission.CAMERA\" />"],
    "activities" : ["<activity android:name=\"com.king.zxing.CaptureActivity\" android:screenOrientation=\"portrait\" android:theme=\"@style/CaptureTheme\"/>"],
    "dependencies":["implementation 'com.king.zxing:zxing-lite:1.1.6-androidx'",
      "api 'androidx.appcompat:appcompat:1.1.0'",
      "api 'com.google.zxing:core:3.3.3'"]
  }
}
```