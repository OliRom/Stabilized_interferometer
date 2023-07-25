#ifndef POS_COUNTER_H
#define POS_COUNTER_H

#include <Arduino.h>
#include "CircularList.h"

class PosCounter{
  private:    
    byte _A_pin;
    byte _B_pin;
    byte _I_pin;

    bool _sens=true;  // Mettre à false pour changer le sens d'incrémentation du compteur
    bool _full_index_pulse;  // Mettre cette valeur à True s'il y a une interruption à chaque changement de phase et à False s'il y a une interruption au changement d'une seule des phases

    long _position = 0;  // Position en pas
    float _step_conversion;
    byte _last_phase;

  public:
    PosCounter(byte phase_A_pin, byte phase_B_pin, byte index_pin, bool full_index_pulse);

    float get_position(byte mode=0);
    /*
    Mode d'acquisition de position:
      0: retourne le nombre de phases passées
      1: retourne le nombre de tours (1/4 mode 0)
      2: retourne la position (la conversion de pas doit être spécifiée)
    */

    void phase_change();
    void set_step_conversion(float step_conversion);
    void zero();
    void direction(bool sens);
    byte read_phase();
    byte get_I_pin();
};

#endif