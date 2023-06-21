#ifndef IMS400CC_H
#define IMS400CC_H

#include <Arduino.h>
#include "Pid.h"
#include "Pos_counter.h"

class Ims400cc{
  private:
    float _position;  // Position de la table
    byte _motorA_pin;  // Motor +
    byte _motorB_pin;  // Motor -
    byte _current_sens_pin;  // Miroir de courant du driver
    byte _eor_p;  // End of run +
    byte _eor_n;  // End of run -
    byte _meca_0;  // Mecanical zero
    byte _moving_led_pin;  // LED indiquant un déplacement
    Pid _PID;  // PID

  public:
    Ims400cc() {};
    Ims400cc(byte motorA_pin, byte motorB_pin, byte current_sens_pin, byte eor_p=0, byte eor_n=0, byte meca_0=0, byte led_pin=0);

    void init_pos_counter(byte phaseA, byte phaseB, byte index_pulse)
    void init_pid(float p, float i, float d, int hist_length=10);

    void attach_limit_switch_pins(byte eor_p, byte eor_n, byte meca_0);
    void attach_moving_led_pin(byte pin);

    void go_to(float position);
    float get_position();
};

#endif