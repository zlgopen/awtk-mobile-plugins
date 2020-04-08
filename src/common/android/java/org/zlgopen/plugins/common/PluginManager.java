package org.zlgopen.plugins;

import java.util.Set;
import java.util.Map;
import android.util.Log;
import java.util.HashMap;
import java.util.Iterator;
import android.app.Activity;
import android.content.Intent;

EXTRA_IMPORTS

public class PluginManager {
  static Activity activity;
  static boolean quited = false;
  static HashMap<String, Plugin> plugins = new HashMap<>();

  public static void runInUIThread(final String callerInfo, final String target, final String action,
      final String args) {
    PluginManager.activity.runOnUiThread(new Runnable() {
      @Override
      public void run() {
        PluginManager.run(target, callerInfo, action, args);
        Log.v("AWTK", "runInUIThread: callerInfo=" + callerInfo);
        Log.v("AWTK", "runInUIThread: args=" + args);
      }
    });
  }

  public static void startThread() {
    new Thread(new Runnable() {
      @Override
      public void run() {
        while (!(PluginManager.quited)) {
          String request = PluginManager.readRequest();

          if (request == null) {
            continue;
          }

          String items[] = request.split(":", 5);

          if (items.length == 5) {
            final String onResult = items[0];
            final String onResultCtx = items[1];
            final String target = items[2];
            final String action = items[3];
            final String args = items[4];
            final String callerInfo = onResult + ":" + onResultCtx + ":" + target + ":" + action;
            PluginManager.runInUIThread(callerInfo, target, action, args);
          } else {
            Log.v("AWTK", "invalid request:" + request);
          }
        }
        Log.v("AWTK", "plugin thread quited");
      }
    }).start();
  }

  public static void start(Activity activity) {
    PluginManager.quited = false;
    PluginManager.activity = activity;

    PluginManager.init();
    PluginManager.registerAll();
    PluginManager.startThread();
    Log.v("AWTK", "PluginManager start");
  }

  public static void stop() {
    Set set = PluginManager.plugins.entrySet();
    Iterator iterator = set.iterator();

    PluginManager.quited = true;
    while (iterator.hasNext()) {
      Map.Entry iter = (Map.Entry) iterator.next();
      Plugin plugin = (Plugin) (iter.getValue());
      plugin.destroy();
    }

    PluginManager.deinit();
    Log.v("AWTK", "PluginManager stop");
  }

  public static void onActivityResult(int requestCode, int resultCode, Intent data) {
    Set set = PluginManager.plugins.entrySet();
    Iterator iterator = set.iterator();

    while (iterator.hasNext()) {
      Map.Entry iter = (Map.Entry) iterator.next();
      Plugin plugin = (Plugin) (iter.getValue());
      if (plugin.matchRequest(requestCode, resultCode, data)) {
        plugin.onActivityResult(requestCode, resultCode, data);
      }
    }

    return;
  }

  public static void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
    Set set = PluginManager.plugins.entrySet();
    Iterator iterator = set.iterator();

    while (iterator.hasNext()) {
      Map.Entry iter = (Map.Entry) iterator.next();
      Plugin plugin = (Plugin) (iter.getValue());
      if (plugin.matchRequest(requestCode, 0, null)) {
        plugin.onRequestPermissionsResult(requestCode, permissions, grantResults);
      }
    }
  }

  public static boolean run(String name, String callerInfo, String action, String args) {
    Plugin plugin = PluginManager.plugins.get(name);

    if (plugin != null) {
      return plugin.run(action, callerInfo, args);
    }

    return false;
  }

  public static void register(String name, Plugin plugin) {
    PluginManager.plugins.put(name, plugin);
  }

  public static void unregister(String name) {
    PluginManager.plugins.remove(name);
  }

  public static void registerAll() {
    int id = 100;
    EXTRA_REGISTERS
  }

  public static native void init();

  public static native void deinit();

  public static native String readRequest();

  public static native void writeResult(String callerInfo, String result);
}
