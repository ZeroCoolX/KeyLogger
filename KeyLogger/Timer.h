#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>

class Timer{
private:
    std::thread Thread;                                                 // non main blocking thread instance
    bool Alive {false};                                                 // thread status
    std::function<void(void)> funct = nullptr;                          // reference to a function to call
    std::chrono::milliseconds interval {std::chrono::milliseconds(0)};  // interval between function calls.
    long CallCount {-1L};                                               // determine how many we want to call a certain function
    long repeat_count {-1L};                                            // count number of times a function is called from max -> 0

    void SleepAndRun(){
        std::this_thread::sleep_for(interval);
        if(Alive){
            GetFunction()();
        }
    }

    void ThreadFunc(){
        if(CallCount == INFINITY){
            while(Alive){
                this->SleepAndRun();
            }
        }else{
            while(repeat_count--){
                this->SleepAndRun();
            }
        }
    }

public:
    static const long INFINITY{-1L};

    Timer(){}

    Timer(const std::function<void(void)> &f)
    : funct(f){}

    Timer(const std::function<void(void)> &f, const unsigned long &interval, const long callCnt = Timer::INFINITY)
    : funct(f), interval(std::chrono::milliseconds(interval)), CallCount(callCnt){}

    void Start(bool async = true){
        if(IsAlive()){
            return;
        }
        Alive = true;
        repeat_count = CallCount;
        if(async){
            Thread = std::thread(ThreadFunc, this);
        }else{
            this->ThreadFunc();
        }
    }

    void Stop(){
        Alive = false;
        Thread.join();
    }

    void SetFunction(const std::function<void(void)> &f){
        funct = f;
    }

    bool IsAlive() const { return Alive; }

    long GetRepeatCount() const { return CallCount; }
    void SetRepeatCount(const long rptCnt){
        if(Alive){ return; }
        CallCount = rptCnt;
    }
    long GetRemainingCount() const {return repeat_count;}

    unsigned long GetInterval() const {return interval.count();}
    void SetInterval(const unsigned long &intv){
        if(Alive){return;}
        interval = std::chrono::milliseconds(intv);
    }

    const std::function<void(void)> &GetFunction() const {return funct;}
};

#endif // TIMER_H
