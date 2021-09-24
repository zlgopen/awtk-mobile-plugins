package org.zlgopen.plugins;

import android.app.Activity;
import android.content.Intent;

import org.json.JSONObject;
import org.json.JSONException;

import java.io.File;
import android.net.Uri;
import android.util.Log;

public class MiscPlugin implements Plugin {
    private int id;
    private Activity activity;
    private String callerInfo;

    @Override
    public void destroy() {
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
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
    }

    @Override
    public boolean run(String action, String callerInfo, String args) {
        try {
            JSONObject json = new JSONObject(args);
            String funcName = json.getString("name");
            String funcArgs = json.getString("args");
            this.callerInfo = callerInfo;

            this.call(funcName, funcArgs);
        } catch (JSONException e) {
            Log.v("AWTK", e.toString());
            PluginManager.writeResult(callerInfo, e.toString());
        }

        return true;
    }

    MiscPlugin(Activity activity, int id) {
        this.id = id;
        this.activity = activity;
    }

    public void notifySystemToScan(String filePath) {
        Intent intent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
        File file = new File(filePath);
        Uri uri = Uri.fromFile(file);

        intent.setData(uri);
        this.activity.sendBroadcast(intent);
    }

    void call(String name, String args) {
      if(name.equals("notifySystemToScan")) {
        this.notifySystemToScan(args);
        PluginManager.writeResult(this.callerInfo, "ok");
      } else {
        Log.v("AWTK", "Not supported function " + name);
        PluginManager.writeResult(this.callerInfo, "fail");
      }
    }
}
