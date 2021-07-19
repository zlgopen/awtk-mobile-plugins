/**
 * File:   wake_lock.h
 * Author: AWTK Develop Team
 * Brief:  wake_lock api
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
 * 2021-07-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WAKE_LOCK_H
#define TK_WAKE_LOCK_H

#include "tkc/types_def.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class wake_lock_t
 * @annotation ["fake"]
 * 用于禁止或允许设备睡眠。
 */

/**
 * @method wake_lock_require
 * 获取wake锁。
 *
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wake_lock_require(void);

/**
 * @method wake_lock_release
 * 释放wake锁。
 *
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wake_lock_release(void);

END_C_DECLS

#endif /*TK_WAKE_LOCK_H*/
