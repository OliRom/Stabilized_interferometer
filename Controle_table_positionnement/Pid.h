#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include "CircularList.h"

class Pid{
  private:
    float _p;
    float _i;
    float _d;
    float _target;
    float *_error;  // https://engineerworkshop.com/blog/variables-pointers-and-indirection-in-arduino-c/
    CircularList<float, ?> *_errors;

  public:
    Pid() {};
    Pid(float p, float i, float d, int hist_length=10);

    void set_params(float p, float i, float d);
    void set_target(float target);
    //void set_history_length(const int length);  // https://engineerworkshop.com/blog/variables-pointers-and-indirection-in-arduino-c/
    
    float command(float signal);
};

#endif