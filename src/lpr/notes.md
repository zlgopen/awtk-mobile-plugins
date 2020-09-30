# 修改

* 1.修改build.gradle

```
ext {
    //android开发版本配置
    android = [
            compileSdkVersion: 29,
            applicationId    : "com.pcl.lpr",
            minSdkVersion    : 21,
            targetSdkVersion : 29,
            versionCode      : 1,
            versionName      : "1.0",
    ]
}
```

2.修改settings.gradle

```
include ':openCV'
project(':openCV').projectDir = new File(settingsDir, '../../../awtk-lpr-android/openCV')

include ':ocr'
project(':ocr').projectDir = new File(settingsDir, '../../../awtk-lpr-android/ocr')
```

3.修改app/build.gradle

> 已自动添加

```
dependencies {
  implementation project(path: ':ocr')
}
```



