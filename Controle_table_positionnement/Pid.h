#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include "CircularList.h"
#define float_limit 3.4028235E+38

class Pid : public Printable{
  private:
    float _p;
    float _i;
    float _d;
    float _target;
    float _max = float_limit;
    float _min = -float_limit;
    //float *_error;  // https://engineerworkshop.com/blog/variables-pointers-and-indirection-in-arduino-c/
    CircularList<float, 0> _errors;

  public:
    Pid() {};
    Pid(float p, float i, float d, int hist_length=10);

    void set_params(float p, float i, float d);
    void set_target(float target);
    void change_history_length(int length);  // https://engineerworkshop.com/blog/variables-pointers-and-indirection-in-arduino-c/
    void set_limit(float mini=float_limit, float maxi=float_limit);
    
    float command(float signal, bool for_info=false);  // Utiliser le paramètre "for_info" pour ne pas ajouter le signal à l'historique des erreurs

    size_t printTo(Print& p) const;
};

#endif