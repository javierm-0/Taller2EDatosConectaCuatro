#pragma once

#include <iostream>
using namespace std;
#include "NodoConectaCuatro.h"
//clase ComputerPlayer se encarga de tratar el arbol de minimax
//se incluye la version sin la poda alfa beta donde se comprobo la enorme diferencia entre no usarla y usarla
class ComputerPlayer{
    public:
        ComputerPlayer();
        ~ComputerPlayer();
        NodoConectaCuatro* miniMax(ConectaCuatro*,int,bool);
        NodoConectaCuatro* miniMaxPodaAlfaBeta(ConectaCuatro*,int,bool,int,int,string);
};
//constructor
ComputerPlayer::ComputerPlayer(){}
//destructor
ComputerPlayer::~ComputerPlayer(){}

//WARNING: este metodo es sin poda alfa beta solo fue usado para comparar la eficiencia obtenida aplicando la poda alfa beta
//Retorna el nodo que contiene el tablero y la columna elegida al recorrer el arbol de minimax
NodoConectaCuatro* ComputerPlayer::miniMax(ConectaCuatro* estadoTablero,int profundidad, bool maximizar){
    int CPU = 2;//'2' representa las jugadas del CPU en el tablero
    int human = 1;//'1' representa las jugadas de la persona en el tablero
    
    if(estadoTablero->existeGanador(CPU)){
        //nodoTerminal: CPU gana
        NodoConectaCuatro* nodoTerminal = new NodoConectaCuatro(estadoTablero);
        return nodoTerminal;
    }
    else if(estadoTablero->existeGanador(human)){
        //nodoTerminal: CPU pierde
        NodoConectaCuatro* nodoTerminal = new NodoConectaCuatro(estadoTablero);
        return nodoTerminal;
    }

    else if(profundidad == 0){
        //si se acaba la profundidad que se deseaba explorar entonces solo retorna el nodo
        NodoConectaCuatro* nodoTerminal = new NodoConectaCuatro(estadoTablero);
        return nodoTerminal;
    }

    else if(estadoTablero->tableroLleno()){
        //si esta lleno no queda nada que hacer, debe retornar cortar la recursion y aceptar el destino
        NodoConectaCuatro* nodoTerminal = new NodoConectaCuatro(estadoTablero);
        return nodoTerminal;
    }


    if(maximizar){
        //cout<<"maximizando"<<endl;
        int columnaElegida = -1; int maximoPuntaje = -999999;
        NodoConectaCuatro* mejorTablero = NULL;
        for(int col = 0; col < estadoTablero->columnas;col++)
        {   
            bool jugadaValida = estadoTablero->ingresarFicha(col,CPU);
            if(jugadaValida)
            {
                int puntaje = miniMax(estadoTablero,profundidad-1,false)->nuevoTablero->calcularPuntajeHeuristicoDificultadImposible();
                //cout<<"puntaje actual: "<<puntaje<<endl;
                bool removeSuccess = estadoTablero->quitarFicha(col,CPU);
                if(!removeSuccess){
                    std::cout<<"ERROR: por alguna razon no se pudo quitar la ficha que se puso en la linea 66"<<endl;
                }
                if(maximoPuntaje < puntaje)
                {
                    maximoPuntaje = puntaje;
                    if(mejorTablero){
                        mejorTablero->~NodoConectaCuatro();//antes de hacerlo apuntar a otro nodo debe destruir el que ya no usare
                    }
                    mejorTablero = new NodoConectaCuatro(estadoTablero);//crea un nodo
                    mejorTablero->columnaElegida = col;//actualiza cual es la columna de la eleccion hecha entre las 7 o menos opciones
                }

            }
        }
        return mejorTablero;
    }
    else{
        //minimizar
        int columnaElegida = -1; int minimoPuntaje = 999999;
        NodoConectaCuatro* peorTablero = NULL;
        for(int col = 0; col < estadoTablero->columnas;col++)
        {
            bool jugadaValida = estadoTablero->ingresarFicha(col,human);
            if(jugadaValida){
                int puntaje = miniMax(estadoTablero,profundidad-1,true)->nuevoTablero->calcularPuntajeHeuristicoDificultadImposible();
                bool removeSuccess = estadoTablero->quitarFicha(col,human);
                if(!removeSuccess){
                    std::cout<<"ERROR: por alguna razon no se pudo quitar la ficha que se puso en la linea 97"<<endl;
                }
                if(minimoPuntaje > puntaje){
                    minimoPuntaje = puntaje;
                    if(peorTablero){
                        peorTablero->~NodoConectaCuatro();
                    }
                    peorTablero = new NodoConectaCuatro(estadoTablero);
                    peorTablero->columnaElegida = col;
                }
            }

        }
        return peorTablero;
    }
}

