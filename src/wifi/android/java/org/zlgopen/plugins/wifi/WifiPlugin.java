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
import androidx.core.content.ContextCompat;
import androidx.core.app.ActivityCompat;
import androidx.core.app.NotificationCompat;
import android.content.pm.PackageManager;

public class WifiPlugin implements Plugin {
  private int id;
  private String callerInfo;
  private Activity activity;
  private static int SCAN_CODE = 1;
  private static int GET_INFO_CODE = 2;

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
      if (code == SCAN_CODE) {
        this.scan();
      } else if (code == GET_INFO_CODE) {
        this.getInfo();
      }
    } else {
      Log.v("AWTK", "onRequestPermissionsResult deny");
      PluginManager.writeResult(this.callerInfo, "{}");
    }
  }

  @Override
  public void onActivityResult(int requestCode, int resultCode, Intent data) {
    return;
  }

  public boolean hasPermissions() {
    if (ContextCompat.checkSelfPermission(this.activity,
        Manifest.permission.CHANGE_WIFI_STATE) != PackageManager.PERMISSION_GRANTED) {
      return false;
    }
    if (ContextCompat.checkSelfPermission(this.activity,
        Manifest.permission.ACCESS_WIFI_STATE) != PackageManager.PERMISSION_GRANTED) {
      return false;
    }
    if (ContextCompat.checkSelfPermission(this.activity,
        Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
      return false;
    }
    if (ContextCompat.checkSelfPermission(this.activity,
        Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
      return false;
    }

    return true;
  }

  @Override
  public boolean run(String action, String callerInfo, String args) {
    try {
      this.callerInfo = callerInfo;
      JSONObject json = new JSONObject(args);

      if (!this.hasPermissions()) {
        int code = 0;
        if (action.equals("scan")) {
          code = (this.id << 16) | SCAN_CODE;
        } else if (action.equals("get_info")) {
          code = (this.id << 16) | GET_INFO_CODE;
        }

        ActivityCompat.requestPermissions(this.activity,
            new String[] { Manifest.permission.CHANGE_WIFI_STATE, Manifest.permission.ACCESS_WIFI_STATE,
                Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION },
            code);
      } else {
        if (action.equals("scan")) {
          this.scan();
        } else if (action.equals("get_info")) {
          this.getInfo();
        }
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

  void scan() {
    Activity activity = this.activity;
    String callerInfo = this.callerInfo;
    WifiManager wifiManager = (WifiManager) activity.getSystemService(Context.WIFI_SERVICE);

    BroadcastReceiver wifiScanReceiver = new BroadcastReceiver() {
      @Override
      public void onReceive(Context c, Intent intent) {
        boolean success = intent.getBooleanExtra(WifiManager.EXTRA_RESULTS_UPDATED, false);
        if (success) {
          Log.v("AWTK", "wifi scan success");
        } else {
          Log.v("AWTK", "wifi scan fail");
        }

        String result = "";
        List<ScanResult> items = wifiManager.getScanResults();

        for (int i = 0; i < items.size(); i++) {
          ScanResult iter = items.get(i);
          result += "{\n";
          result += String.format("\"ssid\":\"%s\",\n", iter.SSID);
          result += String.format("\"level\":\"%s\",\n", iter.level);
          result += String.format("\"bssid\":\"%s\",\n", iter.BSSID);
          result += String.format("\"capabilities\":\"%s\",\n", iter.capabilities);
          result += String.format("\"frequency\":%d\n", iter.frequency);
          if ((i + 1) < items.size()) {
            result += "},\n";
          } else {
            result += "}\n";
          }
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
        String ssid = connectionInfo.getSSID().replaceAll("\"", "");
        str += String.format("\"ssid\":\"%s\",\n", ssid);
        str += String.format("\"bssid\":\"%s\",\n", connectionInfo.getBSSID());
        str += String.format("\"mac\":\"%s\",\n", connectionInfo.getMacAddress());
        str += String.format("\"ip\":%d,\n", connectionInfo.getIpAddress());
        str += String.format("\"freq\":%d,\n", connectionInfo.getFrequency());
        str += String.format("\"network_id\":%d,\n", connectionInfo.getNetworkId());
        str += String.format("\"link_speed\":%d,\n", connectionInfo.getLinkSpeed());
        str += String.format("\"Sign\":%d\n", connectionInfo.getRssi());
      }
      str += "}";
    }

    return str;
  }

  void getInfo() {
    String callerInfo = this.callerInfo;
    PluginManager.writeResult(callerInfo, this.getNetWorkInfo());
  }
}
