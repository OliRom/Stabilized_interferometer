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
    CircularList() {};
    T& operator[](int index);

    void rotate(int index=1);  // Incrémenter la position de "index". Tourne la liste de manière à aller vers les index les "plus vieux" si ajoutés avec append. Peut être négatif
    void append(T value);  // Effacer le dernier élément de la liste et en ajouter un au début
    void insert(int index, T value);  // Insère l'élément à l'index "index" et décale tous les éléments à partir de cet endroit jusqu'à la fin. Efface le dernier élément
    void replace(int index, T value);  // Remplace l'élément à un index particulier
    T next();  // Retourne le prochain élément. Va vers les éléments les plus vieux
    void change_taille(int taille);  // Change la taille du tableau. Si la taille est plus petite, que l'ancienne, les "taille" premières valeurs sont gardées
    T sum();  // Retourne la somme des éléments

    int size();

};

#endif