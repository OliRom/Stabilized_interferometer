/*
Ce fichier définit les méthodes de la classe CircularList. Les instances de CircularList sont des listes circulaires, c'est-à-dire
qu'elles n'ont ni début, ni fin comme un cercle. Elles peuvent être utilisées pour garder en mémoire les n dernières données d'un
système qui évolue dans le temps comme les 10 dernières erreurs d'un PID pour calculer le terme intégral.

Une liste circulaire peut contenir n'importe quel type de variables et être de n'importe quelle longueur.

Les méthodes de la classe sont définies plus bas.
*/

#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include <Arduino.h>
#include "Printable.h"

template <typename T, int N = 0>
class CircularList : public Printable{
  private:
    T* _elements = new T[N];  // Liste des éléments
    int _position = 0;  // Position du premier élément de la liste
    int _taille = N;  // Taille de la liste

    int _modul(int index){
      // Convertir un index qui serait hors des bornes de la liste en un index valide
      int supposed_i = _position + index;

      if (supposed_i < 0){
        int returning_i = supposed_i - (supposed_i / _taille) * _taille + _taille;
        if (returning_i == _taille){return ;}
        else {return returning_i;}
      }
      
      else {return supposed_i - (supposed_i / _taille) * _taille;}
    }

  public:
    CircularList<T, N>() {}

    // Le nième élément de la liste peut être appelé avec: "obj[n]"
    T& operator[](int index){return _elements[_modul(index)];}

    // Obtenir la position du premier élément de la liste
    int get_pos(){return _position;}

    // Incrémenter la position de "index". Tourne la liste de manière à aller vers les index les "plus vieux" si ajoutés avec append. Peut être négatif
    void rotate(int index=1){_position = _modul(index);}

    void append(T value){
      // Effacer le dernier élément de la liste et en ajouter un au début
      _elements[_modul(-1)] = value;
      rotate(-1);
    }
    
    void insert(T value, int index=0){
      // Insère l'élément à l'index "index" et décale tous les éléments à partir de cet endroit jusqu'à la fin. Efface le dernier élément
      int mod_i = _modul(index);
      if (mod_i==0){append(value);}

      else{
        for (int i = -1; _modul(i) != mod_i; i--){
          _elements[_modul(i)] = _elements[_modul(i-1)];
        }
        _elements[_modul(index)] = value;
      }
    }
    
    // Remplace l'élément à un index particulier
    void replace(T value, int index=0){_elements[_modul(index)] = value;}
    
    T next(){
      // Retourne le prochain élément. Va vers les éléments les plus vieux
      rotate();
      return _elements[_position];
    }
    
    void change_taille(int taille, T _default=0){
      // Change la taille du tableau. Si la taille est plus petite que l'ancienne, les "taille" premières valeurs sont gardées. Si la taille est plus grande que l'ancienne, les nouvelles valeurs sont _default.
      if (taille != _taille){ 
        T* new_elements = new T[taille];
        for (int i = 0; i < min(taille, _taille); i++){
          new_elements[i] = _elements[_modul(i)];
        }

        for (int i=_taille; i < taille; i++){
          new_elements[i] = _default;
        }

        delete _elements;
        _elements = new_elements;
        _taille = taille;
        _position = 0;
      }
    }
    
    T sum(){
      // Retourne la somme des éléments
      T s=_elements[0];
      for (int i=1; i<_taille; i++){
        s += _elements[i];
      }
      return s;
    }

    // Retourne le nombre d'éléments dans la liste
    int len(){return _taille;}

    size_t printTo(Print& p) const{
      // Méthode appelée avec la commande: "Serial.print(obj)"
      size_t s = 0;
      
      for (int i=0; i<_taille-1; i++){
        s += p.print(_elements[_modul(i)]);
        s += p.print(" ");
      }
      s += p.print(_elements[_modul(_taille-1)]);
      return s;
    }
};

#endif