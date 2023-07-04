#include "Ims400cc.h"

Ims400cc::Ims400cc(byte motorA_pin, byte motorB_pin, byte current_sens_pin, byte eor_p, byte eor_n, byte meca_0, byte led_pin){
  _motorA_pin = motorA_pin;
  _motorB_pin = motorB_pin;
  _current_sens_pin = current_sens_pin;

  attach_limit_switch_pins(eor_p, eor_n, meca_0);
  attach_moving_led_pin(led_pin);
}

void Ims400cc::init_pos_counter(byte phaseA, byte phaseB, byte index_pulse){
  
}

void Ims400cc::init_pid(float p, float i, float d, int hist_length){

}

void Ims400cc::attach_limit_switch_pins(byte eor_p, byte eor_n, byte meca_0){
  _eor_p = eor_p;
  _eor_n = eor_n;
  _meca_0 = meca_0;
}

void Ims400cc::attach_moving_led_pin(byte pin){
  _moving_led_pin = pin;
}

void Ims400cc::go_to(float position){

}

float Ims400cc::get_position(){
  return _position;
}