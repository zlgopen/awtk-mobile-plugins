package org.zlgopen.plugins;

import android.app.Activity;
import android.content.Intent;
import org.json.JSONObject;
import org.json.JSONException;
import android.util.Log;

public class SharePlugin implements Plugin {
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
      String text = json.getString("text");
      String subject = json.getString("subject");

      this.share(text, subject);
    } catch (JSONException e) {
      Log.v("AWTK", e.toString());
    }

    PluginManager.writeResult(callerInfo, "share done");

    return true;
  }

  SharePlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
  }

  void share(String text, String subject) {
    if (text == null || text.isEmpty()) {
      throw new IllegalArgumentException("Non-empty text expected");
    }

    Intent shareIntent = new Intent();
    shareIntent.setAction(Intent.ACTION_SEND);
    shareIntent.putExtra(Intent.EXTRA_TEXT, text);
    shareIntent.putExtra(Intent.EXTRA_SUBJECT, subject);
    shareIntent.setType("text/plain");
    Intent chooserIntent = Intent.createChooser(shareIntent, null /* dialog title optional */);
    if (activity != null) {
      activity.startActivity(chooserIntent);
    } else {
      chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
      activity.startActivity(chooserIntent);
    }
  }
}
