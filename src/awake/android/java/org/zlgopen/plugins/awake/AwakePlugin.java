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

      Log.v("AWTK", "Awake action: " + action);
      if (action.equals("enable")) {
        this.activity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
      }else if (action.equals("disable")) {
        this.activity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
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
