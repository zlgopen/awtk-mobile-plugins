/**
 * File:   image_picker.h
 * Author: AWTK Develop Team
 * Brief:  image_picker api
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-03-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_PICKER_H
#define TK_IMAGE_PICKER_H

#include "tkc/types_def.h"
#include "common/platform_request.h"

BEGIN_C_DECLS

/**
 * @class image_picker_options_t
 * @annotation ["fake"]
 * 图片选择选项。
 */
typedef struct _image_picker_options_t {
  /**
   * @property {bool_t} single
   * 单选模式。
   */
  bool_t single;

  /**
   * @property {bool_t} include_videos
   * 显示相机。
   */
  bool_t include_videos;

} image_picker_options_t;

/**
 * @class image_picker_t
 * @annotation ["fake"]
 * 二维码识别。
 */

/**
 * @method image_picker_pick
 * 图片选择。
 *
 * > 回调函数是在 AWTK UI 线程中执行的，可以在回调函数中直接操作 AWTK 控件。
 * 
 * @annotation ["static"]
 * @param {image_picker_options_t*} options 选项。
 * @param {platform_request_on_result_t} on_result 完成时的回调函数。
 * @param {void*} on_result_ctx 完成时的回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t image_picker_pick(image_picker_options_t* options, platform_request_on_result_t on_result,
                        void* on_result_ctx);

END_C_DECLS

#endif /*TK_IMAGE_PICKER_H*/
