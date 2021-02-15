/**
 * File:   plugin_manager.c
 * Author: AWTK Develop Team
 * Brief:  plugin_manager
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */
#include "platform_request.h"

#ifdef ANDROID
#include "SDL.h"
#include <jni.h>

JNIEXPORT jstring JNICALL Java_org_zlgopen_plugins_PluginManager_readRequest(JNIEnv* env,
                                                                             jclass cls) {
  str_t str;
  str_init(&str, 0);
  if (platform_request_recv(&str) == RET_OK) {
    jstring jstr = (*env)->NewStringUTF(env, str.str);
    str_reset(&str);

    return jstr;
  } else {
    return (*env)->NewStringUTF(env, "");
  }
}

JNIEXPORT void JNICALL Java_org_zlgopen_plugins_PluginManager_writeResult(JNIEnv* env, jclass cls,
                                                                          jstring jcallInfo,
                                                                          jstring jresult) {
  void* ctx = NULL;
  platform_request_on_result_t on_result = NULL;
  const char* callInfo = (*env)->GetStringUTFChars(env, jcallInfo, JNI_FALSE);
  const char* result = (*env)->GetStringUTFChars(env, jresult, JNI_FALSE);

  if (sscanf(callInfo, "%p:%p", &on_result, &ctx) == 2) {
    if (on_result != NULL) {
      on_result(ctx, result);
    }
  }

  (*env)->ReleaseStringUTFChars(env, jcallInfo, callInfo);
  (*env)->ReleaseStringUTFChars(env, jresult, result);

  return;
}

JNIEXPORT void JNICALL Java_org_zlgopen_plugins_PluginManager_init(JNIEnv* env, jclass cls) {
  platform_request_init();
  log_debug("Java_org_zlgopen_plugins_PluginManager_init\n");

  return;
}

JNIEXPORT void JNICALL Java_org_zlgopen_plugins_PluginManager_deinit(JNIEnv* env, jclass cls) {
  platform_request_deinit();
  log_debug("Java_org_zlgopen_plugins_PluginManager_deinit\n");

  return;
}
#endif /*ANDROID*/
