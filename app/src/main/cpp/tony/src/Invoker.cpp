//
// Created by YongGyu Lee on 2020/05/25.
//


#include <thread>
#include "Invoker.h"
#include "model/ModelReader.h"
#include "utils/LogUtil.h"
#include "utils/TimeUtil.hpp"


Invoker::Invoker() {}

Invoker::Invoker(Invoker &&other)
:   gaze(std::move(other.gaze)), generator(std::move(other.gaze)),
    regressor(std::move(other.regressor))
{}


void Invoker::buildAll() {
    auto buffer =  ModelReader::ReadFlippedGaze();
    gaze = ct::CuteModel(buffer.byte, buffer.size);
    gaze.setCpuNumThreads(2);
    gaze.buildInterpreter();
    if (gaze.isBuilt()){
        LOGD(" \nBuilt Gaze Model (thread %p)", std::this_thread::get_id());
        LOGD(" \n%s\n%s", gaze.summary().c_str(), gaze.summarizeOptions().c_str());
    }
    else
        LOGD(" \n\nBuilding Gaze Model has failed");
    
    buffer = ModelReader::ReadGenerator();
    generator = ct::CuteModel(buffer.byte, buffer.size);
    generator.setGpuDelegate();
//    generator.setCpuNumThreads(2);
    generator.buildInterpreter();
    if(generator.isBuilt()) {
        LOGD(" \nBuilt Generator Model (thread %p)", std::this_thread::get_id());
        LOGD(" \n%s\n%s", generator.summary().c_str(), generator.summarizeOptions().c_str());
    }
    else
        LOGD(" \n\nBuilding Generator has failed");
    
    
    buffer = ModelReader::ReadRegressor();
    regressor = ct::CuteModel(buffer.byte, buffer.size);
//    regressor.setGpuDelegate();
    regressor.setCpuNumThreads(2);
    regressor.buildInterpreter();
    if(regressor.isBuilt()) {
        LOGD(" \nBuilt Regressor Model (thread %p)", std::this_thread::get_id());
        LOGD(" \n%s\n%s", regressor.summary().c_str(), regressor.summarizeOptions().c_str());
    }
    else
        LOGD(" \n\nBuilding Regressor has failed");
}


bool Invoker::built() const {
    return gaze.isBuilt() | generator.isBuilt() | regressor.isBuilt();
}


std::vector<float> Invoker::invokeGaze(void *input1, void *input2, void *input3) {
    gaze.setInput(input1, input2, input3);
    gaze.invoke();
    return gaze.getOutput<float>(0);
}


int Invoker::invokeGenerator() {
    std::vector<char*> generator_input(generator.inputTensorCount());
    for(int i=0; i<generator.inputTensorCount(); ++i)
        generator_input[i] = new char[generator.inputTensor(i)->bytes];
    
    for(int i=0; i<generator.inputTensorCount(); ++i)
        generator.setInput(generator_input[i]);
    
    auto t_begin = getCurrentMillis();
    generator.invoke();
    auto t_end = getCurrentMillis();
    
    std::vector<float> res = generator.getOutput<float>(0);
    
    for(int i=0; i<generator.inputTensorCount(); ++i)
        delete[] generator_input[i];
    
    return (int)(t_end - t_begin);
}


int Invoker::invokeRegressor() {
    std::vector<char*> regressor_input(regressor.inputTensorCount());
    for(int i=0; i<regressor.inputTensorCount(); ++i)
        regressor_input[i] = new char[regressor.inputTensor(i)->bytes];
    
    for(int i=0; i<regressor.inputTensorCount(); ++i)
        regressor.setInput(regressor_input[i]);
    
    auto t_begin = getCurrentMillis();
    regressor.invoke();
    auto t_end = getCurrentMillis();
    
    std::vector<float> res = regressor.getOutput<float>(0);
    
    for(int i=0; i<regressor.inputTensorCount(); ++i)
        delete[] regressor_input[i];
    
    return (int)(t_end - t_begin);
}


int Invoker::invokeGaze() {
    std::vector<char*> gaze_input(gaze.inputTensorCount());
    for(int i=0; i<gaze.inputTensorCount(); ++i)
        gaze_input[i] = new char[gaze.inputTensor(i)->bytes];
    
    for(int i=0; i<gaze.inputTensorCount(); ++i)
        gaze.setInput(gaze_input[i]);
    
    auto t_begin = getCurrentMillis();
    gaze.invoke();
    auto t_end = getCurrentMillis();
    
    std::vector<float> res = gaze.getOutput<float>(0);
    
    for(int i=0; i<gaze.inputTensorCount(); ++i)
        delete[] gaze_input[i];
    
    return (int)(t_end - t_begin);
}
