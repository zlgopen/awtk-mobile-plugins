/**
 * File:   awake.h
 * Author: AWTK Develop Team
 * Brief:  awake api
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
 * 2021-07-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_AWAKE_H
#define TK_AWAKE_H

#include "tkc/types_def.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class awake_t
 * @annotation ["fake"]
 * 用于禁止或允许设备睡眠。
 */

/**
 * @method awake_keep_screen_on
 * 用于禁止或允许设备睡眠。
 *
 * @annotation ["static"]
 * @param {bool_t} value TRUE禁止睡眠，FALSE允许睡眠。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t awake_keep_screen_on(bool_t value);

/**
 * @method awake_turn_screen_on
 * 点亮屏幕。
 *
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t awake_turn_screen_on(void);

END_C_DECLS

#endif /*TK_AWAKE_H*/
