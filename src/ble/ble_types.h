
/**
 * File:   ble_types.h
 * Author: AWTK Develop Team
 * Brief:  ble types
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ===============================================================
 * 2021-02-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BLE_TYPES_H
#define TK_BLE_TYPES_H

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/darray.h"
#include "conf_io/conf_json.h"
#include "tkc/data_reader_mem.h"

#include "common/platform_request.h"

BEGIN_C_DECLS

struct _ble_char_t;
typedef struct _ble_char_t ble_char_t;

struct _ble_t;
typedef struct _ble_t ble_t;

struct _ble_desc_t;
typedef struct _ble_desc_t ble_desc_t;

struct _ble_service_t;
typedef struct _ble_service_t ble_service_t;

struct _ble_device_t;
typedef struct _ble_device_t ble_device_t;

typedef ret_t (*ble_on_write_desc_result_t)(void* ctx, ble_desc_t* desc, ret_t result);
typedef ret_t (*ble_on_discover_services_result_t)(void* ctx, ble_device_t* device);
typedef ret_t (*ble_on_read_desc_result_t)(void* ctx, ble_desc_t* desc, const void* data,
                                           uint32_t size);

END_C_DECLS

#endif /*TK_BLE_TYPES_H*/
