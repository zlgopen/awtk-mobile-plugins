// Copyright 2019 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.zlgopen.plugins;

import android.app.Activity;
import org.json.JSONObject;
import org.json.JSONException;
import android.util.Log;
import android.Manifest;
import android.provider.MediaStore;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;
import android.os.Build.VERSION;
import android.os.Build.VERSION_CODES;

public class BatteryPlugin implements Plugin {
  private int id;
  private String callerInfo;
  private Activity activity;
  private BroadcastReceiver receiver;

  private void unregisterReceiver() {
    if(this.receiver != null) {
      this.activity.unregisterReceiver(this.receiver);
      this.receiver = null;
    }
  }

  @Override
  public void destroy() {
    this.unregisterReceiver();
  }

  @Override
  public boolean matchRequest(int requestCode, int resultCode, Intent data) {
    return false;
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
      
      if(action.equals("get_info")) {
        this.getInfo();
      } else if(action.equals("register")) {
        this.register(json);
      } else if(action.equals("unregister")) {
        this.unregister();
      } else {
        Log.v("AWTK", "not supported action");
      }
    } catch(JSONException e) {
      Log.v("AWTK", e.toString());
      PluginManager.writeResult(this.callerInfo, "fail");
    }

    return true;
  }

  BatteryPlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }

  void getInfo() {
    IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
    Intent batteryStatus = this.activity.registerReceiver(null, ifilter);

    int level = batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
    int scale = batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1);

    String result = "{\"level\":" + Integer.toString(level) + 
      ",\"scale\":" + Integer.toString(level) + "}";
    PluginManager.writeResult(this.callerInfo, result);

    return;
  }
  
  void register(JSONObject json) {
      try {
        final String onEvent = json.getString("onevent");
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BatteryManager.ACTION_CHARGING);
        intentFilter.addAction(BatteryManager.ACTION_DISCHARGING);
        intentFilter.addAction(Intent.ACTION_BATTERY_CHANGED);

         receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
              int currentBattery = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, 0);
              int totalBattery = intent.getIntExtra(BatteryManager.EXTRA_SCALE, 0);
             
              PluginManager.writeResult(onEvent, intent.getAction());

              Log.v("AWTK", intent.getAction());
            }
         };
         
         this.activity.registerReceiver(receiver, intentFilter);
         
         PluginManager.writeResult(this.callerInfo, "ok");
      } catch(JSONException e) {
        Log.v("AWTK", e.toString());
        PluginManager.writeResult(this.callerInfo, "fail");
        return;
      }
  }

  void unregister() {
    this.unregisterReceiver();
  }
}