package org.zlgopen.plugins.wake_lock;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.provider.Settings;
import android.util.Log;
import android.os.PowerManager;
import android.content.Context;
import android.os.PowerManager.WakeLock;

public class WakeLockService extends Service {
    private WakeLock wakeLock = null;

    @Override
    public IBinder onBind(Intent intent) {
        return  null;
    }
    @Override
    public void onCreate() {
        Log.v("AWTK", "WakeLockService onCreate");
    }
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        PowerManager powerManager = (PowerManager) this.getSystemService(Context.POWER_SERVICE);
        this.wakeLock = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "AWTK Wake Lock");
        this.wakeLock.acquire();

        Log.v("AWTK", "WakeLockService require");
        Log.v("AWTK", "WakeLockService onStartCommand");
        return START_STICKY;
    }
    @Override
    public void onDestroy() {
        this.wakeLock.release();
        this.wakeLock = null;
        Log.v("AWTK", "WakeLockService release");
        Log.v("AWTK", "WakeLockService onDestroy");
        super.onDestroy();
    }
}

