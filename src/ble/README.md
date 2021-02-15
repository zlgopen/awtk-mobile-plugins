# BLE

AWTK 的低功耗蓝牙 (BLE) 插件

## 包含头文件

```c
#include "ble/ble.h"
```

## 调用方法

* 1. 创建 ble 对象。

如：

```c
  ble_t* ble = ble_create();
```

* 2. 扫描设备

> 扫描前先要注册 on\_device\_found 回调函数，每扫描到一个设备就会调用该回调函数。

```c
static ret_t ble_main_on_device_found(void* ctx, ble_device_t* device) {
  ...
  return RET_OK;
}

ble_set_on_device_found(ble, ble_main_on_device_found, win);
```

```c
  ble_start_scan(ble);
```

* 3. 连接设备

> 当 on\_device\_services\_discovered 被调用时，设备才真正连接，之后才能收发数据。

注册回调函数：

```c
static ret_t ble_deivce_on_device_services_discovered(void* ctx, ble_device_t* device) {

  return RET_OK;
}

ble_set_on_device_services_discovered(device->ble, ble_deivce_on_device_services_discovered, win);
```

```c
  ble_connect_to(ble, address);
```

* 4. 收发数据。

> 收发数据是异步的，先注册相应的回调函数，再收发数据。

```c

  ble_set_on_characteristic_write(device->ble, ble_on_characteristic_write, win);
  ble_set_on_characteristic_read(device->ble, ble_on_characteristic_read, win);
```

```c
  ble_read_characteristic(device->ble, device->id, achar->id);
  ble_write_characteristic(device->ble, device->id, achar->id, data, FALSE);
```

* 5. 主动上报的数据

注册 on\_characteristic\_changed 即可。

```c
  ble_set_on_characteristic_changed(device->ble, ble_on_characteristic_changed, win);
```  

完整示例请参考 demo。

## API

[BLE API](https://github.com/zlgopen/awtk-mobile-plugins/blob/master/src/ble/ble.h)