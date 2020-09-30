package org.zlgopen.plugins;

import android.app.Activity;
import android.content.Intent;
import org.json.JSONObject;
import org.json.JSONException;
import android.util.Log;
import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.provider.MediaStore;
import android.content.Intent;
import androidx.core.content.ContextCompat;
import androidx.core.app.ActivityCompat;
import androidx.core.app.NotificationCompat;
import android.content.pm.PackageManager;

public class LPRPlugin implements Plugin {
  private int id;
  private String title;
  private String callerInfo;
  private Activity activity;
  public static final int REQUEST_LPR_CODE = 1001;
  public static final String LPRActivity = "com.pcl.ocr.ui.LPRActivity";

  private final String[] myPermissions = new String[]{Manifest.permission.CAMERA,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE};

  private boolean checkSelfPermissions() {
    for (int i = 0; i < this.myPermissions.length; i++) {
        if (ContextCompat.checkSelfPermission(this.activity, this.myPermissions[i]) != PackageManager.PERMISSION_GRANTED) {
          return false;
        }
    }
    return true;
  }
  
  private boolean requestSelfPermissions() {
     int code = (this.id << 16) | REQUEST_LPR_CODE;
     ActivityCompat.requestPermissions(this.activity, this.myPermissions, code);

     return true;
  }

  @Override
  public void destroy() {
  }

  @Override
  public boolean matchRequest(int requestCode, int resultCode, Intent data) {
    int id = requestCode >> 16;
    Log.v("AWTK", id + "(id)vs " + this.id);

    return this.id == id;
  }

  @Override
  public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
    boolean allGranted = false;
    int code = requestCode & 0xffff;
    if(permissions.length == this.myPermissions.length && grantResults.length == this.myPermissions.length) {
      allGranted = true;
      for (int i = 0; i < permissions.length; i++) {
        if(grantResults[i] != PackageManager.PERMISSION_GRANTED) {
          allGranted = false;
          break;
        }
      }
    }

    if (allGranted) {
      Log.v("AWTK", "onRequestPermissionsResult granted");
      if (code == REQUEST_LPR_CODE) {
        this.scan();
      }
    } else {
      Log.v("AWTK", "onRequestPermissionsResult deny");
      PluginManager.writeResult(this.callerInfo, "");
    }
  }

  @Override
  public void onActivityResult(int requestCode, int resultCode, Intent data) {
    int code = requestCode & 0xffff;
		Log.v("AWTK", code + "(code)vs: " + REQUEST_LPR_CODE);
		if (code == REQUEST_LPR_CODE) {
		    if(resultCode == Activity.RESULT_OK) {
          if (data != null) {
              String result = data.getStringExtra("card");
              PluginManager.writeResult(this.callerInfo, result);
              Log.v("AWTK", "result:" + result);
          } else {
            Log.v("AWTK", "request canceled(no data)");
            PluginManager.writeResult(this.callerInfo, "");
          }
        } else {
          Log.v("AWTK", "request canceled(resultCode not ok)");
          PluginManager.writeResult(this.callerInfo, "");
        }
		}

    return;
  }

  @Override
  public boolean run(String action, String callerInfo, String args) {
    try {
      this.callerInfo = callerInfo;
      JSONObject json = new JSONObject(args);
      this.title = json.getString("title");

      if (action.equals("scan")) {
        if (!this.checkSelfPermissions()) {
          this.requestSelfPermissions();
        } else {
          this.scan();
        }
      }

    } catch (JSONException e) {
      Log.v("AWTK", e.toString());
      PluginManager.writeResult(this.callerInfo, "fail");
    }

    return true;
  }

  LPRPlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }

  void scan() {
    int code = (this.id << 16) | REQUEST_LPR_CODE;
    Intent intent = new Intent();
    intent.setClassName(this.activity, LPRActivity);
    this.activity.startActivityForResult(intent, code);
  }
}
