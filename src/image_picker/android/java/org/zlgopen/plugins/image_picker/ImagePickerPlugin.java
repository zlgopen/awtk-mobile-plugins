// Copyright 2019 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
  private String callerInfo;
  private Activity activity;

  public static final int REQUEST_CODE_PICK = 0X01;

  @Override
  public void destroy() {
  }

  @Override
  public boolean matchRequest(int requestCode, int resultCode, Intent data) {
    return ImagePicker.shouldHandle(requestCode, resultCode, data);
  }
  
  @Override
  public void onActivityResult(int requestCode, int resultCode, Intent data) {
    List<Image> images = ImagePicker.getImages(data);
    Image image = ImagePicker.getFirstImageOrNull(data);
    String info = String.format("{\"name\":\"%s\", \"path\":\"%s\"}",image.getName(), image.getPath());
    
    PluginManager.writeResult(this.callerInfo, info);
    Log.v("AWTK", info);

    return;
  }

  @Override
  public boolean run(String action, String callerInfo, String args) {
    try {
      this.callerInfo = callerInfo;
      JSONObject json = new JSONObject(args);
      
      if(action.equals("pick")) {
        this.pick(json);
      }

    } catch(JSONException e) {
      Log.v("AWTK", e.toString());
    }

    return true;
  }

  ImagePickerPlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }

  void pick(JSONObject json) {
    try {
      boolean single = json.getBoolean("single");
      boolean cameraOnly = json.getBoolean("camera_only");
      boolean showCamera = json.getBoolean("show_camera");

      ImagePicker.create(this.activity).start();
    }catch(JSONException e) {
      ImagePicker.create(this.activity).start();
      Log.v("AWTK", e.toString());
    }
  }
}
