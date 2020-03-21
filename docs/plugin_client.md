# 插件客户端

插件客户端的实现非常简单，就是对参数进行打包，再通过 platform request 把请求发送给服务端。

## 插件客户端的 API

插件客户端的 API 一定要体现该插件的功能。

* 对于不需返回值的插件，其定义和普通函数是一样的。如分享插件的 API：

```c
/**
 * @method share_request
 * 请求分享数据。
 *
 * @annotation ["static"]
 * @param {const char*} text text。
 * @param {const char*} subject subject。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t share_request(const char* text, const char* subject);
```

* 对于需返回值的插件，则要提供一个回调函数及上下文，在执行完成后会调用该回调函数。

如扫二维码的插件的 API：

```c
/**
 * @method qrcode_scan
 * 请求扫描二维码。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {const char*} title 标题。
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t qrcode_scan(const char *title, platform_request_on_result_t on_result, void *on_result_ctx);

```

## 参数打包

参数一般采用 JSON 格式，键值需要和插件服务端保持一致。

如分享插件：

* 客户端

```c
ret_t share_request(const char* text, const char* subject) {
  str_t str;
  ret_t ret = 0;
  return_value_if_fail(text != NULL && subject != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_init(&str, strlen(text) + strlen(subject) + 50) != NULL, RET_OOM);

  str_append_char(&str, '{');
  str_append_json_str_pair(&str, "text", text);
  str_append_char(&str, ',');
  str_append_json_str_pair(&str, "subject", subject);
  str_append_char(&str, '}');

  ret = platform_request_send("share", "share", str.str, share_on_result, NULL);

  str_reset(&str);

  return ret;
}
```

* 服务端 (android)

```java
  @Override
  public boolean run(String action, String callerInfo, String args) {
    try {
      JSONObject json = new JSONObject(args);
      String text = json.getString("text");
      String subject = json.getString("subject");

      this.share(text, subject);
    } catch(JSONException e) {
      Log.v("AWTK", e.toString());
    }

    PluginManager.writeResult(callerInfo, "share done");

    return true;
  }
```