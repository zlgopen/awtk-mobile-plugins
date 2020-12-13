package org.zlgopen.plugins;
//https://www.jianshu.com/p/3a372af38103
//https://developer.android.com/reference/android/bluetooth/BluetoothGatt

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Handler;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;
import java.util.ArrayList;

public class BLEPlugin implements Plugin {
    private int id;
    private Activity activity;
    private String callerInfo;
    private String notifyReceiver;

    private static final int START_CODE = 1;
    private static final int START_SCAN_CODE = 2;
    private static final int STOP_SCAN_CODE = 3;
    private static final int GET_INFO_CODE = 4;
    private static final long SCAN_PERIOD = 10000;

    private Handler mHandler;
    private boolean mScanning;
    private boolean mStarted;
    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothLeScanner mBluetoothLeScanner;
    private List<BluetoothDevice> mBluetoothDevices;
    private List<BluetoothGatt> mBluetoothGatts;

    private ScanCallback scanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            BluetoothDevice device = result.getDevice();
            int rssi = result.getRssi();
            String addr = device.getAddress();
            if (rssi >= -70) {
                if (findDeviceByAddr(addr) == null) {
                    mBluetoothDevices.add(device);
                }
            } else {
                Log.v("AWTK", "skip " + addr);
            }
        }

        @Override
        public void onBatchScanResults(List<ScanResult> results) {
            super.onBatchScanResults(results);
        }

        @Override
        public void onScanFailed(int errorCode) {
            super.onScanFailed(errorCode);
        }
    };

    private BluetoothGattCallback mBluetoothGattCallback = new android.bluetooth.BluetoothGattCallback() {

        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status,
                                            int newState) {
            BluetoothDevice device = gatt.getDevice();
            String address = device.getAddress();
            Log.d("AWTK", "onConnectionStateChange (" + address + ") " + newState +
                    " status: " + status);

            List<BluetoothGattService> services = gatt.getServices();
            for (BluetoothGattService s : services) {
                String uuid = s.getUuid().toString();
                List<BluetoothGattCharacteristic> chars = s.getCharacteristics();
                for(BluetoothGattCharacteristic c : chars) {
                    String cs = c.toString();
                }
            }

            try {
                switch (newState) {
                    case BluetoothProfile.STATE_CONNECTED:
                        break;
                    case BluetoothProfile.STATE_DISCONNECTED:
                        break;
                    default:
                        // Log.e(TAG, "New state not processed: " + newState);
                        break;
                }
            } catch (NullPointerException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            BluetoothDevice device = gatt.getDevice();
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt,
                                            BluetoothGattCharacteristic characteristic) {
        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt,
                                         BluetoothGattCharacteristic characteristic, int status) {
        }

        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt,
                                          BluetoothGattCharacteristic characteristic, int status) {
        }

        @Override
        public void onDescriptorRead(BluetoothGatt gatt,
                                     BluetoothGattDescriptor descriptor, int status) {
            Log.i("AWTK", "onDescriptorWrite: " + descriptor.getUuid().toString());
        }

        @Override
        public void onDescriptorWrite(BluetoothGatt gatt,
                                      BluetoothGattDescriptor descriptor, int status) {
            Log.i("AWTK", "onDescriptorWrite: " + descriptor.getUuid().toString());
        }
    };

    public void scanDevices(boolean start) {
        if (start) {
            if (mScanning) {
                return;
            }
            mBluetoothDevices.clear();
            mBluetoothLeScanner.startScan(null, createScanSetting(), scanCallback);
            mScanning = true;
        } else {
            if (!mScanning) {
                return;
            }
            mScanning = false;
            mBluetoothLeScanner.stopScan(scanCallback);
        }
    }

    public ScanSettings createScanSetting() {
        ScanSettings.Builder builder = new ScanSettings.Builder();
        builder.setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            builder.setCallbackType(ScanSettings.CALLBACK_TYPE_ALL_MATCHES);
        }
        return builder.build();
    }

    @Override
    public void destroy() {
    }

    @Override
    public boolean matchRequest(int requestCode, int resultCode, Intent data) {
        int id = requestCode >> 16;
        Log.v("AWTK", id + "(id)vs " + this.id);

        return this.id == id;
    }

    private boolean startBLE() {
        if (mHandler == null) {
            mHandler = new Handler();
            if (!activity.getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
                Log.v("AWTK", "device does not support BLE.");
                return false;
            }

            final BluetoothManager bluetoothManager =
                    (BluetoothManager) activity.getSystemService(Context.BLUETOOTH_SERVICE);
            mBluetoothAdapter = bluetoothManager.getAdapter();
            mBluetoothLeScanner = mBluetoothAdapter.getBluetoothLeScanner();
        }

        if (!mBluetoothAdapter.isEnabled()) {
            int code = (this.id << 16) | START_CODE;
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            activity.startActivityForResult(enableBtIntent, code);
        } else {
            mStarted = true;
        }


        return true;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        int code = requestCode & 0xffff;
        if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            Log.v("AWTK", "onRequestPermissionsResult granted");
            if (this.startBLE()) {
                PluginManager.writeResult(this.callerInfo, "{\"result\":true, \"message\":\"ble started\"}");
            } else {
                PluginManager.writeResult(this.callerInfo, "{\"result\":false, \"message\":\"ble start failed\"}");
            }
        } else {
            Log.v("AWTK", "onRequestPermissionsResult deny");
            PluginManager.writeResult(this.callerInfo, "{\"result\":false, \"message\":\"permission deny\"}");
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        mStarted = resultCode != 0;
        return;
    }

    public boolean hasPermissions() {
        if (ContextCompat.checkSelfPermission(this.activity,
                Manifest.permission.BLUETOOTH) != PackageManager.PERMISSION_GRANTED) {
            return false;
        }
        if (ContextCompat.checkSelfPermission(this.activity,
                Manifest.permission.BLUETOOTH_ADMIN) != PackageManager.PERMISSION_GRANTED) {
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

            if (action.equals("start")) {
                if (this.hasPermissions()) {
                    if (this.startBLE()) {
                        PluginManager.writeResult(callerInfo, "{\"result\":true}");
                    } else {
                        PluginManager.writeResult(callerInfo, "{\"result\":false}");
                    }
                } else {
                    ActivityCompat.requestPermissions(this.activity,
                            new String[]{Manifest.permission.BLUETOOTH, Manifest.permission.BLUETOOTH_ADMIN,
                                    Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION}, START_CODE);
                }
            } else if (mStarted) {
                if (action.equals("start_scan")) {
                    this.scanDevices(true);
                    PluginManager.writeResult(callerInfo, "{\"result\":true}");
                } else if (action.equals("register")) {
                    this.notifyReceiver = json.getString("onevent");
                    PluginManager.writeResult(callerInfo, "{\"result\":true}");
                } else if (action.equals("unregister")) {
                    this.notifyReceiver = null;
                    PluginManager.writeResult(callerInfo, "{\"result\":true}");
                } else if (action.equals("connect")) {
                    this.connectDevice(json.getString("address"));
                } else if (action.equals("disconnect")) {
                    this.disconnectDevice(json.getString("address"));
                } else if (action.equals("stop_scan")) {
                    this.scanDevices(false);
                    PluginManager.writeResult(callerInfo, "{\"result\":true}");
                } else if (action.equals("get_info")) {
                    this.getInfo();
                } else {
                    PluginManager.writeResult(callerInfo, "{\"result\":false, \"message\":\"not supported action\"}");
                }
            } else {
                PluginManager.writeResult(callerInfo, "{\"result\":false, \"message\":\"please call start to start ble first\"}");
            }
        } catch (JSONException e) {
            Log.v("AWTK", e.toString());
            PluginManager.writeResult(this.callerInfo, "{}");
        }

        return true;
    }

    BLEPlugin(Activity activity, int id) {
        this.id = id;
        this.activity = activity;
        mBluetoothDevices = new ArrayList<BluetoothDevice>();
        mBluetoothGatts = new ArrayList<BluetoothGatt>();
    }

    BluetoothGatt findConnectionByAddr(final String address) {
        for (BluetoothGatt c : mBluetoothGatts) {
            String devAddress = c.getDevice().getAddress();
            if (address.equals(devAddress)) {
                return c;
            }
        }
        return null;
    }

    BluetoothDevice findDeviceByName(final String name) {
        for (BluetoothDevice device : mBluetoothDevices) {
            String devName = device.getName();
            if (name.equals(devName)) {
                return device;
            }
        }

        return null;
    }

    BluetoothDevice findDeviceByAddr(final String address) {
        for (BluetoothDevice device : mBluetoothDevices) {
            String devAddress = device.getAddress();
            if (address.equals(devAddress))  {
                return device;
            }
        }

        return null;
    }

    void connectDevice(String address) {
        BluetoothGatt conn = this.findConnectionByAddr(address);
        if (conn == null) {
            //BluetoothDevice device = this.findDeviceByAddr(address);
            BluetoothDevice device = this.findDeviceByName("mi_mtk");

            if (device != null) {
                conn = device.connectGatt(activity, true, mBluetoothGattCallback);
                if (conn != null) {
                    mBluetoothGatts.add(conn);
                    conn.discoverServices();
                    PluginManager.writeResult(this.callerInfo, "{\"result\":true, \"message\":\"success\"}");
                } else {
                    PluginManager.writeResult(this.callerInfo, "{\"result\":false, \"message\":\"connect failed\"}");
                }
            } else {
                PluginManager.writeResult(this.callerInfo, "{\"result\":false, \"message\":\"not found\"}");
            }
        } else {
            PluginManager.writeResult(this.callerInfo, "{\"result\":true, \"message\":\"connected already\"}");
        }
    }

    void disconnectDevice(String address) {
        BluetoothGatt conn = this.findConnectionByAddr(address);
        if (conn != null) {
            conn.disconnect();
            mBluetoothGatts.remove(conn);
            PluginManager.writeResult(this.callerInfo, "{\"result\":true, \"message\":\"success\"}");
        } else {
            PluginManager.writeResult(this.callerInfo, "{\"result\":false, \"message\":\"not found\"}");
        }

    }

    void getInfo() {
        int i = 0;
        String callerInfo = this.callerInfo;

        String str = "[\n";
        for (BluetoothDevice device : mBluetoothDevices) {
            if (i > 0) {
                str += ",\n";
            }
            str += "{\n";
            str += String.format("\"type\":%d,\n", device.getType());
            str += String.format("\"name\":\"%s\",\n", device.getName() != null ? device.getName() : "");
            str += String.format("\"address\":\"%s\"\n", device.getAddress());
            str += "}";
            i++;
        }
        str += "\n]\n";

        PluginManager.writeResult(callerInfo, str);
    }
}
