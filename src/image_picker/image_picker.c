/**
 * File:   image_picker.c
 * Author: AWTK Develop Team
 * Brief:  image_picker
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

#include "tkc/utils.h"
#include "image_picker/image_picker.h"
#include "common/platform_request.h"

ret_t image_picker_pick(image_picker_options_t* options, platform_request_on_result_t on_result,
                        void* on_result_ctx) {
  str_t str;
  ret_t ret = 0;
  uint32_t size = 0;
  return_value_if_fail(options != NULL && on_result != NULL, RET_BAD_PARAMS);

  size = tk_strlen(options->folder_title) + tk_strlen(options->image_title) + 100;
  return_value_if_fail(str_init(&str, size) != NULL, RET_OOM);

  str_append_char(&str, '{');
  str_append_json_bool_pair(&str, "single", options->single);
  str_append_char(&str, ',');
  str_append_json_bool_pair(&str, "camera_only", options->camera_only);
  str_append_char(&str, ',');
  str_append_json_bool_pair(&str, "show_camera", options->show_camera);

  if (options->folder_title != NULL) {
    str_append_char(&str, ',');
    str_append_json_str_pair(&str, "folder_title", options->folder_title);
  }
  if (options->image_title != NULL) {
    str_append_char(&str, ',');
    str_append_json_str_pair(&str, "image_title", options->image_title);
  }
  str_append_char(&str, '}');

  ret = platform_request_send("image_picker", "pick", str.str, on_result, on_result_ctx);

  str_reset(&str);

  return ret;
}
