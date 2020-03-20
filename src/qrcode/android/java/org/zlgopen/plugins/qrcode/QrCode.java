// Copyright 2019 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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

public class QrCode implements Plugin {
  private int id;
  private Activity activity;
  public static final String KEY_TITLE = "key_title";
  public static final String KEY_IS_QR_CODE = "key_code";
  public static final String KEY_IS_CONTINUOUS = "key_continuous_scan";

  public static final int REQUEST_CODE_SCAN = 0X01;
  public static final int REQUEST_CODE_PHOTO = 0X02;

  public static final int RC_CAMERA = 0X01;
  public static final int RC_READ_PHOTO = 0X02;

  @Override
  public void deinit() {
  }

  @Override
  public boolean matchRequest(int requestCode) {
    return false;
  }
  
  @Override
  public void onActivityResult(int requestCode, int resultCode, Intent data) {
    return;
  }

  @Override
  public String run(String action, String args) {
    try {
      JSONObject json = new JSONObject(args);
      String title = json.getString("title");
      
      if(action.equals("scan")) {
        this.scan(title);
      }

    } catch(JSONException e) {
      Log.v("AWTK", e.toString());
    }
    return action;
  }

  QrCode(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }

  void scan(String title) {
    int code = this.id << 16 | REQUEST_CODE_SCAN;
    Intent intent = new Intent(this.activity, CaptureActivity.class);
    intent.putExtra(KEY_TITLE,title);
    intent.putExtra(KEY_IS_CONTINUOUS, true);

    this.activity.startActivityForResult(intent, code);
  }
}
