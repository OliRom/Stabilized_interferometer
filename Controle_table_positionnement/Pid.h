/*
Ce fichier définit les méthodes de la classe Pid. Un exemple pour utiliser un pid de paramètres P=1, I=0.1, D=0.5 et
qui utilise les 10 dernières valeur d'erreur pour calculer le terme I est présenté ci-dessous.

* * * * *
#include Pid.h
#define read_pin 1
#define command_pin 2

Pid pid_instance(1, 0.1, 0.5, 10);

int lecture;
int commande;

void setup(){
  pinMode(read_pin, INPUT);
  pinMode(commdand_pin, OUTPUT);

  pid_instance.set_limit(0, 255);
}

void loop(){
  lecture = analogRead(read_pin);
  commande = pid_instance.command(lecture)
  analogWrite(commande);

  delay(1000);
}
* * * * *

Les méthodes de la classe sont définies plus bas.
*/

#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include "CircularList.h"
#define float_limit 3.4028235E+38

class Pid : public Printable{
  private:
    float _p;
    float _i;
    float _d;
    float _target;
    float _max = float_limit;
    float _min = -float_limit;
    //float *_error;  // https://engineerworkshop.com/blog/variables-pointers-and-indirection-in-arduino-c/
    CircularList<float, 0> _errors;  // Liste circulaire (voir le fichier CircularList.h) qui garde en mémoire les n dernières valeurs d'erreur pour calculer le terme I.

  public:
    Pid() {};
    Pid(float p, float i, float d, int hist_length=10);

    void set_params(float p, float i, float d);  // Changer les paramètres du PID
    void set_target(float target);  // Définir la cible du PID
    void change_history_length(int length);  // Changer le nombre d'erreurs gardées pour calculer le terme I du PID.
    void set_limit(float mini, float maxi);  // Borner la commande du PID. La commande restera toujours dans cet intervalle.
    
    // Retourne la commande du PID lorsque le signal est spécifié.
    // signal: signal envoyé au PID. L'erreur est calculée par le PID en utilisant la target.
    // for_info: mettre ce paramètre à true pour savoir quelle est la commande du PID pour une certaine valeur de signal sans impacter les prochaines commandes du PID. Si ce paramètre est mis à true, l'erreur calculée ne sera pas ajoutée dans la liste des erreurs passées.
    float command(float signal, bool for_info=false);

    size_t printTo(Print& p) const;  // Méthode qui est appelée avec la commande: "Serial.print(pid_instance)"
};

#endif