//
// Created by YongGyu Lee on 2020/05/21.
//

#ifndef HELLO_VMEX_CALLBACKINTERFACE_H
#define HELLO_VMEX_CALLBACKINTERFACE_H

#include <thread>

class CallbackInterface {
public:
    
    virtual void updateGazeInferenceTime(float ms) = 0;
    
    virtual void updateGeneratorInferenceTime(float ms) = 0;
    
    virtual void updateRegressorInferenceTime(float ms) = 0;
    
    virtual void setText(const char* string) = 0;
    
    virtual void appendText(const char* string) = 0;
    
    virtual void clearText() = 0;
    
};

#endif //HELLO_VMEX_CALLBACKINTERFACE_H
