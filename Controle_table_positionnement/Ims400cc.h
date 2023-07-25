#ifndef IMS400CC_H
#define IMS400CC_H

#include <Arduino.h>
#include "Pid.h"
#include "PosCounter.h"

class Ims400cc : public PosCounter{
  private:
    byte _motor_p_pin;  // Motor +
    byte _motor_n_pin;  // Motor -
    byte _current_sens_pin;  // Miroir de courant du driver
    byte _eor_p;  // End of run +
    byte _eor_n;  // End of run -
    byte _meca_0;  // Mecanical zero
    byte _moving_led_pin;  // LED indiquant un déplacement
    Pid _PID;  // PID
    int _pid_delay;  // Délais en milisecondes entre chaque appel de commande du pid
    bool _stop_flag = false;  // Mettre à true pour arrêter la table de positionnement si elle est à une de ses extrémités
    long _max_pos;  // Position maximale en nombre de pas que la table peut atteindre

    void _set_promicro_pwm_prescaler(byte pin, byte prescaler);

  public:
    Ims400cc(byte motor_p_pin, byte motor_n_pin, byte current_sens_pin, byte phaseA, byte phaseB, byte index_pulse, byte eor_p, byte eor_n, byte meca_0, byte led_pin, int pid_delay, bool full_index_pulse);

    void init_pid(float p, float i, float d, int hist_length=10);

    void set_pwm_prescaler(char arduino_type);
    /* arduino_type:
      "m": pro micro
      default: None
    */

    byte get_interrupt_pin(char pin);
    /*
    'p': end of run positive
    'n': end of run negative
    '0': mecanical zero
    'i': index pulse pour l'encodeur de position
    */

    void stop_interruption();  // Interruption à appeler lorsque la table de positionnement est à une extrémité (eor déclenché)
    void zero_interrupt(){};  // Interruption à appeler lorsque le mecanical_zero est déclenché. Cette fonction n'a pas d'utilité pour l'instant et n'est donc pas définie

    void go_to(float position, float error);  // Position 
    void calibrate();
};

#endif