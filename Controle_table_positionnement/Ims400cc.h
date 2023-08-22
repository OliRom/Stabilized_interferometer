/*
Ce fichier définit les méthodes de la classe Ims400cc. Les instances de Ims400cc permettent de contrôler aisément une table de
positionnement IMS400CC à l'aide d'un Arduino lorsque les branchements sont bien faits (voir documentation de la table de
positionnement).

Les méthodes de la classe sont définies plus bas.
*/

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
    float _max_pos;  // Position maximale en nombre de pas que la table peut atteindre

    // Augmente la fréquence du PWM d'un Arduino pro micro pour éviter que le moteur voit une tension alternative
    // Voir la méthode "set_pwm_prescaler"
    void _set_promicro_pwm_prescaler(byte pin, byte prescaler);

  public:
    Ims400cc(byte motor_p_pin, byte motor_n_pin, byte current_sens_pin, byte phaseA, byte phaseB, byte index_pulse, byte eor_p, byte eor_n, byte meca_0, byte led_pin, int pid_delay, bool full_index_pulse);

    // Initialisation du PID qui contrôle la puissance envoyée au moteur. 
    // hist_length": le nombre d'erreurs gardées en mémoire pour calculer le terme intégral du PID
    void init_pid(float p, float i, float d, int hist_length=10);

    // Ajuste la fréqence du PWM du Arduino pour éviter que le moteur voit une tension alternative.
    // arduino_type: type du arduino utilisé
    // * La méthode a seulement été implémentée pour un Arduino pro micro. Pour ajouter d'autres types de Arduino, il faut ajouter des cases dans le switch/case
    //   dans l'implémentation de la méthode dans le fichier .cpp. Il faut aussi ajouter une méthode privée à appeler pour chaque nouveau type de Arduino. Se
    //   fier à la méthode _set_promicro_pwm_prescaler.
    void set_pwm_prescaler(char arduino_type);
    /* arduino_type:
      "m": pro micro
      default: None
    */

    // Obtenir le numéro de la pin sur laquelle une certaine interruption est attachée.
    // pin: signal auquel on veut le numéro de la pin qui le monitore
    byte get_interrupt_pin(char pin);
    /*
    'p': end of run positive
    'n': end of run negative
    '0': mecanical zero
    'i': index pulse pour l'encodeur de position
    */

    void stop_interruption();  // Interruption à appeler lorsque la table de positionnement est à une extrémité (eor déclenché)
    void zero_interrupt(){};  // Interruption à appeler lorsque le mecanical_zero est déclenché. Cette fonction n'a pas d'utilité pour l'instant et n'est donc pas définie

    // Positionne la table de positionnement à une certaine place avec une erreur maximale
    // position: position (um). La position est spécifiée dans le mode 2 de get_position() du PosCounter
    // error: erreur maximale permise (um). La table ne va jamais arrêter de se corriger tant que l'erreur n'est pas respectée
    void go_to(float position, float error);

    // Effectue une calibration de la table de positionnement
    // vitesse: vitesse à laquelle la table fait sa calibration
    void calibrate(int vitesse=144);

    // Retourne la position maximale à laquelle la table peut aller
    float get_max_pos();
};

#endif