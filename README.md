# hellovmex
## for Tensorflow Lite Issue

* There is an callback bug with calling UI objects in the app, but please just ignore that part.


### Java
location: `app/src/main/java/con.example.hello_vmex/`

`
MainActivity.java
`
* MainActivity

`
jni/CallbackInterface.java
`
* Callback interface class for native C++ callback

`
jni/NativeWrapper
`
* Wrapper class for native C++ code


### C++
location: `app/src/main/cpp/`

`
jni/native-lib.cpp
`
* native jni code

`jni/JNIWrapper.cpp`, `jni/JNIWrapper.hpp`
* Jni Wrapper class


`
tony/include/CallbackInterface.h
`
* Callback interface helper for CallbackInterface.java

`tony/include/Controller.h`, `tony/src/Controller.cpp`
* Control thread(s)


`tony/include/Invoker.h`, `tony/src/Invoker.cpp`
* Build & run models


`
tony/tflite/cutemodel
`
* Helper class for building and invoking a Tensorflow Lite models


`tony/tflite/include`, `tony/tflite/lib`
* Tensorflow Lite headers and libraries 







