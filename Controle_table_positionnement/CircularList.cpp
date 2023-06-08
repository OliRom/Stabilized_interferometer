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
  
}

// Corps des fonctions non remplis à partir d'ici
template <typename T, int N>
void CircularList<T, N>::rotate(){
  
}

template <typename T, int N>
void CircularList<T, N>::rotate(int index){
  
}

template <typename T, int N>
void CircularList<T, N>::append(T value){
  
}

template <typename T, int N>
void CircularList<T, N>::insert(int index, T value){
  
}

template <typename T, int N>
void CircularList<T, N>::replace(int index, T value){
  
}

template <typename T, int N>
void CircularList<T, N>::next(){
  
}

template <typename T, int N>
void CircularList<T, N>::augmente_taille(){
  
}