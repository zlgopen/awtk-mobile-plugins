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
import android.os.Build.VERSION;
import android.os.Build.VERSION_CODES;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteException;
import android.widget.Toast;

import org.zlgopen.foobarserver.IFooBarService;

public class FooBarPlugin implements Plugin {
  private int id;
  private String callerInfo;
  private Activity activity;
  private BroadcastReceiver receiver;

  @Override
  public void destroy() {
    this.unbindService();
  }

  @Override
  public boolean matchRequest(int requestCode, int resultCode, Intent data) {
    return false;
  }

  @Override
  public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
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

      if (action.equals("add")) {
        int a = json.getInt("a");
        int b = json.getInt("b");
        this.Add(a, b);
      } else {
        Log.v("AWTK", "not supported action");
      }
    } catch (JSONException e) {
      Log.v("AWTK", e.toString());
      PluginManager.writeResult(this.callerInfo, "fail");
    }

    return true;
  }

  FooBarPlugin(Activity activity, int id) {
    this.id = id;
    this.activity = activity;
		this.bindService();
  }

  void Add(int a, int b) {
		int sum = 0;

		try {
				sum = mFooBarService.add(a, b);
				Log.w("client", Integer.toString(sum));
		}catch (RemoteException e) {
				Log.e("client", e.toString());
		}

    String result = "{\"action\":\"add\"" + ",\"result\":" + Integer.toString(sum) + "}";
    PluginManager.writeResult(this.callerInfo, result);

    return;
  }

	public static IFooBarService mFooBarService;
    private ServiceConnection mServiceConn = new ServiceConnection() {
        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.e("client", "onServiceDisconnected");
            mFooBarService = null;
        }


        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.e("client", "onServiceConnected");
            mFooBarService = IFooBarService.Stub.asInterface(service);
        }
    };


    public void bindService() {
        Intent intent = new Intent("org.zlgopen.foobarserver.IFooBarService");
        intent.setPackage("org.zlgopen.foobarserver");

        this.activity.bindService(intent, mServiceConn, Context.BIND_AUTO_CREATE);
    }

    public void unbindService() {
        this.activity.unbindService(mServiceConn);
    }
}
