//
// Created by ik_visualcamp on 2019. 2. 25..
//

#ifndef LogUtil_h
#define LogUtil_h

#ifdef __ANDROID__
  #include <android/log.h>
  #ifndef LOG_TAG
    #define LOG_TAG "LOG_TAG_ANDROID"
  #endif
#endif

#ifdef __APPLE__
  #include "TargetConditionals.h"
  #include <syslog.h>
#endif

#ifdef __ANDROID__
#ifdef NDEBUG
  #define LOGV(...)
  #define LOGD(...)
  #define LOGI(...)
  #define LOGW(...)
  #define LOGE(...)
  #define LOGF(...)
  #define LOGS(...)
#else
  #define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
  #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
  #define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
  #define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
  #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
  #define LOGF(...) __android_log_print(ANDROID_FATAL_ERROR, LOG_TAG, __VA_ARGS__)
  #define LOGS(...) __android_log_print(ANDROID_SILENT_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#elif __APPLE__ && TARGET_OS_IPHONE
  #ifdef DEBUG
    #define LOGV(...) syslog(LOG_NOTICE, __VA_ARGS__);
    #define LOGD(...) syslog(LOG_DEBUG, __VA_ARGS__);
    #define LOGI(...) syslog(LOG_INFO, __VA_ARGS__);
    #define LOGW(...) syslog(LOG_WARNING, __VA_ARGS__);
    #define LOGE(...) syslog(LOG_ERR, __VA_ARGS__);
    #define LOGF(...) syslog(LOG_ERR, __VA_ARGS__);
    #define LOGS(...) syslog(LOG_ERR, __VA_ARGS__);
  #else
    #define LOGV(...)
    #define LOGD(...)
    #define LOGI(...)
    #define LOGW(...)
    #define LOGE(...)
    #define LOGF(...)
    #define LOGS(...)
  #endif
#else
  #define LOGV(...)
  #define LOGD(...)
  #define LOGI(...)
  #define LOGW(...)
  #define LOGE(...)
  #define LOGF(...)
  #define LOGS(...)
#endif

#endif //LogUtil_h