//minimax con poda alfa beta
//Retorna el nodo que contiene el estado del tablero con la columna elegido al recorrer el arbol de minimax
//Se incluye el string 'dificultad' con el cual se decide 
//que funcion heuristica usar para evaluar el puntaje de cada estado del tablero
//En la poda alfa beta, se tiene a alfa como el mejor para el maximizador y beta como el mejor para el minimizador
NodoConectaCuatro* ComputerPlayer::miniMaxPodaAlfaBeta(ConectaCuatro* estadoTablero,int profundidad, bool maximizar,int alfa,int beta,string dificultad){
    int CPU = 2;//'2' representa las jugadas del CPU en el tablero
    int human = 1;//'1' representa las jugadas de la persona en el tablero
    if(estadoTablero->existeGanador(CPU)){
        //nodoTerminal: CPU gana
        NodoConectaCuatro* nodoTerminal = new NodoConectaCuatro(estadoTablero);
        return nodoTerminal;
    }
    else if(estadoTablero->existeGanador(human)){
        //nodoTerminal: CPU pierde
        NodoConectaCuatro* nodoTerminal = new NodoConectaCuatro(estadoTablero);
        return nodoTerminal;
    }

    else if(profundidad == 0){
        //si se acaba la profundidad que se deseaba explorar entonces solo retorna el nodo
        NodoConectaCuatro* nodoTerminal = new NodoConectaCuatro(estadoTablero);
        return nodoTerminal;//en este tablero no ocurre nada, deberia retornar 0 su ptj
    }

    else if(estadoTablero->tableroLleno()){
        //si esta lleno no queda nada que hacer, debe retornar cortar la recursion y aceptar el destino
        NodoConectaCuatro* nodoTerminal = new NodoConectaCuatro(estadoTablero);
        return nodoTerminal;
    }


    if(maximizar){
        int columnaElegida = -1; int maximoPuntaje = -999999;
        NodoConectaCuatro* mejorTablero = NULL;
        for(int col = 0; col < estadoTablero->columnas;col++)
        {   
            bool jugadaValida = estadoTablero->ingresarFicha(col,CPU);
            
            if(jugadaValida)
            {
                int bonificacionTurno = (estadoTablero->contarCasillasVacias()+1);//cuantas mas casillas vacias hayan, menos turnos habran ocurrido haciendo preferible a esta jugada
                int puntaje;
                if(toupper(dificultad[0]) == 'E'){
                    puntaje = miniMaxPodaAlfaBeta(estadoTablero,profundidad-1,false,alfa,beta,dificultad)->nuevoTablero->calcularPuntajeHeuristicoDificultadFacil()*bonificacionTurno;
                }
                else if(toupper(dificultad[0]) == 'M'){
                    puntaje = miniMaxPodaAlfaBeta(estadoTablero,profundidad-1,false,alfa,beta,dificultad)->nuevoTablero->calcularPuntajeHeuristicoDificultadNormal()*bonificacionTurno;
                }
                else if(toupper(dificultad[0]) == 'H'){
                    puntaje = miniMaxPodaAlfaBeta(estadoTablero,profundidad-1,false,alfa,beta,dificultad)->nuevoTablero->calcularPuntajeHeuristicoDificultadDificil()*bonificacionTurno;
                }
                else{//'I'
                    puntaje = miniMaxPodaAlfaBeta(estadoTablero,profundidad-1,false,alfa,beta,dificultad)->nuevoTablero->calcularPuntajeHeuristicoDificultadImposible()*bonificacionTurno;
                }
                //si no es una hoja o terminal, llegara a maximizar o minimizar
                //donde se hara una jugada y se calculara el puntaje de esa jugada en el tablero
                //si esa jugada no llega a un terminal o al limite de profundidad 
                //entonces continua llamandose recursivamente luego de hacer otra jugada mas
                //luego de llegar a una hoja o limite de profundidad 
                //iterara para crear las 6 o menos combinaciones restantes de ese estado del tablero
                //y cada uno de ellos de la misma forma que el primero, puede llamarse recursivamente en caso que no haya llegado al terminal o al limite de profundidad
                //luego finalmente compara los puntajes donde se guardara en el 'nodo' la columna 
                //escogida debido a que fue el ptj mas alto encontrado entre los nodos de ese nivel(en maximizar)
                //en minimizar funciona similar pero se escogera el estado con el puntaje mas bajo

                bool removeSuccess = estadoTablero->quitarFicha(col,CPU);
                if(!removeSuccess){
                    std::cout<<"ERROR: por alguna razon no se pudo quitar la ficha"<<endl;
                }
                if(maximoPuntaje < puntaje)
                {
                    //cout<<"antes: "<<maximoPuntaje<<", actual(Maximizar)"<<puntaje<<endl;
                    //en maximizar, si hay un ptj mejor entonces pregunto si ya estaba apuntando a un nodo existente(el del ptj que ahora es obsoleto)
                    //le envio al destructor, y apunto al nodo cuyo ptj fue mejor
                    maximoPuntaje = puntaje;
                    if(mejorTablero){
                        mejorTablero->~NodoConectaCuatro();//antes de hacerlo apuntar a otro nodo debe destruir el que ya no usare
                    }
                    mejorTablero = new NodoConectaCuatro(estadoTablero);//crea un nodo
                    mejorTablero->columnaElegida = col;//actualiza cual es la columna de la eleccion hecha entre las 7 opciones
                    
                }
                
                if(alfa < maximoPuntaje){
                    alfa = maximoPuntaje;
                }
                if(beta <= alfa){
                    break;
                }
            }
        }
        return mejorTablero;
    }
    else{
        //minimizar
        int columnaElegida = -1; int minimoPuntaje = 999999;
        NodoConectaCuatro* peorTablero = NULL;
        for(int col = 0; col < estadoTablero->columnas;col++)
        {
            bool jugadaValida = estadoTablero->ingresarFicha(col,human);
            if(jugadaValida){
                int bonificacionTurno = (estadoTablero->contarCasillasVacias()+1);//cuantas mas casillas vacias hayan, menos turnos habran ocurrido
                int puntaje;
                if(toupper(dificultad[0]) == 'E'){
                    puntaje = miniMaxPodaAlfaBeta(estadoTablero,profundidad-1,true,alfa,beta,dificultad)->nuevoTablero->calcularPuntajeHeuristicoDificultadFacil()*bonificacionTurno;
                }
                else if(toupper(dificultad[0]) == 'M'){
                    puntaje = miniMaxPodaAlfaBeta(estadoTablero,profundidad-1,true,alfa,beta,dificultad)->nuevoTablero->calcularPuntajeHeuristicoDificultadNormal()*bonificacionTurno;
                }
                else if(toupper(dificultad[0]) == 'H'){
                    puntaje = miniMaxPodaAlfaBeta(estadoTablero,profundidad-1,true,alfa,beta,dificultad)->nuevoTablero->calcularPuntajeHeuristicoDificultadDificil()*bonificacionTurno;
                }
                else{
                    puntaje = miniMaxPodaAlfaBeta(estadoTablero,profundidad-1,true,alfa,beta,dificultad)->nuevoTablero->calcularPuntajeHeuristicoDificultadImposible()*bonificacionTurno;
                }
                bool removeSuccess = estadoTablero->quitarFicha(col,human);
                if(!removeSuccess){
                    std::cout<<"ERROR: por alguna razon no se pudo quitar la ficha"<<endl;
                }
                if(minimoPuntaje > puntaje){
                    minimoPuntaje = puntaje;
                    if(peorTablero){
                        peorTablero->~NodoConectaCuatro();
                    }
                    
                    peorTablero = new NodoConectaCuatro(estadoTablero);
                    peorTablero->columnaElegida = col;
                }
                if(beta > minimoPuntaje){
                    beta = minimoPuntaje;
                }

                if(beta <= alfa){
                    break;
                }
            }

        }
        return peorTablero;
    }
}