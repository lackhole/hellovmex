//
// Created by YongGyu Lee on 2020/05/25.
//


#include "CallbackInterface.h"
#include "Invoker.h"

#include <vector>
#include <thread>

class Controller{
public:
    
    struct ThreadUnit{
        enum class ThreadState {
            DEAD,
            PROCESSING,
            IDLE
        } state;
        
        bool alive;
        std::mutex lock;
        std::condition_variable conditionVariable;
        Invoker invoker;
        
        ThreadUnit(ThreadState state) : state(state), alive(true) {}
        ThreadUnit(ThreadUnit&& other)
            : state(other.state), invoker(std::move(other.invoker)), alive(other.alive){
            
        }
    };
    
    
    
    Controller(int numThreads);
    ~Controller();
    
    void setCallbackInterface(CallbackInterface* callbackInterface);
    
    void run();
    void stop();
    bool isRunning() const;
    
    void runMain();
    
private:
    
    void invokeModels(int index);
    
    int numThreads;
    CallbackInterface* callback;
    std::vector<ThreadUnit> threadUnits;
    bool is_running;
    
};


