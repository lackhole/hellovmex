//
// Created by YongGyu Lee on 2020/05/21.
//

#include "JNIWrapper.hpp"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL
Java_com_example_hello_1vmex_jni_NativeWrapper_nativeInitWrapper(
        JNIEnv *env, jobject inst ) {
    auto* wrapper = new JNIWrapper();
    return reinterpret_cast<jlong>(wrapper);
}

JNIEXPORT void JNICALL
Java_com_example_hello_1vmex_jni_NativeWrapper_nativeSetCallback(
        JNIEnv* env, jobject inst,
        jlong wrapper_ptr, jobject callback ) {
    reinterpret_cast<JNIWrapper *>(wrapper_ptr)->setCallback(env, callback);
}


JNIEXPORT void JNICALL
Java_com_example_hello_1vmex_jni_NativeWrapper_nativeRunCallback(
        JNIEnv* env, jobject inst,
        jlong wrapper_ptr){
    reinterpret_cast<JNIWrapper *>(wrapper_ptr)->test();
}


JNIEXPORT void JNICALL
Java_com_example_hello_1vmex_jni_NativeWrapper_nativeSetNumThreads(
        JNIEnv* env, jobject inst,
        jlong wrapper_ptr, jint numThreads){
    auto wrapper = reinterpret_cast<JNIWrapper *>(wrapper_ptr);
    wrapper->setNumThread(numThreads);
    wrapper->setCallbackInterface();
}


JNIEXPORT void JNICALL
Java_com_example_hello_1vmex_jni_NativeWrapper_nativeRun(
        JNIEnv* env, jobject inst,
        jlong wrapper_ptr) {
    reinterpret_cast<JNIWrapper *>(wrapper_ptr)->run();
}


JNIEXPORT void JNICALL
Java_com_example_hello_1vmex_jni_NativeWrapper_nativeStop(
        JNIEnv* env, jobject inst,
        jlong wrapper_ptr) {
    reinterpret_cast<JNIWrapper *>(wrapper_ptr)->stop();
}


JNIEXPORT void JNICALL
Java_com_example_hello_1vmex_jni_NativeWrapper_nativeRunMain(
        JNIEnv* env, jobject inst,
        jlong wrapper_ptr) {
    reinterpret_cast<JNIWrapper *>(wrapper_ptr)->runMain();
}

#ifdef __cplusplus
}
#endif