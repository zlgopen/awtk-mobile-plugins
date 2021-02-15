
/**
 * File:   ble_uuid.h
 * Author: AWTK Develop Team
 * Brief:  ble uuid
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
 * 2021-02-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BLE_UUID_H
#define TK_BLE_UUID_H

#include "ble_types.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

const char* ble_uuid_to_desc(const char* uuid);

END_C_DECLS

#endif /*TK_BLE_UUID_H*/
