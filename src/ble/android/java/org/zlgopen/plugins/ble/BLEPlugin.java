package org.zlgopen.plugins;

//参考资源
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
import org.json.JSONArray;

import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.ArrayList;
import java.util.UUID;

public class BLEPlugin implements Plugin {
    private int id;
    private Activity activity;
    private String action;
    private String callerInfo;
    private String notifyReceiver;

    private static final int START_CODE = 1;

    private Handler mHandler;
    private boolean mScanning;
    private boolean mStarted;
    private BluetoothAdapter mBluetoothAdapter;
    private List<BluetoothGatt> mBluetoothGatts;
    private BluetoothLeScanner mBluetoothLeScanner;
    private List<BluetoothDevice> mBluetoothDevices;

    public static String byteArrayToHex(byte[] a) {
        StringBuilder sb = new StringBuilder(a.length * 2);
        for (byte b : a)
            sb.append(String.format("%02x ", b));
        return sb.toString();
    }

    public JSONArray serviceToJson(List<BluetoothGattService> services) {
        try {
            JSONArray json = new JSONArray();

            for (BluetoothGattService s : services) {
                JSONObject jsonService = new JSONObject();
                JSONArray jsonChars = new JSONArray();

                json.put(jsonService);
                jsonService.put("uuid", s.getUuid().toString());
                jsonService.put("chars", jsonChars);

                List<BluetoothGattCharacteristic> chars = s.getCharacteristics();
                for (BluetoothGattCharacteristic c : chars) {
                    JSONObject jsonChar = new JSONObject();
                    JSONArray jsonDescs = new JSONArray();
                    jsonChars.put(jsonChar);
                    int permissions = c.getPermissions();

                    jsonChar.put("uuid", c.getUuid().toString());
                    jsonChar.put("descs", jsonDescs);

                    int properties = c.getProperties();

                    if ((properties & BluetoothGattCharacteristic.PROPERTY_BROADCAST) != 0x0) {
                        jsonChar.put("Broadcast", true);
                    }

                    if ((properties & BluetoothGattCharacteristic.PROPERTY_READ) != 0x0) {
                        jsonChar.put("Read", true);
                    }

                    if ((properties & BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE) != 0x0) {
                        jsonChar.put("WriteWithoutResponse", true);
                    }

                    if ((properties & BluetoothGattCharacteristic.PROPERTY_WRITE) != 0x0) {
                        jsonChar.put("Write", true);
                    }

                    if ((properties & BluetoothGattCharacteristic.PROPERTY_NOTIFY) != 0x0) {
                        jsonChar.put("Notify", true);
                    }

                    if ((properties & BluetoothGattCharacteristic.PROPERTY_INDICATE) != 0x0) {
                        jsonChar.put("Indicate", true);
                    }

                    if ((properties & BluetoothGattCharacteristic.PROPERTY_SIGNED_WRITE) != 0x0) {
                        jsonChar.put("AuthenticateSignedWrites", true);
                    }

                    if ((properties & BluetoothGattCharacteristic.PROPERTY_EXTENDED_PROPS) != 0x0) {
                        jsonChar.put("ExtendedProperties", true);
                    }
/*
                    int cpermissions = c.getPermissions();

                    if ((cpermissions & BluetoothGattCharacteristic.PERMISSION_READ) != 0x0) {
                        jsonChar.put("Read", true);
                    }

                    if ((cpermissions & BluetoothGattCharacteristic.PERMISSION_WRITE) != 0x0) {
                        jsonChar.put("Write", true);
                    }

                    if ((cpermissions & BluetoothGattCharacteristic.PERMISSION_READ_ENCRYPTED) != 0x0) {
                        jsonChar.put("ReadEncrypted", true);
                    }

                    if ((cpermissions & BluetoothGattCharacteristic.PERMISSION_WRITE_ENCRYPTED) != 0x0) {
                        jsonChar.put("WriteEncrypted", true);
                    }

                    if ((cpermissions & BluetoothGattCharacteristic.PERMISSION_READ_ENCRYPTED_MITM) != 0x0) {
                        jsonChar.put("ReadEncryptedMITM", true);
                    }

                    if ((cpermissions & BluetoothGattCharacteristic.PERMISSION_WRITE_ENCRYPTED_MITM) != 0x0) {
                        jsonChar.put("WriteEncryptedMITM", true);
                    }

                    if ((cpermissions & BluetoothGattCharacteristic.PERMISSION_WRITE_SIGNED) != 0x0) {
                        jsonChar.put("WriteSigned", true);
                    }

                    if ((cpermissions & BluetoothGattCharacteristic.PERMISSION_WRITE_SIGNED_MITM) != 0x0) {
                        jsonChar.put("WriteSignedMITM", true);
                    }
*/
                    List<BluetoothGattDescriptor> descs = c.getDescriptors();
                    for (BluetoothGattDescriptor d : descs) {
                        JSONObject jsonDesc = new JSONObject();
                        jsonDescs.put(jsonDesc);

                        jsonDesc.put("uuid", d.getUuid().toString());
/*
                        int dpermission = d.getPermissions();

                        if ((dpermission & BluetoothGattDescriptor.PERMISSION_READ) != 0x0) {
                            jsonDesc.put("Read", true);
                        }

                        if ((dpermission & BluetoothGattDescriptor.PERMISSION_WRITE) != 0x0) {
                            jsonDesc.put("Write", true);
                        }

                        if ((dpermission & BluetoothGattDescriptor.PERMISSION_READ_ENCRYPTED) != 0x0) {
                            jsonDesc.put("ReadEncrypted", true);
                        }

                        if ((dpermission & BluetoothGattDescriptor.PERMISSION_WRITE_ENCRYPTED) != 0x0) {
                            jsonDesc.put("WriteEncrypted", true);
                        }

                        if ((dpermission & BluetoothGattDescriptor.PERMISSION_READ_ENCRYPTED_MITM) != 0x0) {
                            jsonDesc.put("ReadEncryptedMITM", true);
                        }

                        if ((dpermission & BluetoothGattDescriptor.PERMISSION_WRITE_ENCRYPTED_MITM) != 0x0) {
                            jsonDesc.put("WriteEncryptedMITM", true);
                        }

                        if ((dpermission & BluetoothGattDescriptor.PERMISSION_WRITE_SIGNED) != 0x0) {
                            jsonDesc.put("WriteSigned", true);
                        }

                        if ((dpermission & BluetoothGattDescriptor.PERMISSION_WRITE_SIGNED_MITM) != 0x0) {
                            jsonDesc.put("WriteSignedMITM", true);
                        }
 */
                    }
                }
            }

            return json;
        } catch (JSONException e) {
            Log.v("AWTK", e.toString());
            return null;
        }
    }

