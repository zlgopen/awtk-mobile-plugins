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
  return_value_if_fail(options != NULL && on_result != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_init(&str, 100) != NULL, RET_OOM);

  str_append_char(&str, '{');
  str_append_json_bool_pair(&str, "single", options->single);
  str_append_char(&str, ',');
  str_append_json_bool_pair(&str, "include_videos", options->include_videos);
  str_append_char(&str, '}');

  ret = platform_request_send("image_picker", "pick", str.str, on_result, on_result_ctx);

  str_reset(&str);

  return ret;
}
