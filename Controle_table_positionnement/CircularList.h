#ifndef CIRCULAR_LIST_H
#define CIRCULAT_LIST_H

#include <Arduino.h>

template <typename T, int N = 0>
class CircularList{
  private:
    T* _elements = new T[N];
    int _position = 0;
    int _modul(int index);  // Convertir un index qui serait hors des bornes de la liste en un index valide
    int _taille = N;

  public:
    T& operator[](int index);

    void rotate();  // Incrémenter la position de 1. Tourne la liste de manière à aller vers les index les "plus vieux" si ajoutés avec append
    void rotate(int index);  // Incrémenter la position de "index". Peut être négatif
    void append(T value);  // Effacer le dernier élément de la liste et en ajouter un au début
    void insert(int index, T value);  // Insère l'élément à l'index "index" et décale tous les éléments à partir de cet endroit jusqu'à la fin. Efface le dernier élément
    void replace(int index, T value);  // Remplace l'élément à un index particulier
    T next();  // Retourne le prochain élément. Va vers les éléments les plus vieux
    void augmente_taille(int taille);  // Augmenter la taille du tableau

    int size();

};

#endif