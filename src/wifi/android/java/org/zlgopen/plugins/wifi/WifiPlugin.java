package org.zlgopen.plugins;

import java.util.List;
import android.app.Activity;
import android.content.Intent;
import org.json.JSONObject;
import org.json.JSONException;
import android.util.Log;
import android.Manifest;
import android.content.Context;
import android.provider.MediaStore;
import android.net.wifi.WifiManager;
import android.content.BroadcastReceiver;
import android.net.wifi.ScanResult;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.wifi.WifiInfo;
import android.net.NetworkInfo;

public class WifiPlugin implements Plugin {
  private int id;
  private String callerInfo;
  private Activity activity;


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
  public void onActivityResult(int requestCode, int resultCode, Intent data) {
    return;
  }

  @Override
  public boolean run(String action, String callerInfo, String args) {
    try {
      this.callerInfo = callerInfo;
      JSONObject json = new JSONObject(args);

      if (action.equals("scan")) {
        this.scan(callerInfo);
      } else if (action.equals("get_info")) {
        this.getInfo(callerInfo);
      }

    } catch (JSONException e) {
      Log.v("AWTK", e.toString());
      PluginManager.writeResult(this.callerInfo, "{}");
    }

    return true;
  }

  WifiPlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }

  void scan(final String callerInfo) {
    Activity activity = this.activity;
    WifiManager wifiManager = (WifiManager)activity.getSystemService(Context.WIFI_SERVICE);

    BroadcastReceiver wifiScanReceiver = new BroadcastReceiver() {
      @Override
      public void onReceive(Context c, Intent intent) {
        boolean success = intent.getBooleanExtra(
                           WifiManager.EXTRA_RESULTS_UPDATED, false);
        if (success) {
          Log.v("AWTK", "wifi scan success");
        } else {
          Log.v("AWTK", "wifi scan fail");
        }
        
        String result = "";
        List<ScanResult> items = wifiManager.getScanResults();

        for (int i = 0; i < items.size(); i++) {
          ScanResult iter = items.get(i);

          result +=  "{\n";
          result += String.format("\"ssid\":\"%s\",\n", iter.SSID);
          result += String.format("\"bssid\":\"%s\",\n", iter.BSSID);
          result += String.format("\"capabilities\":\"%s\",\n", iter.capabilities);
          result += String.format("\"frequency\":\"%d\"\n", iter.frequency);
          result +=  "}\n";
        }

        PluginManager.writeResult(callerInfo, result);
        activity.unregisterReceiver(this);
      }
    };

    IntentFilter intentFilter = new IntentFilter();
    intentFilter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
    this.activity.registerReceiver(wifiScanReceiver, intentFilter);
    boolean success = wifiManager.startScan();
    if (!success) {
      PluginManager.writeResult(callerInfo, "{}");
    }
  }
 
  private String getNetWorkInfo() {
    String str = "{}";
    Activity activity = this.activity;

    ConnectivityManager connManager = (ConnectivityManager) activity.getSystemService(Context.CONNECTIVITY_SERVICE);
    NetworkInfo networkInfo = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
    if (networkInfo.isConnected()) {
      final WifiManager wifiManager = (WifiManager) activity.getSystemService(Context.WIFI_SERVICE);
      final WifiInfo connectionInfo = wifiManager.getConnectionInfo();
      str = "{\n";
      str += "\"connected\":true,\n";
      if (connectionInfo != null) {
        str += String.format("\"ssid\":\"%s\",\n", connectionInfo.getSSID());
        str += String.format("\"bssid\":\"%s\",\n", connectionInfo.getBSSID());
        str += String.format("\"mac\":\"%s\"\n,", connectionInfo.getMacAddress());
        str += String.format("\"ip\":\"%d\"\n", connectionInfo.getIpAddress());
      }
      str += "}";
    }

    return str;
}

  void getInfo(final String callerInfo) {
     PluginManager.writeResult(callerInfo, this.getNetWorkInfo());
  }
}
