//
// Created by YongGyu Lee on 2020/05/25.
//

#ifndef HELLO_VMEX_INVOKER_H
#define HELLO_VMEX_INVOKER_H

#include "cutemodel/CuteModel.hpp"
#include <vector>


class Invoker{
public:
    
    Invoker();
    Invoker(Invoker&& other);
    
    void buildAll();
    
    bool built() const;
    
    std::vector<float> invokeGaze(void* input1, void* input2, void* input3);
    
    int invokeGenerator();
    
    int invokeRegressor();
    
    int invokeGaze();
    
    
    template<typename T>
    std::vector<T> generateRandomVector(size_t n);
    
    template<typename T>
    std::vector<T> generateZeroVector(size_t n);
    
private:
    ct::CuteModel gaze;
    ct::CuteModel generator;
    ct::CuteModel regressor;
};

template<typename T>
std::vector<T> Invoker::generateRandomVector(size_t n){
    const static auto inv_rand_max = 1./RAND_MAX;
    std::vector<T> v(n);
    for(auto& elem : v) elem = T(rand() * inv_rand_max);
    return std::move(v);
}

template<typename T>
std::vector<T> Invoker::generateZeroVector(size_t n) {
    return std::move(std::vector<T>(n, 0));
}


#endif //HELLO_VMEX_INVOKER_H
