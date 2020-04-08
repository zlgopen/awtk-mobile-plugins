package org.zlgopen.plugins;

import android.app.Activity;
import android.content.Intent;
import org.json.JSONObject;
import org.json.JSONException;
import android.util.Log;
import com.king.zxing.CaptureActivity;
import com.king.zxing.Intents;
import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.provider.MediaStore;
import android.content.Intent;
import androidx.core.content.ContextCompat;
import androidx.core.app.ActivityCompat;
import androidx.core.app.NotificationCompat;
import android.content.pm.PackageManager;

public class QrCodePlugin implements Plugin {
  private int id;
  private String title;
  private String callerInfo;
  private Activity activity;
  public static final String KEY_TITLE = "key_title";
  public static final String KEY_IS_QR_CODE = "key_code";
  public static final String KEY_IS_CONTINUOUS = "key_continuous_scan";

  public static final int REQUEST_CODE_SCAN = 0X01;
  public static final int REQUEST_CODE_PHOTO = 0X02;

  public static final int RC_CAMERA = 0X01;
  public static final int RC_READ_PHOTO = 0X02;

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
    int code = requestCode & 0xffff;
    if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
      Log.v("AWTK", "onRequestPermissionsResult granted");
      if (code == REQUEST_CODE_SCAN) {
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
    Log.v("AWTK", code + "(code)vs: " + REQUEST_CODE_SCAN);
    if (code == REQUEST_CODE_SCAN) {
      String result = data.getStringExtra(Intents.Scan.RESULT);
      PluginManager.writeResult(this.callerInfo, result);
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
        if (ContextCompat.checkSelfPermission(this.activity,
            Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
          int code = (this.id << 16) | REQUEST_CODE_SCAN;
          ActivityCompat.requestPermissions(this.activity, new String[] { Manifest.permission.CAMERA }, code);
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

  QrCodePlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }

  void scan() {
    String title = this.title;
    int code = (this.id << 16) | REQUEST_CODE_SCAN;
    Intent intent = new Intent(this.activity, CaptureActivity.class);
    intent.putExtra(KEY_TITLE, title);
    intent.putExtra(KEY_IS_CONTINUOUS, true);

    this.activity.startActivityForResult(intent, code);
  }
}