    public BluetoothGattCharacteristic findCharacteristic(List<BluetoothGattService> services, String uuid) {
        for (BluetoothGattService s : services) {
            BluetoothGattCharacteristic c = s.getCharacteristic(UUID.fromString(uuid));
            if (c != null) {
                return c;
            }
        }
        return null;
    }

    public BluetoothGattDescriptor findDescriptor(List<BluetoothGattService> services, String uuid) {
        for (BluetoothGattService s : services) {
            List<BluetoothGattCharacteristic> chars = s.getCharacteristics();
            for (BluetoothGattCharacteristic c : chars) {
                BluetoothGattDescriptor d = c.getDescriptor(UUID.fromString(uuid));
                if (d != null) {
                    return d;
                }
            }
        }

        return null;

    }

    public JSONObject deviceToJson(BluetoothDevice device, String msgType, int rssi, List<BluetoothGattService> services) {
        try {
            String addr = device.getAddress();
            String name = device.getName();
            int type = device.getType();

            JSONObject json = new JSONObject()
                    .put("type", msgType)
                    .put("rssi", rssi)
                    .put("name", name)
                    .put("ble", type == BluetoothDevice.DEVICE_TYPE_LE)
                    .put("addr", addr);

            if (services != null) {
                json.put("services", this.serviceToJson(services));
            }

            return json;
        } catch (JSONException e) {
            return null;
        }
    }

    private ScanCallback mScanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            BluetoothDevice device = result.getDevice();

            if (findDeviceByAddr(device.getAddress()) == null) {
                mBluetoothDevices.add(device);

                String str = deviceToJson(device, "onScanResult", result.getRssi(), null).toString();
                Log.v("AWTK", str);
                if (notifyReceiver != null) {
                    PluginManager.writeResult(notifyReceiver, str);
                }
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

            try {
                switch (newState) {
                    case BluetoothProfile.STATE_CONNECTED:
                        gatt.discoverServices();
                        break;
                    case BluetoothProfile.STATE_DISCONNECTED:
                        mBluetoothGatts.remove(gatt);
                        break;
                    default:
                        // Log.e(TAG, "New state not processed: " + newState);
                        break;
                }
            } catch (NullPointerException e) {
                e.printStackTrace();
            }
        }

