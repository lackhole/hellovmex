package com.example.hello_vmex.jni;

public class NativeWrapper {
    static{
        System.loadLibrary("native-lib");
    }
    
    private static long wrapperPtr = 0;
    
    
    private native long nativeInitWrapper();
    
    private native void nativeSetCallback(long wrapperPtr, CallbackInterface callback);
    
    private native void nativeRunCallback(long wrapperPtr);
    
    private native void nativeSetNumThreads(long wrapperPtr, int numThreads);
    
    private native void nativeRun(long wrapperPtr);
    
    private native void nativeStop(long wrapperPtr);
    
    private native void nativeRunMain(long wrapperPtr);
    
    
    
    public NativeWrapper() {
        wrapperPtr = nativeInitWrapper();
    }
    
    
    public void setCallback(CallbackInterface callback) {
        nativeSetCallback(wrapperPtr, callback);
    }
    
    public void runCallback(){
        nativeRunCallback(wrapperPtr);
    }
    
    public void setNumThreads(int numThreads){
        nativeSetNumThreads(wrapperPtr, numThreads);
    }
    
    public void run(){
        nativeRun(wrapperPtr);
    }
    
    public void stop(){
        nativeStop(wrapperPtr);
    }
    
    public void runMain(){
        nativeRunMain(wrapperPtr);
    }
}
