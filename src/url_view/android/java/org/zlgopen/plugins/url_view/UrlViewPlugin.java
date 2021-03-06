package org.zlgopen.plugins;

import android.app.Activity;
import android.content.Intent;

import org.json.JSONObject;
import org.json.JSONException;

import android.net.Uri;
import android.util.Log;

public class UrlViewPlugin implements Plugin {
    private int id;
    private Activity activity;

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
            String url = json.getString("url");
            String mimeType = json.getString("mime_type");

            this.view(url, mimeType);
        } catch (JSONException e) {
            Log.v("AWTK", e.toString());
        }

        PluginManager.writeResult(callerInfo, "url_view done");

        return true;
    }

    UrlViewPlugin(Activity activity, int id) {
        this.id = id;
        this.activity = activity;
    }

    void view(String url, String mimeType) {
        if (url == null || url.isEmpty()) {
            throw new IllegalArgumentException("Non-empty url expected");
        }

        if (!url.startsWith("file:") && !url.startsWith("mailto") && !url.startsWith("tel:") 
          && !url.contains("://")) {
            url = "file://" + url;
        }

        Intent aintent = new Intent();
        aintent.setAction(android.content.Intent.ACTION_VIEW);

        Uri aurl = Uri.parse(url);
        if (url.startsWith("file://")) {
            aintent.setDataAndType(aurl, mimeType);
            Intent chooserIntent = Intent.createChooser(aintent, null /* dialog title optional */);
            if (activity != null) {
                activity.startActivity(chooserIntent);
            } else {
                chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                activity.startActivity(chooserIntent);
            }
        } else {
            aintent.setData(aurl);
            activity.startActivity(aintent);
        }

    }
}
