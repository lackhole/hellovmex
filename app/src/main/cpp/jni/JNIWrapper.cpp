//
// Created by YongGyu Lee on 2020/05/21.
//


#include <utils/LogUtil.h>
#include "JNIWrapper.hpp"

JavaVM* JNIWrapper::jvm = nullptr;
JNIWrapper::CallbackFields JNIWrapper::callbackFields;

JNIWrapper::Callback::Callback(jobject globalCallback)
: instance(globalCallback)
{}

JNIWrapper::Callback::~Callback() {
    instance = nullptr;
}

inline void
JNIWrapper::Callback::updateGazeInferenceTime(float ms) {
    std::lock_guard<std::mutex> locker(this->callback_lock);
    JNIEnvManager env(jvm);
    env->CallVoidMethod(instance, callbackFields.updateGazeInferenceTime.methodID, ms);
}

inline void
JNIWrapper::Callback::updateGeneratorInferenceTime(float ms) {
    std::lock_guard<std::mutex> locker(this->callback_lock);
    JNIEnvManager env(jvm);
    env->CallVoidMethod(instance, callbackFields.updateGeneratorInferenceTime.methodID, ms);
}

inline void
JNIWrapper::Callback::updateRegressorInferenceTime(float ms) {
    std::lock_guard<std::mutex> locker(this->callback_lock);
    JNIEnvManager env(jvm);
    env->CallVoidMethod(instance, callbackFields.updateRegressorInferenceTime.methodID, ms);
//    callVoidJNIMethod(jvm, instance,
//        callbackFields.updateRegressorInferenceTime.methodID, ms);
}

inline void
JNIWrapper::Callback::setText(const char* string) {
    std::lock_guard<std::mutex> locker(this->callback_lock);
    JNIEnvManager env(jvm);
    
    jstring jstr = env->NewStringUTF(string);
    env->CallVoidMethod(instance,callbackFields.setText.methodID, jstr);
}

inline void
JNIWrapper::Callback::appendText(const char* string) {
    std::lock_guard<std::mutex> locker(this->callback_lock);
    JNIEnvManager env(jvm);
    
    jstring jstr = env->NewStringUTF(string);
    env->CallVoidMethod(instance, callbackFields.appendText.methodID, jstr);
}

inline void
JNIWrapper::Callback::clearText() {
    std::lock_guard<std::mutex> locker(this->callback_lock);
    JNIEnvManager env(jvm);
    
    env->CallVoidMethod(instance, callbackFields.clearText.methodID);
}


JNIWrapper::JNIWrapper()
: callback(nullptr)
{
    controller = nullptr;
}

JNIWrapper::~JNIWrapper() {
    delete callback;
    delete controller;
}

bool JNIWrapper::setCallback(JNIEnv *env, jobject javaCallbackObj) {
    jvm = nullptr;
    env->GetJavaVM(&jvm);
    jobject globalCallback = env->NewGlobalRef(javaCallbackObj);
    callbackObject = nullptr;
    
    if (env->IsSameObject(callbackObject, globalCallback)) return true;
    if (callbackObject) env->DeleteGlobalRef(callbackObject);
    
    callbackObject = globalCallback;
    for(auto& cb : callbackFields.callbacks) cb.get().methodID = NULL;
    
    if (globalCallback) {
        jclass clazz = env->GetObjectClass(globalCallback);
        
        if (clazz)
            for(auto& cb : callbackFields.callbacks)
                cb.get().methodID = env->GetMethodID(clazz, cb.get().name, cb.get().sig);
            
        env->ExceptionClear();
        
        for (const auto& cb : callbackFields.callbacks)
            if (!cb.get().methodID) {
                LOGD("Failed to find a function");
                env->DeleteGlobalRef(globalCallback);
                callbackObject = nullptr;
                return false;
            }
    }
    
    callback = new Callback(callbackObject);
    return true;
}

void JNIWrapper::setCallbackInterface() {
    if (controller == nullptr) {
        LOGD("Cannot set callbackInterface: controller is nullptr");
        return;
    }
    if (callback == nullptr){
        LOGD("Cannot set callbackInterface: callback is nullptr");
        return;
    }
    
    controller->setCallbackInterface(callback);
}

void JNIWrapper::setNumThread(int numThreads) {
    controller = new Controller(numThreads);
}

void JNIWrapper::run() {
    LOGD("JNIWrapper::run()");
    
    if (controller == nullptr || controller->isRunning())
        return;
    
    controller->run();
}

void JNIWrapper::stop() {
    LOGD("JNIWrapper::stop()");
    if (!controller->isRunning())
        return;
    controller->stop();
}

void JNIWrapper::runMain() {
    if (controller == nullptr) return;
    
    controller->runMain();
}