#ifndef IMS400CC_H
#define IMS400CC_H

#include <Arduino.h>
#include "Pid.h"
#include "PosCounter.h"

class Ims400cc : public PosCounter{
  private:
    byte _motorA_pin;  // Motor +
    byte _motorB_pin;  // Motor -
    byte _current_sens_pin;  // Miroir de courant du driver
    byte _eor_p;  // End of run +
    byte _eor_n;  // End of run -
    byte _meca_0;  // Mecanical zero
    byte _moving_led_pin;  // LED indiquant un déplacement
    Pid _PID;  // PID
    int _pid_delay;  // Délais en microsecondes entre chaque appel de commande du pid
    bool _stop_flag = false;  // Mettre à true pour arrêter la table de positionnement si elle est à une de ses extrémités

  public:
    Ims400cc(byte motorA_pin, byte motorB_pin, byte current_sens_pin, byte phaseA, byte phaseB, byte index_pulse, byte eor_p, byte eor_n, byte meca_0, byte led_pin, int pid_delay);

//    void init_pos_counter(byte phaseA, byte phaseB, byte index_pulse)
    void init_pid(float p, float i, float d, int hist_length=10);

    byte get_interrupt_pin(char pin);
    /*
    'p': end of run positive
    'n': end of run negative
    '0': mecanical zero
    */
    void stop_interruption();  // Interruption à appeler lorsque la table de positionnement est à une extrémité (eor déclenché)
    void zero_interrupt();  // Interruption à appeler lorsque le mecanical_zero est déclenché. Cette fonction n'a pas d'utilité pour l'instant et n'est donc pas définie

    void go_to(float position);
//    float get_position();
};

#endif