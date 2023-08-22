/*
Ce fichier définit les méthodes de la classe PosCounter. Les instances de PosCounter sont associées à des pins du Arduino
qui monitorent les signaux de phase d'un encodeur de position et les converti en différentes unités à la demande de
l'utilisateur.

Les méthodes de la classe sont définies plus bas.
*/

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
    float _step_conversion;  // Variable qui contient le ratio unité_arbitraire/phase. Il y a 4 phases par tour pour 2 signaux de phase.
    byte _last_phase;  // Dernière phase mesurée

  public:
    PosCounter(byte phase_A_pin, byte phase_B_pin, byte index_pin, bool full_index_pulse);

    // Obtenir la position
    // mode: spécifie l'unité utilisée pour retourner la valeur
    float get_position(byte mode=0);
    /*
    Mode d'acquisition de position:
      0: retourne le nombre de phases passées
      1: retourne le nombre de tours (la valeur retournée est 1/4 de celle du mode 0)
      2: retourne la position (la conversion de pas (_step_conversion) doit être spécifiée)
    */

    void phase_change();  // Méthode appelée à chaque fois qu'il y a une interruption lors d'un changement de phase
    void set_step_conversion(float step_conversion);  // Spécifier le ration unité_arbitraire/phase
    void zero();  // Remettre le compteur à zéro
    void direction(bool sens);  // Inverser le sens d'incrémentation du compteur
    byte read_phase();  // Lire la phase actuelle de l'encodeur de position
    byte get_I_pin();  // Obtenir la pin sur laquelle l'interruption de phase est attachée
};

#endif