#include "Arduino.h"
#include "Ims400cc.h"

Ims400cc::Ims400cc(byte motorA_pin, byte motorB_pin, byte current_sens_pin, byte phaseA, byte phaseB, byte index_pulse, byte eor_p, byte eor_n, byte meca_0, byte led_pin, int pid_delay) : PosCounter(phaseA, phaseB, index_pulse){
  _motorA_pin = motorA_pin;
  _motorB_pin = motorB_pin;
  pinMode(motorA_pin, OUTPUT);
  pinMode(motorB_pin, OUTPUT);
  digitalWrite(motorA_pin, LOW);
  digitalWrite(motorB_pin, LOW);

  _current_sens_pin = current_sens_pin;
  _eor_p = eor_p;
  _eor_n = eor_n;
  _meca_0 = meca_0;
  pinMode(current_sens_pin, INPUT);
  pinMode(eor_p, INPUT);
  pinMode(eor_n, INPUT);
  pinMode(meca_0, INPUT);

  _moving_led_pin = led_pin;
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  _pid_delay = pid_delay;

  set_step_conversion(1);  // 1 micromètre de déplacement correspond à un changement de phase

//  attach_limit_switch_pins(eor_p, eor_n, meca_0);
}

// void Ims400cc::init_pos_counter(byte phaseA, byte phaseB, byte index_pulse){
  
// }

void Ims400cc::init_pid(float p, float i, float d, int hist_length){
  _PID = Pid(p, i, d, hist_length);
}

byte Ims400cc::get_interrupt_pin(char pin){
  switch (pin){
    case 'p': return _eor_p;
    case 'n': return _eor_n;
    case '0': return _meca_0;
  }
}

void Ims400cc::stop_interruption(){
  digitalWrite(_motorA_pin, HIGH);
  digitalWrite(_motorB_pin, HIGH);
  _stop_flag = true;
}

void Ims400cc::go_to(float position){
  digitalWrite(_moving_led_pin, HIGH);
  digitalWrite(_motorA_pin, HIGH);
  digitalWrite(_motorB_pin, HIGH);

  _PID.set_target(position);
  int vitesse = 0;
  float command;
  
  float signal = get_position(2);

  while (((position - signal > 100) or (vitesse > 100)) and (not _stop_flag)){
    command = _PID.command(signal);
    if (command > 0){
      analogWrite(_motorA_pin, (1-command) * 1275);  // 255 * 5 = 1275
      digitalWrite(_motorB_pin, HIGH);
    } else {
      digitalWrite(_motorA_pin, HIGH);
      analogWrite(_motorB_pin, (1+command) * 1275);
    }

    delayMicroseconds(_pid_delay);
  }

  digitalWrite(_motorA_pin, HIGH);
  digitalWrite(_motorB_pin, HIGH);
  delay(100);  // Pour laisser le temps à la table de positionnement de s'arrêter

  digitalWrite(_moving_led_pin, LOW);
  digitalWrite(_motorA_pin, LOW);
  digitalWrite(_motorB_pin, LOW);

  _stop_flag = false;
}

// float Ims400cc::get_position(){
//   return _position;
// }