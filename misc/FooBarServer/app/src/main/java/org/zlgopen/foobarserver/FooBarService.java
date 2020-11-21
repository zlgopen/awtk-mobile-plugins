package org.zlgopen.foobarserver;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class FooBarService extends Service {
    public FooBarService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        return binder;
    }

    private final IFooBarService.Stub binder = new IFooBarService.Stub() {
        public int add(int a, int b) {
            return a + b;
        }
    };
}