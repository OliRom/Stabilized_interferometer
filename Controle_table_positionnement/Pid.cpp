#include "Pid.h"

Pid::Pid(float p, float i, float d, int hist_length){
  set_params(p, i, d);
  _errors.change_taille(hist_length);
}

void Pid::set_params(float p, float i, float d){
  _p = p;
  _i = i;
  _d = d;
}

void Pid::set_target(float target){
  _target = target;
}

void Pid::change_history_length(int length){
  _errors.change_taille(length, 0);
}

void Pid::set_limit(float mini, float maxi){
  if (mini != float_limit){_min = mini;}
  if (maxi != float_limit){_max = maxi;}
}

float Pid::command(float signal, bool for_info){
  if (not for_info){_errors.append(_target-signal);}
  float com = _p * _errors[0] + _i * _errors.sum() + _d * (_errors[0]-_errors[1]);
  com = max(com, _min);
  com = min(com, _max);
  return com;
}

size_t Pid::printTo(Print& p) const{
  size_t s = 0;
  float signal = _target-_errors[0];
    
  s += p.print("Cible: ");
  s += p.println(_target);
  s += p.print("Signal: ");
  s += p.println(signal);
  s += p.print("Commande: ");
  s += p.println(command(signal, true));
  s += p.print("Erreurs: ");
  s += p.print(_errors);

  return s;
}
