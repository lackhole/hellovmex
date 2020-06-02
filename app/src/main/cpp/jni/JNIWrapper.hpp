//
// Created by YongGyu Lee on 2020/05/22.
//

#ifndef HELLO_VMEX_JNIWRAPPER_HPP
#define HELLO_VMEX_JNIWRAPPER_HPP

#include <jni.h>
#include <CallbackInterface.h>
#include <vector>
#include <Controller.h>

class JNIEnvManager{
public:
    JNIEnvManager(JavaVM* jvm) : jvm(jvm) {
        envStat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
        if (envStat != JNI_OK) jvm->AttachCurrentThread(&env, NULL);
    }
    
    ~JNIEnvManager() {
        env->ExceptionClear();
        if (envStat != JNI_OK) jvm->DetachCurrentThread();
    }
    
    JNIEnv* operator ->() {
        return env;
    }
    
private:
    JNIEnv* env;
    JavaVM* jvm;
    int envStat;
};

class JNIWrapper{
public:
    
    JNIWrapper();
    ~JNIWrapper();

    bool setCallback(JNIEnv* env, jobject javaCallbackObj);
    
    void setCallbackInterface();
    
    void setNumThread(int numThreads);

    inline void test() {
        callback->updateGazeInferenceTime(3.14159265F);
        callback->clearText();
        callback->appendText("\nThis");
        callback->appendText("\nIs");
        callback->appendText("\nCallback");
        callback->updateGeneratorInferenceTime(1.1);
        callback->updateRegressorInferenceTime(2.2);
    }
    
    void run();
    
    void stop();
    
    void runMain();
    
private:
    struct CallbackFields {
        struct JniCallback{
            jmethodID methodID = NULL;
            const char* name;
            const char* sig;
        };
        
        JniCallback
            updateGazeInferenceTime = {NULL, "updateGazeInferenceTime", "(F)V"},
            updateGeneratorInferenceTime = {NULL, "updateGeneratorInferenceTime", "(F)V"},
            updateRegressorInferenceTime = {NULL, "updateRegressorInferenceTime", "(F)V"},
            setText = {NULL, "setText", "(Ljava/lang/String;)V"},
            appendText = {NULL, "appendText", "(Ljava/lang/String;)V"},
            clearText = {NULL, "clearText", "()V"};
        
        std::vector<std::reference_wrapper<JniCallback>> callbacks = {
                updateGazeInferenceTime,
                updateGeneratorInferenceTime,
                updateRegressorInferenceTime,
                setText,
                appendText,
                clearText
        };
    };

    static JavaVM* jvm;
    static CallbackFields callbackFields;
    
    jobject callbackObject;
    
    class Callback: public CallbackInterface {
    public:
        Callback(jobject globalCallback);
        ~Callback();
    
        inline void updateGazeInferenceTime(float ms) override;
    
        inline void updateGeneratorInferenceTime(float ms) override;
    
        inline void updateRegressorInferenceTime(float ms) override;
        
        inline void setText(const char* string) override;
        
        inline void appendText(const char* string) override;
        
        inline void clearText() override;

    private:
        jobject instance;
        std::mutex callback_lock;
    };
    
    CallbackInterface* callback = nullptr;
    
    Controller* controller;

};


#endif //HELLO_VMEX_JNIWRAPPER_HPP
