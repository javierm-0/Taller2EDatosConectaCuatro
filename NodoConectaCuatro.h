#pragma once

#include "ConectaCuatro.h"
using namespace std;

//clase NodoConectaCuatro, usado para que guarde el estado del tablero retornado
//y la columnaElegida al recorrer el arbol, recibe un ConectaCuatro
class NodoConectaCuatro{
    public:
        int columnaElegida;
        ConectaCuatro* nuevoTablero;
        NodoConectaCuatro(ConectaCuatro*);
        ~NodoConectaCuatro();
};

//constructor del nodo
NodoConectaCuatro::NodoConectaCuatro(ConectaCuatro* nuevoTablero){
    this->columnaElegida = 0;
    this->nuevoTablero = nuevoTablero;
}

//destructor del nodo(no deberia destruir el tablero que contiene ya que lo necesito para seguir jugando conectaCuatro)
NodoConectaCuatro::~NodoConectaCuatro(){}

