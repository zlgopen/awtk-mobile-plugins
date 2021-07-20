package org.zlgopen.plugins;

import android.app.Activity;
import org.json.JSONObject;
import org.json.JSONException;
import android.util.Log;
import android.Manifest;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;
import android.os.Build.VERSION;
import android.os.Build.VERSION_CODES;
import android.view.WindowManager;
import android.os.Build;
import android.view.Window;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.widget.Toast;

public class WakeLockPlugin implements Plugin {
  private int id;
  private String callerInfo;
  private Activity activity;

  @Override
  public void destroy() {
  }

  @Override
  public boolean matchRequest(int requestCode, int resultCode, Intent data) {
    return false;
  }

  @Override
  public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
  }

  @Override
  public void onActivityResult(int requestCode, int resultCode, Intent data) {
    return;
  }

  @Override
  public boolean run(String action, String callerInfo, String args) {
    try {
      this.callerInfo = callerInfo;
      JSONObject json = new JSONObject(args);
      final Window window = this.activity.getWindow();

      Log.v("AWTK", "WakeLock action: " + action);
      if (action.equals("require")) {
        this.activity.startService(new Intent(this.activity, 
          org.zlgopen.plugins.wake_lock.WakeLockService.class));
      } else if (action.equals("release")) {
        this.activity.stopService(new Intent(this.activity, 
          org.zlgopen.plugins.wake_lock.WakeLockService.class));
      } else {
        Log.v("AWTK", "not supported action");
      }
      PluginManager.writeSuccess(this.callerInfo, action);
    } catch (JSONException e) {
      Log.v("AWTK", e.toString());
      PluginManager.writeFailure(this.callerInfo, "unknown", e.toString());
    }

    return true;
  }

  WakeLockPlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }
}
