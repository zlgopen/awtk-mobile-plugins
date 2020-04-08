package org.zlgopen.plugins;

import java.util.ArrayList;
import java.util.List;
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
import com.esafirm.imagepicker.features.ImagePicker;
import com.esafirm.imagepicker.features.ImagePickerComponentHolder;
import com.esafirm.imagepicker.features.ImagePickerConfig;
import com.esafirm.imagepicker.features.IpCons;
import com.esafirm.imagepicker.features.ReturnMode;
import com.esafirm.imagepicker.features.imageloader.DefaultImageLoader;
import com.esafirm.imagepicker.model.Image;
import com.esafirm.rximagepicker.RxImagePicker;

public class ImagePickerPlugin implements Plugin {
  private int id;
  private String action;
  private JSONObject args;
  private String callerInfo;
  private Activity activity;

  public static final int REQUEST_CODE_PICK = 0X01;

  @Override
  public void destroy() {
  }

  @Override
  public boolean matchRequest(int requestCode, int resultCode, Intent data) {
    Log.v("AWTK", "requestCode:" + Integer.toString(requestCode));
    int id = requestCode >> 16;
    if(data == null && id == this.id) {
      return true;
    }

    return ImagePicker.shouldHandle(requestCode, resultCode, data);
  }

  @Override
  public void onActivityResult(int requestCode, int resultCode, Intent data) {
    String result = "";
    List<Image> images = ImagePicker.getImages(data);

    for (int i = 0; i < images.size(); i++) {
      Image image = images.get(i);

      if (i > 0) {
        result += ":";
      }
      result += image.getPath();
    }

    Log.v("AWTK", result);
    PluginManager.writeResult(this.callerInfo, result);

    return;
  }

  @Override
  public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
    if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
      Log.v("AWTK", "onRequestPermissionsResult granted");
      this.pick();
    } else {
      Log.v("AWTK", "onRequestPermissionsResult deny");
      PluginManager.writeResult(this.callerInfo, "");
    }
  }

  @Override
  public boolean run(String action, String callerInfo, String args) {
    try {
      this.action = action;
      this.callerInfo = callerInfo;
      this.args = new JSONObject(args);

      if (action.equals("pick")) {
        if (ContextCompat.checkSelfPermission(this.activity,
            Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            int id = this.id << 16;
            ActivityCompat.requestPermissions(this.activity, new String[] { Manifest.permission.CAMERA },id);
        } else {
          this.pick();
        }
      }
    } catch (JSONException e) {
      Log.v("AWTK", e.toString());
    }

    return true;
  }

  ImagePickerPlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }

  void pick() {
    JSONObject json = this.args;
    Log.v("AWTK", "pick:" + json.toString());

    try {
      boolean single = json.getBoolean("single");
      boolean includeVideos = json.getBoolean("include_videos");

      if (single) {
        ImagePicker.create(this.activity).includeVideo(includeVideos).single().start();
      } else {
        ImagePicker.create(this.activity).includeVideo(includeVideos).multi().start();
      }
    } catch (JSONException e) {
      ImagePicker.create(this.activity).start();
      Log.v("AWTK", e.toString());
    }
  }
}
