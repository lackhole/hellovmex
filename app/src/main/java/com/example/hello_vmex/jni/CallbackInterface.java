package com.example.hello_vmex.jni;

public interface CallbackInterface {
    
    void updateGazeInferenceTime(float ms);
    
    void updateGeneratorInferenceTime(float ms);
    
    void updateRegressorInferenceTime(float ms);
    
    void setText(String string);
    
    void appendText(String string);
    
    void clearText();
}