        public void enableNotify(BluetoothGatt gatt) {
            List<BluetoothGattService> services = gatt.getServices();

            for (BluetoothGattService s : services) {
                List<BluetoothGattCharacteristic> chars = s.getCharacteristics();
                for (BluetoothGattCharacteristic c : chars) {
                    gatt.setCharacteristicNotification(c, true);
                }
            }

            return;
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            BluetoothDevice device = gatt.getDevice();
            JSONObject json = deviceToJson(device, "onServicesDiscovered", 0, gatt.getServices());
            String str = json.toString();

            this.enableNotify(gatt);

            Log.v("AWTK", str);
            if (notifyReceiver != null) {
                PluginManager.writeResult(notifyReceiver, str);
            }
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt,
                                            BluetoothGattCharacteristic characteristic) {
            super.onCharacteristicChanged(gatt, characteristic);
            Log.i("AWTK", "onCharacteristicChanged: " + characteristic.getUuid().toString());
            byte[] bytes = characteristic.getValue();
            String data = new String(bytes, StandardCharsets.UTF_8);
            String hex = byteArrayToHex(bytes);

            if (notifyReceiver != null) {
                try {
                    JSONObject json = new JSONObject();
                    json.put("type", "onCharacteristicChanged")
                            .put("address", gatt.getDevice().getAddress())
                            .put("uuid", characteristic.getUuid().toString())
                            .put("hex", hex)
                            .put("data", data);
                    PluginManager.writeResult(notifyReceiver, json.toString());
                } catch (JSONException e) {
                    Log.d("AWTK", e.toString());
                }
            }
        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt,
                                         BluetoothGattCharacteristic characteristic, int status) {
            super.onCharacteristicRead(gatt, characteristic, status);
            Log.i("AWTK", "onDescriptorWrite: " + characteristic.getUuid().toString());
            byte[] bytes = characteristic.getValue();
            String data = new String(bytes, StandardCharsets.UTF_8);
            String hex = byteArrayToHex(bytes);

            if (notifyReceiver != null) {
                try {
                    JSONObject json = new JSONObject();
                    json.put("type", "onCharacteristicRead")
                            .put("address", gatt.getDevice().getAddress())
                            .put("uuid", characteristic.getUuid().toString())
                            .put("status", status)
                            .put("hex", hex)
                            .put("data", data);
                    PluginManager.writeResult(notifyReceiver, json.toString());
                } catch (JSONException e) {
                    Log.d("AWTK", e.toString());
                }
            }
        }

        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt,
                                          BluetoothGattCharacteristic characteristic, int status) {
            super.onCharacteristicWrite(gatt, characteristic, status);
            Log.i("AWTK", "onDescriptorWrite: " + characteristic.getUuid().toString());
            byte[] bytes = characteristic.getValue();
            String data = new String(bytes, StandardCharsets.UTF_8);
            String hex = byteArrayToHex(bytes);

            if (notifyReceiver != null) {
                try {
                    JSONObject json = new JSONObject();
                    json.put("type", "onCharacteristicWrite")
                            .put("address", gatt.getDevice().getAddress())
                            .put("uuid", characteristic.getUuid().toString())
                            .put("status", status)
                            .put("hex", hex)
                            .put("data", data);
                    PluginManager.writeResult(notifyReceiver, json.toString());
                } catch (JSONException e) {
                    Log.d("AWTK", e.toString());
                }
            }
        }

        @Override
        public void onDescriptorRead(BluetoothGatt gatt,
                                     BluetoothGattDescriptor descriptor, int status) {
            super.onDescriptorRead(gatt, descriptor, status);

            Log.i("AWTK", "onDescriptorRead: " + descriptor.getUuid().toString());
            byte[] bytes = descriptor.getValue();
            String data = new String(bytes, StandardCharsets.UTF_8);
            String hex = byteArrayToHex(bytes);

            if (notifyReceiver != null) {
                try {
                    JSONObject json = new JSONObject();
                    json.put("type", "onDescriptorRead")
                            .put("address", gatt.getDevice().getAddress())
                            .put("uuid", descriptor.getUuid().toString())
                            .put("status", status)
                            .put("hex", hex)
                            .put("data", data);
                    PluginManager.writeResult(notifyReceiver, json.toString());
                } catch (JSONException e) {
                    Log.d("AWTK", e.toString());
                }
            }
        }

        @Override
        public void onDescriptorWrite(BluetoothGatt gatt,
                                      BluetoothGattDescriptor descriptor, int status) {
            super.onDescriptorWrite(gatt, descriptor, status);
            Log.i("AWTK", "onDescriptorWrite: " + descriptor.getUuid().toString());
            byte[] bytes = descriptor.getValue();
            String data = new String(bytes, StandardCharsets.UTF_8);
            String hex = byteArrayToHex(bytes);

            if (notifyReceiver != null) {
                try {
                    JSONObject json = new JSONObject();
                    json.put("type", "onDescriptorWrite")
                            .put("address", gatt.getDevice().getAddress())
                            .put("uuid", descriptor.getUuid().toString())
                            .put("status", status)
                            .put("hex", hex)
                            .put("data", data);
                    PluginManager.writeResult(notifyReceiver, json.toString());
                } catch (JSONException e) {
                    Log.d("AWTK", e.toString());
                }
            }
        }
    };

    public void getConnectedDevices() {
        final BluetoothManager bluetoothManager =
                (BluetoothManager) activity.getSystemService(Context.BLUETOOTH_SERVICE);
        List<BluetoothDevice> connectedDevices = bluetoothManager.getConnectedDevices(BluetoothProfile.GATT);

        BluetoothGatt gatt = null;
        for (BluetoothDevice d : connectedDevices) {
            gatt = findConnectionByAddr(d.getAddress());
            if (gatt == null) {
                gatt = d.connectGatt(activity, true, mBluetoothGattCallback);
            }

            JSONObject json = deviceToJson(d, "onDeviceConnected", 0, gatt.getServices());
            String str = json.toString();
            if (notifyReceiver != null) {
                PluginManager.writeResult(notifyReceiver, str);
            }
        }
    }

    public void scanDevices(boolean start) {
        if (start) {
            if (mScanning) {
                return;
            }

            mBluetoothDevices.clear();
            mBluetoothLeScanner.startScan(null, createScanSetting(), mScanCallback);
            mScanning = true;
        } else {
            if (!mScanning) {
                return;
            }
            mScanning = false;
            mBluetoothLeScanner.stopScan(mScanCallback);
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
            this.action = action;

            JSONObject json = new JSONObject(args);

            if (action.equals("start")) {
                if (this.hasPermissions()) {
                    if (this.startBLE()) {
                        PluginManager.writeSuccess(callerInfo, action);
                    } else {
                        PluginManager.writeFailure(callerInfo, action, "start ble fail");
                    }
                } else {
                    ActivityCompat.requestPermissions(this.activity,
                            new String[]{Manifest.permission.BLUETOOTH, Manifest.permission.BLUETOOTH_ADMIN,
                                    Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION}, START_CODE);
                }
            } else if (mStarted) {
                if (action.equals("start_scan")) {
                    this.scanDevices(true);
                    PluginManager.writeSuccess(callerInfo, action);
                } else if (action.equals("get_connected_devices")) {
                    this.getConnectedDevices();
                    PluginManager.writeSuccess(callerInfo, action);
                } else if (action.equals("register")) {
                    this.notifyReceiver = json.getString("onevent");
                    PluginManager.writeSuccess(callerInfo, action);
                } else if (action.equals("unregister")) {
                    this.notifyReceiver = null;
                    PluginManager.writeSuccess(callerInfo, action);
                } else if (action.equals("connect")) {
                    this.connectDevice(json.getString("address"));
                } else if (action.equals("write_char")) {
                    this.writeChar(json);
                } else if (action.equals("write_desc")) {
                    this.writeDesc(json);
                } else if (action.equals("read_char")) {
                    this.readChar(json);
                } else if (action.equals("read_desc")) {
                    this.readDesc(json);
                } else if (action.equals("disconnect")) {
                    this.disconnectDevice(json.getString("address"));
                } else if (action.equals("stop_scan")) {
                    this.scanDevices(false);
                    PluginManager.writeSuccess(callerInfo, action);
                } else {
                    PluginManager.writeFailure(callerInfo, action, "not supported action");
                }
            } else {
                PluginManager.writeFailure(callerInfo, action, "please call start to start ble first");
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
            if (address.equals(devAddress)) {
                return device;
            }
        }

        return null;
    }

    void readChar(JSONObject json) {
        try {
            String address = json.getString("address");
            String uuid = json.getString("uuid");

            BluetoothGatt conn = this.findConnectionByAddr(address);
            if (conn != null) {
                BluetoothGattCharacteristic c = findCharacteristic(conn.getServices(), uuid);
                if (c != null) {
                    conn.readCharacteristic(c);
                    PluginManager.writeSuccess(this.callerInfo, this.action);
                } else {
                    PluginManager.writeFailure(this.callerInfo, this.action, "not found BluetoothGattCharacteristic");
                }
            } else {
                PluginManager.writeFailure(this.callerInfo, this.action, "please connect device first");
            }
        } catch (JSONException e) {
            PluginManager.writeFailure(this.callerInfo, this.action, "bad parameters");
        }
    }

    void readDesc(JSONObject json) {
        try {
            String address = json.getString("address");
            String uuid = json.getString("uuid");

            BluetoothGatt conn = this.findConnectionByAddr(address);
            if (conn != null) {
                BluetoothGattDescriptor d = findDescriptor(conn.getServices(), uuid);
                if (d != null) {
                    conn.readDescriptor(d);
                    PluginManager.writeSuccess(this.callerInfo, this.action);
                } else {
                    PluginManager.writeFailure(this.callerInfo, this.action, "not found BluetoothGattDescriptor");
                }
            } else {
                PluginManager.writeFailure(this.callerInfo, this.action, "please connect device first");
            }
        } catch (JSONException e) {
            PluginManager.writeFailure(this.callerInfo, this.action, "bad parameters");
        }

    }

    void writeChar(JSONObject json) {
        try {
            String address = json.getString("address");
            String uuid = json.getString("uuid");
            String data = json.getString("data");
            String format = json.getString("format");
            /*TODO: Support HEX*/
            
            BluetoothGatt conn = this.findConnectionByAddr(address);
            if (conn != null) {
                BluetoothGattCharacteristic c = findCharacteristic(conn.getServices(), uuid);
                if (c != null) {
                    conn.setCharacteristicNotification(c, true);
                    c.setValue(data);
                    conn.writeCharacteristic(c);
                    PluginManager.writeSuccess(this.callerInfo, this.action);
                } else {
                    PluginManager.writeFailure(this.callerInfo, this.action, "not found BluetoothGattCharacteristic");
                }
            } else {
                PluginManager.writeFailure(this.callerInfo, this.action, "please connect device first");
            }
        } catch (JSONException e) {
            PluginManager.writeFailure(this.callerInfo, this.action, "bad parameters");
        }
    }

    void writeDesc(JSONObject json) {
        try {
            String address = json.getString("address");
            String uuid = json.getString("uuid");
            String data = json.getString("data");
            String format = json.getString("format");
            /*TODO: Support HEX*/

            BluetoothGatt conn = this.findConnectionByAddr(address);
            if (conn != null) {
                BluetoothGattDescriptor d = findDescriptor(conn.getServices(), uuid);
                if (d != null) {
                    d.setValue(data.getBytes());
                    conn.writeDescriptor(d);
                    PluginManager.writeSuccess(this.callerInfo, this.action);
                } else {
                    PluginManager.writeFailure(this.callerInfo, this.action, "not found BluetoothGattCharacteristic");
                }
            } else {
                PluginManager.writeFailure(this.callerInfo, this.action, "please connect device first");
            }
        } catch (JSONException e) {
            PluginManager.writeFailure(this.callerInfo, this.action, "bad parameters");
        }
    }

    void connectDevice(String address) {
        BluetoothGatt conn = this.findConnectionByAddr(address);
        if (mScanning) {
            scanDevices(false);
        }

        if (conn == null) {
            BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);

            if (device != null) {
                conn = device.connectGatt(activity, true, mBluetoothGattCallback);
                if (conn != null) {
                    mBluetoothGatts.add(conn);
                    PluginManager.writeSuccess(this.callerInfo, this.action);
                } else {
                    PluginManager.writeFailure(this.callerInfo, this.action, "connect failed");
                }
            } else {
                PluginManager.writeFailure(this.callerInfo, this.action, "not found");
            }
        } else {
            PluginManager.writeFailure(this.callerInfo, this.action, "connected already");
        }
    }

    void disconnectDevice(String address) {
        BluetoothGatt conn = this.findConnectionByAddr(address);
        if (conn != null) {
            conn.disconnect();
            mBluetoothGatts.remove(conn);
            PluginManager.writeSuccess(this.callerInfo, this.action);
        } else {
            PluginManager.writeFailure(this.callerInfo, this.action, "not found");
        }
    }
}
