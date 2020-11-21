# AWTK 在 Android 平台实现跨进程服务调用

前几天，有朋友想在 AWTK 的应用程序中，调用打印机提供的服务，去打印采集到的数据。打印机本身有一个 APP，它提供了一个对外的服务，并附带一个 AIDL 文件。

要说这个东西本身也不难，但是要在 AWTK 应用程序中，通过 C 语言函数去调用 java 的函数，再通过 java 的函数去调用另外一个进程的服务，对于只会 C 语言的嵌入式程序员来说，不懂 java 也不懂 android，还绕了几个大弯，那确实太难了。

这里用一个 FooBar 的服务，演示一下整个开发过程，供有需要的童鞋参考。

## 1. 开发提供服务的 APP

先用 Android Studio 新建一个普通的 Android Java 项目 FooBarServer。

### 1.1 新建一个 aidl 文件。

文件位置：app/src/main/aidl/org/zlgopen/foobarserver/IFooBarService.aidl

```java
// IFooBarService.aidl
package org.zlgopen.foobarserver;

// Declare any non-default types here with import statements

interface IFooBarService {
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     */
    int add(int a, int b);
}
```

> 为了方便说明，这里只是提供了一个 add 函数，输入 a 和 b 两个参数，返回两者之和。

### 1.2 新建一个服务。

文件位置：app/src/main/java/org/zlgopen/foobarserver/FooBarService.java

```
package org.zlgopen.foobarserver;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class FooBarService extends Service {
    public FooBarService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        return binder;
    }

    private final IFooBarService.Stub binder = new IFooBarService.Stub() {
        public int add(int a, int b) {
            return a + b;
        }
    };
}
```

### 1.3 修改 AndroidManifest.xml 对外声明提供的服务。

```xml
        <service
            android:name=".FooBarService"
            android:enabled="true"
            android:process=":remote">
            <intent-filter>
                <action android:name="org.zlgopen.foobarserver.IFooBarService"/>
            </intent-filter>
        </service>
```

> 要跨进程调用，这行代码很关键：android:process=":remote"

## 2. 开发使用服务的 APP

为了验证服务端的正确性，我们再用 Android Studio 创建一个普通的 Android Java 应用程序 FooBarClient。

### 2.1 拷贝 IFooBarService.aidl

把 app/src/main/aidl/org/zlgopen/foobarserver/IFooBarService.aidl 从 FooBarService 拷贝到 FooBarClient 下，并保持目录结构不变。

### 2.2 修改 MainActivity.java

建立与服务的连接，并调用相应的函数。

```java

    public static IFooBarService mFooBarService;
    private ServiceConnection mServiceConn = new ServiceConnection() {
        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.e("client", "onServiceDisconnected");
            mFooBarService = null;
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.e("client", "onServiceConnected");
            mFooBarService = IFooBarService.Stub.asInterface(service);
            try {
                int result = mFooBarService.add(100, 200);
                Log.e("client", Integer.toString(result));
            }catch (RemoteException e) {
                Log.e("client", e.toString());
            }
        }
    };

    public void bindService() {
        Intent intent = new Intent("org.zlgopen.foobarserver.IFooBarService");
        intent.setPackage("org.zlgopen.foobarserver");
        bindService(intent, mServiceConn, Context.BIND_AUTO_CREATE);
    }

    public void unbindService() {
        unbindService(mServiceConn);
    }
 ```

> 编译并安装 FooBarServer 和 FooBarClient。

 ## 3. 编写 AWTK 插件

### 3.1 C 语言 API 接口

AWTK 的 UI 线程和 Android 的 Activity 线程并不是同一个，所以 AWTK 的插件全部是异步接口，调用完成后通过回调函数返回执行结果。

> 参数和返回值都使用 JSON 格式的字符串。

```c
/**
 * @method foobar_add
 * 调用 add 函数。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {int} a a。
 * @param {int} b b。
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t foobar_add(int a, int b, platform_request_on_result_t on_result, void* on_result_ctx);
```

实现很简单：把参数打包成 JSON 格式的字符串，再用 platform\_request\_send 发送给插件管理器，由插件管理器负责把请求转发给插件。

```c
ret_t foobar_add(int a, int b, platform_request_on_result_t on_result, void* on_result_ctx) {
  ret_t ret = 0;
  char args[64];
  return_value_if_fail(on_result, RET_BAD_PARAMS);
  tk_snprintf(args, sizeof(args)-1, "{\"a\":%d, \"b\":%d}", a, b);
  ret = platform_request_send("foobar", "add", args, on_result, on_result_ctx);

  return ret;
}
```

### 3.2 Java 语言实现插件

文件位置：src/foobar/android/java/org/zlgopen/plugins/foobar/FooBarPlugin.java

插件负责建立与服务之间的连接，并将 AWTK C API 发送过来的请求，转发给 FooBarServer。

建立连接的过程与 FooBarClient 类似，只是由于 Plugin 不是 Activity，如果要调用 Activity 的函数，需要通过 this.activity 调用。

```java
	public static IFooBarService mFooBarService;
    private ServiceConnection mServiceConn = new ServiceConnection() {
        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.e("client", "onServiceDisconnected");
            mFooBarService = null;
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.e("client", "onServiceConnected");
            mFooBarService = IFooBarService.Stub.asInterface(service);
        }
    };

    public void bindService() {
        Intent intent = new Intent("org.zlgopen.foobarserver.IFooBarService");
        intent.setPackage("org.zlgopen.foobarserver");

        this.activity.bindService(intent, mServiceConn, Context.BIND_AUTO_CREATE);
    }

    public void unbindService() {
        this.activity.unbindService(mServiceConn);
    }
```

run 函数负责处理来自 AWTK C API 的请求，并转发给 FooBarServer:

```java
  public boolean run(String action, String callerInfo, String args) {
    try {
      this.callerInfo = callerInfo;
      JSONObject json = new JSONObject(args);

      if (action.equals("add")) {
        int a = json.getInt("a");
        int b = json.getInt("b");
        this.Add(a, b);
      } else {
        Log.v("AWTK", "not supported action");
      }
    } catch (JSONException e) {
      Log.v("AWTK", e.toString());
      PluginManager.writeResult(this.callerInfo, "fail");
    }

    return true;
  }

  void Add(int a, int b) {
		int sum = 0;

		try {
				sum = mFooBarService.add(a, b);
				Log.w("client", Integer.toString(sum));
		}catch (RemoteException e) {
				Log.e("client", e.toString());
		}

    String result = "{\"action\":\"add\"" + ",\"result\":" + Integer.toString(sum) + "}";
    PluginManager.writeResult(this.callerInfo, result);

    return;
  }
```

### 3.3 AWTK C 语言调用示例

* 包含头文件

```
#include "foobar/foobar.h
```

* 调用函数并显示结果

```c
static ret_t foobar_on_result(void* ctx, const char* data){
  widget_t* result_label = WIDGET(ctx);
  conf_doc_t* doc = conf_doc_load_json(data, strlen(data));
  int result = conf_doc_get_int(doc, "result", 0);
  log_debug("foobar:%s\n result:%d\n", data, result);
  conf_doc_destroy(doc);

  widget_set_text_utf8(result_label, data);
  widget_invalidate_force(result_label, NULL);

  return RET_OK;
}

static ret_t on_click(void* ctx, event_t* e) {
  foobar_add(100, 200, foobar_on_result, ctx);
  return RET_OK;
}

```

完整代码请参考 [awtk-mobile-plugins](https://github.com/zlgopen/awtk-mobile-plugins)