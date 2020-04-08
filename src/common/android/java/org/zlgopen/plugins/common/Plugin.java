package org.zlgopen.plugins;

import java.lang.*;
import android.content.Intent;

/**
 * 插件接口。
 * 
 * @version 1.0
 */
public interface Plugin {
  /**
   * 销毁插件。
   * 
   * @return 无。
   */
  public void destroy();

  /**
   * 匹配指定的请求码是否由本插件发出。
   * 
   * @param requestCode 请求码。
   * @param resultCode  结果码.
   * @param data        返回的数据。
   * 
   * @return 请求码是否由本插件发出。
   */
  public boolean matchRequest(int requestCode, int resultCode, Intent data);

  public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults);

  /**
   * 执行指定的action。
   * 
   * @param action     动作名称。
   * @param callerInfo 调用者的信息，用于返回结果。
   * @param args       参数。一般用json格式。
   * 
   * @return 是否执行成功。
   */
  public boolean run(String action, String callerInfo, String args);

  /**
   * 当请求是由另外一个Activity执行时，执行完成后调用本函数返回执行结果。
   * 
   * @param requestCode 请求码.
   * @param resultCode  结果码.
   * @param data        返回的数据。
   * 
   * @return 无。
   */
  public void onActivityResult(int requestCode, int resultCode, Intent data);
}
