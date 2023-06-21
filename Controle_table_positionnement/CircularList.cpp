#include "Circular_list.h"

template <typename T, int N>
int CircularList<T, N>::_modul(int index){
  int supposed_i = _position + index;

  if (supposed_i < 0){
    int returning_i = supposed_i - (supposed_i / N) * N + N
    if (returning_i == N){return ;}
    else {return returning_i;}
  }
  
  else {return supposed_i - (supposed_i / N) * N;}
}

template <typename T, int N>
T& CircularList<T, N>::operator[](int index){
  return _elements[_modul(index)];
}

template <typename T, int N>
int CircularList<T, N>::size(){
  return _taille;
}

// Corps des fonctions non remplis à partir d'ici
template <typename T, int N>
void CircularList<T, N>::rotate(int index){
  _position = _modul(_position + index);
}

template <typename T, int N>
void CircularList<T, N>::append(T value){
  _elements[_modul(-1)] = value;
  _rotate(-1);
}

template <typename T, int N>
void CircularList<T, N>::insert(int index, T value){
  for (int i = -1, _modul(i) != _modul(index), i--){
    _elements[_modul(i)] = _elements[_modul(i-1)];
  }
  _elements[_modul(index)] = value;
}

template <typename T, int N>
void CircularList<T, N>::replace(int index, T value){
  _elements[_modul(index)] = value;
}

template <typename T, int N>
T CircularList<T, N>::next(){
  _rotate();
  return _elements[_modul(_position)];
}

template <typename T, int N>
void CircularList<T, N>::augmente_taille(int taille){
  T* new_elements = new T[taille];
  for (int i = 0, i < min(taille, _taille), i++){
    new_elements[i] = _elements[_modul(i)];
  }

  delete = _elements;
  _elements = new_elements;
  _taille = taille;
  _position = 0;
}

template <typename T, int N>
T CircularList<T, N>::sum(){
  T s=_elements[0];
  for (int i=1, i<_taille, i++){
    s += _elements[i];
  }
  return s;