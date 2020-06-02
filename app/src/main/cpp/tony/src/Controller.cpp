//
// Created by YongGyu Lee on 2020/05/25.
//

#ifndef HELLO_VMEX_THREADCONTROLLER_CPP
#define HELLO_VMEX_THREADCONTROLLER_CPP

#include "Controller.h"

#include <chrono>
#include <sstream>
#include <utils/LogUtil.h>

#include "utils/TimeUtil.hpp"

Controller::Controller(int numThreads)
    : numThreads(numThreads), is_running(false)
{
    for(int i=0; i<numThreads; ++i){
        threadUnits.emplace_back(ThreadUnit::ThreadState::IDLE);
    }
}

Controller::~Controller() {
    for(auto& unit : threadUnits)
        unit.state = ThreadUnit::ThreadState::DEAD;
}

void Controller::setCallbackInterface(CallbackInterface *callbackInterface) {
    callback = callbackInterface;
}

bool Controller::isRunning() const {
    return is_running;
}

void Controller::run() {
    LOGD("Controller::run()");
    callback->clearText();
    for(int i=0; i<threadUnits.size(); ++i){
        threadUnits[i].alive = true;
        std::thread(&Controller::invokeModels, this, i).detach();
    }
    
    is_running = true;
    
//    invokeModels(0);
}

void Controller::stop() {
    char* str = new char[100]();
    for(int i=0; i<threadUnits.size(); ++i) {
        std::unique_lock<std::mutex> mutex_lock(threadUnits[i].lock);
        threadUnits[i].alive = false;
        mutex_lock.unlock();
    }
    is_running = false;
}

void Controller::runMain() {
    static Invoker invoker;
    static int runOnce = [](Controller& controller){
        controller.callback->clearText();
        
        std::string str = "(Main Thread) Building Model... \n";
        controller.callback->appendText(str.c_str());
        invoker.buildAll();
    
        str = "(Main Thread) Complete \n";
        controller.callback->appendText(str.c_str());
        
        str = "(Main Thread) Running \n";
        controller.callback->appendText(str.c_str());
        return 0;
    }(*this);
    
    
    
    int t = invoker.invokeGenerator();
    callback->updateGeneratorInferenceTime((float)t);
    
    t = invoker.invokeRegressor();
    callback->updateRegressorInferenceTime((float)t);
    
    t = invoker.invokeGaze();
    callback->updateGazeInferenceTime((float)t);
}

void Controller::invokeModels(int index) {
    using ThreadState = ThreadUnit::ThreadState;
    using std::string;
    
    ThreadUnit& unit = threadUnits[index];
    
    std::ostringstream address;
    address << std::this_thread::get_id();
    string str;
    
    if (!unit.invoker.built()) {
        LOGD("Controller::invokeModels() - 1");
        str = string("(") + address.str() + ") Building Model... \n";
        callback->appendText(str.c_str());
        unit.invoker.buildAll();
    
        LOGD("Controller::invokeModels() - 2");
        str = string("(") + address.str() + ") Complete \n";
        callback->appendText(str.c_str());
    }
    
    LOGD("Controller::invokeModels() - 3");
    str = string("(") + address.str() + ") Running \n";
    callback->appendText(str.c_str());
    while(unit.alive) {
        
        std::unique_lock<std::mutex> mutex_lock(unit.lock);
        unit.state = ThreadState::PROCESSING;
        mutex_lock.unlock();
        
        int t = unit.invoker.invokeGenerator();
        callback->updateGeneratorInferenceTime((float)t);
    
        t = unit.invoker.invokeRegressor();
        callback->updateRegressorInferenceTime((float)t);
    
        t = unit.invoker.invokeGaze();
        callback->updateGazeInferenceTime((float)t);
    
        
        mutex_lock.lock();
        unit.state = ThreadState::IDLE;
        mutex_lock.unlock();
    }
    
    
    str = string("(") + address.str() + ") Stop \n";
    callback->appendText(str.c_str());
}


#endif //HELLO_VMEX_THREADCONTROLLER_CPP
