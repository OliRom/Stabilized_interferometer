#include "Arduino.h"
#include "PosCounter.h"

PosCounter* PosCounter::_ptr = nullptr;

PosCounter::PosCounter(byte phase_A_pin, byte phase_B_pin, byte index_pin) : _A_pin(phase_A_pin), _B_pin(phase_B_pin), _I_pin(index_pin) {
  pinMode(_A_pin, INPUT);
  pinMode(_B_pin, INPUT);
  pinMode(_I_pin, INPUT);

  attachInterrupt(digitalPinToInterrupt(_I_pin), _interrupt_call, RISING);

  _last_phase = read_phase();
  _ptr = this;
}

void PosCounter::_interrupt_call(){
  if (_ptr != nullptr){
    _ptr->_phase_change();
  }
}

void PosCounter::_phase_change(){
  /*
  Sens positif: B suit A ou A fuit B
  Sens négatif: B fuit A ou A suit B
  */

  byte new_phase = read_phase();
  bool cw;
  bool ccw;

  byte code = ((_last_phase &3) << 2) + (new_phase &3);

  cw = (bitRead(code, 3)^bitRead(code, 0)) && ~(bitRead(code, 2)^bitRead(code, 1));
  ccw = ~(bitRead(code, 3)^bitRead(code, 0)) && (bitRead(code, 2)^bitRead(code, 1));

  // Serial.println(code, BIN);
  // Serial.print(_last_phase, BIN);
  // Serial.print(" ");
  // Serial.println(new_phase, BIN);
  // Serial.print(cw);
  // Serial.print(" ");
  // Serial.print(ccw);
  if (cw^ccw){
    _position += (cw ^ _sens) ? +1 : -1;
  }
  // else {Serial.print(" else");}
  // Serial.println();
  // Serial.println();

  _last_phase = new_phase;

  // if (bitRead(new_phase, 0) == bitRead(_last_phase, 0)){
  //   if (bitRead(new_phase, 1) == bitRead(new_phase, 0)){  // B suit A
  //     avance = true;
  //   }
  //   else{  // B fuit A
  //     avance = false;
  //   }
  // }

  // else {
  //   if (bitRead(new_phase, 1) == bitRead(new_phase, 0)){  // A suit B
  //     avance = false;
  //   }
  //   else{  // A fuit B
  //     avance = true;
  //   }
  // }

  // _last_phase = new_phase;

  // avance = (avance ^ _sens);  // XOR (exclusive or)

  // if (avance){_position++;}
  // else {_position--;}
}

float PosCounter::get_position(byte mode){
  switch (mode){
    case 0: return (float)_position;
    case 1: return ((float)_position * 4);
    case 2: return ((float)_position * _step_conversion);
  }
}

void PosCounter::set_step_conversion(float step_conversion){_step_conversion = step_conversion;}

void PosCounter::zero(){_position = 0;}

void PosCounter::direction(bool sens){_sens = sens;}

byte PosCounter::read_phase(){
  byte phase;
  bitWrite(phase, 0, digitalRead(_A_pin));
  bitWrite(phase, 1, digitalRead(_B_pin));
  return phase;
}

byte PosCounter::get_I_pin(){
  return _I_pin;
}
