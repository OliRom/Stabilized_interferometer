#include "Arduino.h"
#include "Ims400cc.h"

Ims400cc::Ims400cc(byte motor_p_pin, byte motor_n_pin, byte current_sens_pin, byte phaseA, byte phaseB, byte index_pulse, byte eor_p, byte eor_n, byte meca_0, byte led_pin, int pid_delay, bool full_index_pulse) : PosCounter(phaseA, phaseB, index_pulse, full_index_pulse){
  _motor_p_pin = motor_p_pin;
  _motor_n_pin = motor_n_pin;
  pinMode(motor_p_pin, OUTPUT);
  pinMode(motor_n_pin, OUTPUT);
  digitalWrite(motor_p_pin, LOW);
  digitalWrite(motor_n_pin, LOW);

  _current_sens_pin = current_sens_pin;
  _eor_p = eor_p;
  _eor_n = eor_n;
  _meca_0 = meca_0;
  pinMode(current_sens_pin, INPUT);

  pinMode(eor_p, INPUT_PULLUP);
  pinMode(eor_n, INPUT_PULLUP);
  // pinMode(meca_0, INPUT_PULLUP);

  _moving_led_pin = led_pin;
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  _pid_delay = pid_delay;

  set_step_conversion(1);  // 1 micromètre de déplacement correspond à un changement de phase
}

void Ims400cc::init_pid(float p, float i, float d, int hist_length){
  _PID = Pid(p, i, d, hist_length);
  _PID.set_limit(-1.0, 1.0);
}

void Ims400cc::set_pwm_prescaler(char arduino_type){
  switch (arduino_type) {
    case 'm':
      _set_promicro_pwm_prescaler(_motor_p_pin, 0x01);
      _set_promicro_pwm_prescaler(_motor_n_pin, 0x01);
  }
}

void Ims400cc::_set_promicro_pwm_prescaler(byte pin, byte prescaler){
  switch (pin){
    case 3: TCCR0B = (TCCR0B & 0xF8) | prescaler;  // Ne fonctionne pas super bien sur cette pin
    case 5: TCCR3B = (TCCR3B & 0xF8) | prescaler;
    case 6: TCCR4B = (TCCR4B & 0xF8) | prescaler;
    case 9: TCCR1B = (TCCR1B & 0xF8) | prescaler;
    case 10: TCCR1B = (TCCR1B & 0xF8) | prescaler;
  }
}

byte Ims400cc::get_interrupt_pin(char pin){
  switch (pin){
    case 'p': return _eor_p;
    case 'n': return _eor_n;
    case '0': return _meca_0;
    case 'i': return get_I_pin();
  }
}

void Ims400cc::stop_interruption(){
  digitalWrite(_motor_p_pin, LOW);
  digitalWrite(_motor_n_pin, LOW);
  _stop_flag = true;
}

void Ims400cc::go_to(float position, float error){
  digitalWrite(_moving_led_pin, HIGH);
  digitalWrite(_motor_p_pin, HIGH);
  digitalWrite(_motor_n_pin, HIGH);

  _PID.set_target(position);
  int vitesse = 0;
  float command;
  
  float signal = get_position(2);
  float new_signal;

  while (((abs(position - signal) > error) or (vitesse > 100)) and (not _stop_flag)){
    command = _PID.command(signal) * 20;

    if (command < 0){
      digitalWrite(_motor_p_pin, LOW);
      analogWrite(_motor_n_pin, -command + 136);
    } else {
      analogWrite(_motor_p_pin, command + 136);
      digitalWrite(_motor_n_pin, LOW);
    }

    delay(_pid_delay);
    new_signal = get_position(2);
    vitesse = abs(signal - new_signal);
    signal = new_signal;
  }

  digitalWrite(_motor_p_pin, HIGH);
  digitalWrite(_motor_n_pin, HIGH);
  delay(100);  // Pour laisser le temps à la table de positionnement de s'arrêter

  digitalWrite(_moving_led_pin, LOW);
  digitalWrite(_motor_p_pin, LOW);
  digitalWrite(_motor_n_pin, LOW);

  _stop_flag = false;
}

void Ims400cc::calibrate(){
  digitalWrite(_moving_led_pin, HIGH);
  analogWrite(_motor_n_pin, 140);
  while (digitalRead(_eor_n)){}
  digitalWrite(_motor_n_pin, LOW);
  zero();
  digitalWrite(_moving_led_pin, LOW);
  delay(30000);

  digitalWrite(_moving_led_pin, HIGH);
  analogWrite(_motor_p_pin, 140);
  while (digitalRead(_eor_p)){}
  digitalWrite(_motor_p_pin, LOW);
  _max_pos = get_position(0);
  _stop_flag = false;
  digitalWrite(_moving_led_pin, LOW);
  delay(30000);

  float pos = get_position(2);
  go_to(pos/2, 10);
}
