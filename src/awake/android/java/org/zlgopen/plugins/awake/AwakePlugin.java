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

import android.widget.Toast;

public class AwakePlugin implements Plugin {
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

      Log.v("AWTK", "Awake action: " + action);
      if (action.equals("keep_screen_on")) {
        window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
      } else if (action.equals("not_keep_screen_on")) {
        window.clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
      } else if (action.equals("turn_screen_on")) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
          this.activity.setTurnScreenOn(true);
          Log.v("AWTK", "this.activity.setTurnScreenOn(true)");
        } else {
          window.addFlags(WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON);
          Log.v("AWTK", "WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON");
        } 
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

  AwakePlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }

}
