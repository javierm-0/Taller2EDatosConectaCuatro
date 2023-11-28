#pragma once

#include <iostream>

using namespace std;

#include <fstream>
#include <sstream>
#include <string>

//la clase ConectaCuatro contiene metodos necesarios para:
//corroborar jugadas validas
//determinar un ganador
//mostrar el estado del tablero
//guardar en un csv el tablero (guardar partida)
//opcion de cargar partida o empezar una nueva
class ConectaCuatro{
    public:
        static const int filas = 6;//El tablero de conectaCuatro tiene 6 filas
        static const int columnas = 7;//El tablero de conectaCuatro tiene 7 columnas
        int tablero[filas][columnas];
        
        ConectaCuatro();
        ConectaCuatro(int antiguoTablero[filas][columnas]);
        ~ConectaCuatro();
        string verTablero();
        string verTableroConMejorFormato();
        
        bool ingresarFicha(int,int);
        bool quitarFicha(int,int);
        bool Consecutivos(int,int,int,int,int,int);
        bool existeGanador(int);
        void guardarPartida();
        bool soloDigitos(string);
        void cargarPartida();
        bool tableroLleno();
        int calcularPuntajeHeuristicoDificultadImposible();
        int contarCasillasVacias();
        int calcularPuntajeHeuristicoDificultadFacil();
        int calcularPuntajeHeuristicoDificultadNormal();
        int calcularPuntajeHeuristicoDificultadDificil();
        void limpiarTablero();
};

//constructor de un nuevo tablero
ConectaCuatro::ConectaCuatro(){
    for(int fil = 0;fil < filas;fil++){
        for(int col = 0; col < columnas;col++){
            this->tablero[fil][col] = 0;
        }
    }
}

//constructor del tablero, sera usado para construir los distintos estados del tablero
ConectaCuatro::ConectaCuatro(int antiguoTablero[filas][columnas]){
    for(int fil = 0;fil < filas;fil++){
        for(int col = 0; col < columnas;col++){
            this->tablero[fil][col] = antiguoTablero[fil][col];
        }
    }
}

//destructor del tablero ConectaCuatro(usado durante la liberacion del exceso de nodos despues del turno)
ConectaCuatro::~ConectaCuatro(){}

//retorna un string con el formato completo del tablero actual
string ConectaCuatro::verTablero(){
    string r = "";
    for(int fil = 0; fil < filas;fil++){
        for(int col = 0; col < columnas;col++){
            r += to_string(this->tablero[fil][col])+" ";
        }
        r += "\n";
    }
    return r;
}

//si se logra una jugada valida retorna true
bool ConectaCuatro::ingresarFicha(int col,int jugador){
    for(int fil = filas-1; fil >= 0;fil--){
        if(this->tablero[fil][col] == 0){
           this->tablero[fil][col] = jugador;
           return true;
        }
    }
    return false;
}

//si al menos un char no es un numero retorna false
//sino retorna true y ahora puedo convertirlo a int de manera segura
bool ConectaCuatro::soloDigitos(string numero){
    for(int i = 0;i < numero.size();i++){
        if(!isdigit(numero[i])) return false;
    }
    return true;
}


//retorna false si alguno de los 4 movimientos no es igual al jugador
//retorna false si no hay espacio para corroborar 4 celdas hacia esa direccion
//si no se demuestra ninguno de estos 2 entonces retorna true
//este metodo sera llamado para ver si en alguna de las celdas de la matriz(tablero) existen 4 iguales consecutivos('1' o '2')
bool ConectaCuatro::Consecutivos(int fil,int col, int jugador, int filActivar, int colActivar,int repeticiones){
    for(int i = 0;i < repeticiones;i++){
        int filC = fil+i*filActivar;//si es '0' fija ese eje
        int colC = col+i*colActivar;//y si es '1' avanzara, '-1' invierte el sentido del avance
        if(filC >= 0 && filC < filas 
        && colC >= 0 && colC < columnas){
            //se llega a sumar hasta 4 por lo que se debe corroborar que no se exceda el tamanio
            //como se restara por 4 se debe corroborar que la resta no avance a menos que el numero 0
            if(this->tablero[filC][colC] != jugador) return false;
        }
        else return false;//si fil o col no entran en rango return false
    }
    return true;
}

//retorna true si existe al menos una recta de 4 celdas iguales y distintas de 0 en cualquier direccion
//retorna false si no se cumple ninguna condicion en ninguna celda de todo el tablero
//int jugador:
//'1' jugador
//'2' CPU
bool ConectaCuatro::existeGanador(int jugador){
    for(int fil = 0; fil < filas;fil++){
        for(int col = 0; col < columnas;col++){
            if(Consecutivos(fil,col,jugador,1,1,4)) return true;//activa mov hacia abajo y hacia la derecha(diagonal inf derecha)
            if(Consecutivos(fil,col,jugador,1,-1,4)) return true;//avanza hacia abajo y hacia la izquierda(diagonal inf izquierda)
            if(Consecutivos(fil,col,jugador,1,0,4)) return true;//activa movimientos hacia abajo y fija el eje X(fil)
            if(Consecutivos(fil,col,jugador,0,1,4)) return true;//fija el eje Y(de las filas) y activa mov hacia la derecha(suma Col)
        }
    }
    return false;
}

//en un string guarda la matriz del tablero
//asigna un nombre de archivo que si ya existe sera sobreescrito
//y finalmente le pasa el string a dicho archivo
void ConectaCuatro::guardarPartida(){
    string r = this->verTablero();
    string nombreArchivo = "tablero.csv";
    ofstream archivo(nombreArchivo);
    if(archivo.is_open()){
        archivo<<r;
        archivo.close();
    }
    else std::cout<<"error inesperado en el guardado de partida..."<<endl;
}

//si existe el tablero.csv se lo pasa a la matriz de esta clase
void ConectaCuatro::cargarPartida(){
    ifstream archivo("tablero.csv");
    if(!archivo.is_open()) {
        std::cout<<"error inesperado"<<endl; return;
    }
    string linea;
    int fil = 0; int col;

    while(getline(archivo,linea) && fil < filas){
        istringstream split(linea);
        string dato;
        for(int col = 0; col < columnas; col++){
            getline(split,dato,' ');
            this->tablero[fil][col] = stoi(dato);
        }
        fil++;
    }
}

//si encuentra un '0' (casilla vacia) retorna false
//true sino
bool ConectaCuatro::tableroLleno(){
    for(int fil = 0;fil < filas;fil++){
        for(int col = 0; col < columnas;col++){
            if(this->tablero[fil][col] == 0) return false;
        }
    }
    return true;
}

//recibe la columna y el jugador
//busca desde la fila mas alta(0) y pregunta por la ficha del jugador
//al encontrarlo reemplaza con un '0' que es el valor de la casilla vacia y retorna true
//si no se encuentra la ficha dentro de las filas de esa columna especifica retorna false
bool ConectaCuatro::quitarFicha(int col,int jugador){
    for(int fil = 0; fil < filas; fil++){
        if(this->tablero[fil][col] == jugador){//encuentra la ficha del jugador
           this->tablero[fil][col] = 0;//y la quita, representando una casilla vacia con '0'
           return true;//todo bien
        }
    }
    std::cout<<"error: no se encontro la ficha a quitar"<<endl;//algo malo paso
    return false;
}

//Retorna un entero que representa que tan beneficioso es el estado actual para el CPU
//si ya existe ganador retorna el puntaje de victoria derrota o empate
//sino entonces evalua el estado actual y otorga puntaje para distintos casos
int ConectaCuatro::calcularPuntajeHeuristicoDificultadImposible(){
    int human = 1; int CPU = 2;
    int puntaje = 0;
    if(this->existeGanador(human)) return -3000;
    if(this->existeGanador(CPU)) return 3000;
    if(this->tableroLleno()) return 0;

    const int prioridadBaja = 1;//1 pts
    const int prioridadMedia = 5;//5 pts
    const int prioridadAlta = 10;//10 pts

    //el juego continua
    //se evalua la situacion segun que tan cerca este de ganar
    //o segun estrategias que en la mayor de las veces esten con mas posibilidades de ganar
    //el puntaje positivo representa que tan beneficioso es para el CPU
    //el puntaje negativo representa que tan opuesto es para el CPU
    for(int fil = 0; fil < this->filas;fil++){
        if(this->tablero[fil][3] == CPU){//preferencia al centro del tablero
            puntaje += prioridadAlta;
        }

        if(this->tablero[fil][3] == human){
            puntaje -= prioridadAlta;
        }

        if(this->tablero[fil][2] == CPU) puntaje += prioridadMedia;
        if(this->tablero[fil][4] == CPU) puntaje += prioridadMedia;
        if(this->tablero[fil][1] == CPU) puntaje += prioridadBaja;
        if(this->tablero[fil][5] == CPU) puntaje += prioridadBaja;

        if(this->tablero[fil][2] == human) puntaje -= prioridadMedia;
        if(this->tablero[fil][4] == human) puntaje -= prioridadMedia;
        if(this->tablero[fil][1] == human) puntaje -= prioridadBaja;
        if(this->tablero[fil][5] == human) puntaje -= prioridadBaja;
        
        for(int col = 0; col < this->columnas;col++){
            //defensa del CPU
            if(this->Consecutivos(fil,col,human,1,0,3)) //3 vertical(abajo) persona
            {
                if(fil < this->filas-1){
                    if(tablero[fil-1][col] == CPU){
                        puntaje += 300;//bloquea la jugada y recupera puntaje
                    }
                    else{
                        puntaje -= 300;
                    }
                }
            }
            
            if(this->Consecutivos(fil,col,human,0,1,2) && col > 0){//la casilla detectada parte en [fil][col > 0] como minimo
                if(tablero[fil][col-1] == CPU){
                    puntaje += 100;//bloquea movimientos horizontales simples(izq)
                }
                else{
                    puntaje -= 100;
                }
            }

            if(this->Consecutivos(fil,col,human,0,1,2) && col+2 < this->columnas){//la casilla detectada parte en[fil][col+2 < columnas], col:4,3,2,1,...
                if(tablero[fil][col+2] == CPU){
                    puntaje += 100;//bloquea movimientos horizontales simples(der)
                }
                else{
                    puntaje -= 100;
                }
            }

            
            if(this->Consecutivos(fil,col,human,1,1,2))
            {   
                if(fil-1 >= 0 && col-1 >= 0){//si estuviese cerca del tope no deberia penalizar
                    if(tablero[fil-1][col-1] == CPU){
                        puntaje += 200;
                    }
                    else puntaje -= 200;
                }
                
                if(fil+2 < this->filas-1 && col+2 < this->columnas){
                    if(tablero[fil+2][col+2] == CPU){
                        puntaje += 200;
                    }
                    else{
                        puntaje -= 200;
                    }
                }
                
            }

            if(this->Consecutivos(fil,col,human,1,-1,2))
            {
                if(fil+2 < this->filas && col-2 >= 0){
                    if(tablero[fil+2][col-2] == CPU){//tapar por abajo
                        puntaje += 200;
                    }
                    else{
                        puntaje -= 200;
                    }
                }
                if(fil-1 >=0 && col+1 < this->columnas){
                    if(tablero[fil-1][col+1] == CPU){//tapar por arriba
                        puntaje += 200;
                    }
                    else puntaje -= 200;
                }
            }

            if(this->Consecutivos(fil,col,human,1,1,3)){
                puntaje -= prioridadAlta*5;
            } 
            else if(this->Consecutivos(fil,col,human,1,1,2)){
                puntaje -= prioridadMedia*5;
            } 
            else{
                puntaje += prioridadAlta*5;
            }

            //jugadas de la persona
            if(this->Consecutivos(fil,col,human,1,1,3))//3 diagonal persona
            {
                puntaje -= prioridadAlta*10;
            }
            if(this->Consecutivos(fil,col,human,1,-1,3))//3 diagonal op persona
            {
                puntaje -= prioridadAlta*10;
            }
            
            if(this->Consecutivos(fil,col,human,1,0,2))//2 vertical persona
            {
                puntaje -= prioridadBaja*5;
            }
            if(this->Consecutivos(fil,col,human,0,1,2))//2 horizontal persona
            {
                puntaje -= prioridadMedia*5;
            }
            if(this->Consecutivos(fil,col,human,1,1,2))//2 diagonal  persona
            {
                puntaje -= prioridadAlta*2;
            }
            if(this->Consecutivos(fil,col,human,1,-1,2))//2 diagonal op persona
            {
                puntaje -= prioridadAlta*2;
            }

            
            //jugadas del CPU
            if(this->Consecutivos(fil,col,CPU,1,0,3))//3 vertical CPU
            {
                puntaje += prioridadBaja*2;//es una jugada predecible
            }
            if(this->Consecutivos(fil,col,CPU,0,1,3))//3 horizontal CPU
            {
                puntaje += prioridadMedia;//podria no ser tan predecible
            }
            if(this->Consecutivos(fil,col,CPU,1,1,3))//3 diagonales CPU
            {
                puntaje += prioridadAlta;//las diagonales son mas elaboradas
            }
            if(this->Consecutivos(fil,col,CPU,1,-1,3)){
                puntaje += prioridadAlta;
            }

            
            if(this->Consecutivos(fil,col,CPU,1,0,2))//2 vertical CPU
            {
                puntaje += prioridadBaja;//es una jugada predecible
            }
            if(this->Consecutivos(fil,col,CPU,0,1,2))//2 horizontal CPU
            {
                puntaje += prioridadMedia;//podria no ser tan predecible
            }
            if(this->Consecutivos(fil,col,CPU,1,1,2))//2 diagonal CPU
            {
                puntaje += prioridadAlta;//las diagonales son mas elaboradas
            }
            if(this->Consecutivos(fil,col,CPU,1,-1,2)){//2 diagonal opuesta CPU
                //diagonales
                puntaje += prioridadAlta;
            }

        }
    }
    
    //cout<<"Puntaje heuristico: "<<puntaje<<" pts"<<endl;
    return puntaje;
}

//retorna un string con un formato que permita una interfaz mas amigable para la vista al momento de jugar ConectaCuatro
string ConectaCuatro::verTableroConMejorFormato(){

    string r = "";
    for(int fil = 0; fil < this->filas;fil++){
        for(int col = 0; col < this->columnas;col++){
            if(this->tablero[fil][col] == 1)//jugador 'X'
            {
                r += "X";
            }
            else if(this->tablero[fil][col] == 2)//CPU 'O'
            {
                r += "O";
            }
            else{
                r += "'";
            }
            r += "    ";
        }
        r +="\n\n";
    }
    r += "1    2    3    4    5    6    7";
    return r;
}

//retorna la cantidad de casillas que no han sido ocupadas aun
//Esta funcion es usada para obtener un multiplicador del puntaje para que se prefieran nodos mas recientes
int ConectaCuatro::contarCasillasVacias(){
    int vacios = 0;
    for(int fil = 0; fil < this->filas;fil++){
        for(int col = 0; col < this->columnas;col++){
            if(this->tablero[fil][col] == 0){//si es casilla vacia
                vacios++;//acumula
            }
        }
    }
    return vacios;
}

//retorna un entero que representa el puntajeHeuristico para el CPU
//se retiran las funciones de defensa diagonal para 2 fichas diagonales
//se asigna menor puntaje a las funciones de defensa general resultando en una baja de prioridad
int ConectaCuatro::calcularPuntajeHeuristicoDificultadFacil(){
    int human = 1; int CPU = 2;
    int puntaje = 0;
    if(this->existeGanador(human)) return -3000;
    if(this->existeGanador(CPU)) return 3000;
    if(this->tableroLleno()) return 0;

    const int prioridadBaja = 1;//1 pts
    const int prioridadMedia = 5;//5 pts
    const int prioridadAlta = 10;//10 pts

    //el juego continua
    //se evalua la situacion segun que tan cerca este de ganar
    //o segun estrategias que en la mayor de las veces esten con mas posibilidades de ganar
    //el puntaje positivo representa que tan beneficioso es para el CPU
    //el puntaje negativo representa que tan opuesto es para el CPU
    for(int fil = 0; fil < this->filas;fil++){
        if(this->tablero[fil][3] == CPU){//preferencia al centro del tablero
            puntaje += prioridadBaja;
        }

        if(this->tablero[fil][3] == human){
            puntaje -= prioridadBaja;
        }

        if(this->tablero[fil][2] == CPU) puntaje += prioridadMedia;
        if(this->tablero[fil][4] == CPU) puntaje += prioridadMedia;
        if(this->tablero[fil][1] == CPU) puntaje += prioridadAlta;
        if(this->tablero[fil][5] == CPU) puntaje += prioridadAlta;

        if(this->tablero[fil][2] == human) puntaje -= prioridadMedia;
        if(this->tablero[fil][4] == human) puntaje -= prioridadMedia;
        if(this->tablero[fil][1] == human) puntaje -= prioridadBaja;
        if(this->tablero[fil][5] == human) puntaje -= prioridadBaja;
        
        for(int col = 0; col < this->columnas;col++){
            //defensa del CPU
            if(this->Consecutivos(fil,col,human,1,0,3)) //3 vertical(abajo) persona
            {
                if(fil < this->filas-1){
                    if(tablero[fil-1][col] == CPU){
                        puntaje += prioridadMedia;//bloquea la jugada y recupera puntaje
                    }
                    else{
                        puntaje -= prioridadMedia;
                    }
                }
            }
            
            if(this->Consecutivos(fil,col,human,0,1,2) && col > 0){//la casilla detectada parte en [fil][col > 0] como minimo
                if(tablero[fil][col-1] == CPU){
                    puntaje += prioridadBaja;//bloquea movimientos horizontales simples(izq)
                }
                else{
                    puntaje -= prioridadBaja;
                }
            }

            if(this->Consecutivos(fil,col,human,0,1,2) && col+2 < this->columnas){//la casilla detectada parte en[fil][col+2 < columnas], col:4,3,2,1,...
                if(tablero[fil][col+2] == CPU){
                    puntaje += prioridadBaja;//bloquea movimientos horizontales simples(der)
                }
                else{
                    puntaje -= prioridadBaja;
                }
            }
            if(this->Consecutivos(fil,col,human,1,1,3)){
                puntaje -= prioridadBaja;
            } 
            else if(this->Consecutivos(fil,col,human,1,1,2)){
                puntaje -= prioridadMedia;
            } 
            else{
                puntaje += prioridadBaja;
            }
            //jugadas de la persona
            if(this->Consecutivos(fil,col,human,1,1,3))//3 diagonal persona
            {
                puntaje -= prioridadAlta*5;
            }
            if(this->Consecutivos(fil,col,human,1,-1,3))//3 diagonal op persona
            {
                puntaje -= prioridadAlta*5;
            }
            
            if(this->Consecutivos(fil,col,human,1,0,2))//2 vertical persona
            {
                puntaje -= prioridadBaja*5;
            }
            if(this->Consecutivos(fil,col,human,0,1,2))//2 horizontal persona
            {
                puntaje -= prioridadMedia*5;
            }
            if(this->Consecutivos(fil,col,human,1,1,2))//2 diagonal  persona
            {
                puntaje -= prioridadAlta;
            }
            if(this->Consecutivos(fil,col,human,1,-1,2))//2 diagonal op persona
            {
                puntaje -= prioridadAlta;
            }

            
            //jugadas del CPU
            if(this->Consecutivos(fil,col,CPU,1,0,3))//3 vertical CPU
            {
                puntaje += prioridadBaja*2;
            }
            if(this->Consecutivos(fil,col,CPU,0,1,3))//3 horizontal CPU
            {
                puntaje += prioridadMedia*2;
            }
            if(this->Consecutivos(fil,col,CPU,1,1,3))//3 diagonales CPU
            {
                puntaje += prioridadBaja*2;
            }
            if(this->Consecutivos(fil,col,CPU,1,-1,3)){
                puntaje += prioridadBaja*2;
            }

            
            if(this->Consecutivos(fil,col,CPU,1,0,2))//2 vertical CPU
            {
                puntaje += prioridadMedia;//es una jugada predecible
            }
            if(this->Consecutivos(fil,col,CPU,0,1,2))//2 horizontal CPU
            {
                puntaje += prioridadMedia;//podria no ser tan predecible
            }
            if(this->Consecutivos(fil,col,CPU,1,1,2))//2 diagonal CPU
            {
                puntaje += prioridadBaja;//las diagonales son mas elaboradas
            }
            if(this->Consecutivos(fil,col,CPU,1,-1,2)){//2 diagonal opuesta CPU
                //diagonales
                puntaje += prioridadBaja;
            }

        }
    }
    return puntaje;
}

//Retorna un entero que representa que tan beneficioso es el estado actual para el CPU
//Se baja la prioridad a bloquear diagonales
int ConectaCuatro::calcularPuntajeHeuristicoDificultadNormal(){
       int human = 1; int CPU = 2;
    int puntaje = 0;
    if(this->existeGanador(human)) return -3000;
    if(this->existeGanador(CPU)) return 3000;
    if(this->tableroLleno()) return 0;

    const int prioridadBaja = 1;//1 pts
    const int prioridadMedia = 5;//5 pts
    const int prioridadAlta = 10;//10 pts

    //el juego continua
    //se evalua la situacion segun que tan cerca este de ganar
    //o segun estrategias que en la mayor de las veces esten con mas posibilidades de ganar
    //el puntaje positivo representa que tan beneficioso es para el CPU
    //el puntaje negativo representa que tan opuesto es para el CPU
    for(int fil = 0; fil < this->filas;fil++){
        if(this->tablero[fil][3] == CPU){//preferencia al centro del tablero
            puntaje += prioridadBaja;
        }

        if(this->tablero[fil][3] == human){
            puntaje -= prioridadBaja;
        }

        if(this->tablero[fil][2] == CPU) puntaje += prioridadMedia;
        if(this->tablero[fil][4] == CPU) puntaje += prioridadMedia;
        if(this->tablero[fil][1] == CPU) puntaje += prioridadBaja;
        if(this->tablero[fil][5] == CPU) puntaje += prioridadBaja;

        if(this->tablero[fil][2] == human) puntaje -= prioridadMedia;
        if(this->tablero[fil][4] == human) puntaje -= prioridadMedia;
        if(this->tablero[fil][1] == human) puntaje -= prioridadBaja;
        if(this->tablero[fil][5] == human) puntaje -= prioridadBaja;
        
        for(int col = 0; col < this->columnas;col++){
            //defensa del CPU
            if(this->Consecutivos(fil,col,human,1,0,3)) //3 vertical(abajo) persona
            {
                if(fil < this->filas-1){
                    if(tablero[fil-1][col] == CPU){
                        puntaje += prioridadAlta;//bloquea la jugada y recupera puntaje
                    }
                    else{
                        puntaje -= prioridadAlta;
                    }
                }
            }
            
            if(this->Consecutivos(fil,col,human,0,1,2) && col > 0){//la casilla detectada parte en [fil][col > 0] como minimo
                if(tablero[fil][col-1] == CPU){
                    puntaje += prioridadMedia;//bloquea movimientos horizontales simples(izq)
                }
                else{
                    puntaje -= prioridadMedia;
                }
            }

            if(this->Consecutivos(fil,col,human,0,1,2) && col+2 < this->columnas){//la casilla detectada parte en[fil][col+2 < columnas], col:4,3,2,1,...
                if(tablero[fil][col+2] == CPU){
                    puntaje += prioridadMedia;//bloquea movimientos horizontales simples(der)
                }
                else{
                    puntaje -= prioridadMedia;
                }
            }

            
            if(this->Consecutivos(fil,col,human,1,1,2))
            {   
                if(fil-1 >= 0 && col-1 >= 0){//si estuviese cerca del tope no deberia penalizar
                    if(tablero[fil-1][col-1] == CPU){
                        puntaje += prioridadAlta;
                    }
                    else puntaje -= prioridadAlta;
                }
                
                if(fil+2 < this->filas-1 && col+2 < this->columnas){
                    if(tablero[fil+2][col+2] == CPU){
                        puntaje += prioridadAlta;
                    }
                    else{
                        puntaje -= prioridadAlta;
                    }
                }
                
            }

            if(this->Consecutivos(fil,col,human,1,-1,2))
            {
                if(fil+2 < this->filas && col-2 >= 0){
                    if(tablero[fil+2][col-2] == CPU){//tapar por abajo
                        puntaje += prioridadBaja;
                    }
                    else{
                        puntaje -= prioridadBaja;
                    }
                }
                if(fil-1 >=0 && col+1 < this->columnas){
                    if(tablero[fil-1][col+1] == CPU){//tapar por arriba
                        puntaje +=prioridadAlta;
                    }
                    else puntaje -= prioridadAlta;
                }
            }

            if(this->Consecutivos(fil,col,human,1,1,3)){
                puntaje -= prioridadAlta;
            } 
            else if(this->Consecutivos(fil,col,human,1,1,2)){
                puntaje -= prioridadMedia;
            } 
            else{
                puntaje += prioridadAlta;
            }

            //jugadas de la persona
            if(this->Consecutivos(fil,col,human,1,1,3))//3 diagonal persona
            {
                puntaje -= prioridadAlta*10;
            }
            if(this->Consecutivos(fil,col,human,1,-1,3))//3 diagonal op persona
            {
                puntaje -= prioridadAlta*10;
            }
            
            if(this->Consecutivos(fil,col,human,1,0,2))//2 vertical persona
            {
                puntaje -= prioridadBaja*5;
            }
            if(this->Consecutivos(fil,col,human,0,1,2))//2 horizontal persona
            {
                puntaje -= prioridadMedia*5;
            }
            if(this->Consecutivos(fil,col,human,1,1,2))//2 diagonal  persona
            {
                puntaje -= prioridadAlta*2;
            }
            if(this->Consecutivos(fil,col,human,1,-1,2))//2 diagonal op persona
            {
                puntaje -= prioridadAlta*2;
            }

            
            //jugadas del CPU
            if(this->Consecutivos(fil,col,CPU,1,0,3))//3 vertical CPU
            {
                puntaje += prioridadBaja*2;//es una jugada predecible
            }
            if(this->Consecutivos(fil,col,CPU,0,1,3))//3 horizontal CPU
            {
                puntaje += prioridadMedia;//podria no ser tan predecible
            }
            if(this->Consecutivos(fil,col,CPU,1,1,3))//3 diagonales CPU
            {
                puntaje += prioridadAlta;//las diagonales son mas elaboradas
            }
            if(this->Consecutivos(fil,col,CPU,1,-1,3)){
                puntaje += prioridadAlta;
            }

            
            if(this->Consecutivos(fil,col,CPU,1,0,2))//2 vertical CPU
            {
                puntaje += prioridadBaja;//es una jugada predecible
            }
            if(this->Consecutivos(fil,col,CPU,0,1,2))//2 horizontal CPU
            {
                puntaje += prioridadMedia;//podria no ser tan predecible
            }
            if(this->Consecutivos(fil,col,CPU,1,1,2))//2 diagonal CPU
            {
                puntaje += prioridadAlta;//las diagonales son mas elaboradas
            }
            if(this->Consecutivos(fil,col,CPU,1,-1,2)){//2 diagonal opuesta CPU
                //diagonales
                puntaje += prioridadAlta;
            }

        }
    }
    return puntaje;
}

//Retorna un entero que representa que tan beneficioso es el estado actual para el CPU
//se baja ligeramente la prioridad a bloquear diagonales
int ConectaCuatro::calcularPuntajeHeuristicoDificultadDificil(){
       int human = 1; int CPU = 2;
    int puntaje = 0;
    if(this->existeGanador(human)) return -3000;
    if(this->existeGanador(CPU)) return 3000;
    if(this->tableroLleno()) return 0;

    const int prioridadBaja = 1;//1 pts
    const int prioridadMedia = 5;//5 pts
    const int prioridadAlta = 10;//10 pts

    //el juego continua
    //se evalua la situacion segun que tan cerca este de ganar
    //o segun estrategias que en la mayor de las veces esten con mas posibilidades de ganar
    //el puntaje positivo representa que tan beneficioso es para el CPU
    //el puntaje negativo representa que tan opuesto es para el CPU
    for(int fil = 0; fil < this->filas;fil++){
        if(this->tablero[fil][3] == CPU){//preferencia al centro del tablero
            puntaje += prioridadAlta;
        }

        if(this->tablero[fil][3] == human){
            puntaje -= prioridadAlta;
        }

        if(this->tablero[fil][2] == CPU) puntaje += prioridadMedia;
        if(this->tablero[fil][4] == CPU) puntaje += prioridadMedia;
        if(this->tablero[fil][1] == CPU) puntaje += prioridadBaja;
        if(this->tablero[fil][5] == CPU) puntaje += prioridadBaja;

        if(this->tablero[fil][2] == human) puntaje -= prioridadMedia;
        if(this->tablero[fil][4] == human) puntaje -= prioridadMedia;
        if(this->tablero[fil][1] == human) puntaje -= prioridadBaja;
        if(this->tablero[fil][5] == human) puntaje -= prioridadBaja;
        
        for(int col = 0; col < this->columnas;col++){
            //defensa del CPU
            if(this->Consecutivos(fil,col,human,1,0,3)) //3 vertical(abajo) persona
            {
                if(fil < this->filas-1){
                    if(tablero[fil-1][col] == CPU){
                        puntaje += 300;//bloquea la jugada y recupera puntaje
                    }
                    else{
                        puntaje -= 300;
                    }
                }
            }
            
            if(this->Consecutivos(fil,col,human,0,1,2) && col > 0){//la casilla detectada parte en [fil][col > 0] como minimo
                if(tablero[fil][col-1] == CPU){
                    puntaje += 100;//bloquea movimientos horizontales simples(izq)
                }
                else{
                    puntaje -= 100;
                }
            }

            if(this->Consecutivos(fil,col,human,0,1,2) && col+2 < this->columnas){//la casilla detectada parte en[fil][col+2 < columnas], col:4,3,2,1,...
                if(tablero[fil][col+2] == CPU){
                    puntaje += 100;//bloquea movimientos horizontales simples(der)
                }
                else{
                    puntaje -= 100;
                }
            }

            
            if(this->Consecutivos(fil,col,human,1,1,2))
            {   
                if(fil-1 >= 0 && col-1 >= 0){//si estuviese cerca del tope no deberia penalizar
                    if(tablero[fil-1][col-1] == CPU){
                        puntaje += prioridadAlta*5;
                    }
                    else puntaje -= prioridadAlta*5;
                }
                
                if(fil+2 < this->filas-1 && col+2 < this->columnas){
                    if(tablero[fil+2][col+2] == CPU){
                        puntaje += prioridadAlta*5;
                    }
                    else{
                        puntaje -= prioridadAlta*5;
                    }
                }
                
            }

            if(this->Consecutivos(fil,col,human,1,-1,2))
            {
                if(fil+2 < this->filas && col-2 >= 0){
                    if(tablero[fil+2][col-2] == CPU){//tapar por abajo
                        puntaje += prioridadAlta*5;//menor puntaje para bajar la prioridad a bloquear diagonales
                    }
                    else{
                        puntaje -= prioridadAlta*5;
                    }
                }
                if(fil-1 >=0 && col+1 < this->columnas){
                    if(tablero[fil-1][col+1] == CPU){//tapar por arriba
                        puntaje += prioridadAlta*5;
                    }
                    else puntaje -= prioridadAlta*5;
                }
            }

            if(this->Consecutivos(fil,col,human,1,1,3)){
                puntaje -= prioridadAlta;
            } 
            else if(this->Consecutivos(fil,col,human,1,1,2)){
                puntaje -= prioridadMedia;
            } 
            else{
                puntaje += prioridadAlta;
            }

            //jugadas de la persona
            if(this->Consecutivos(fil,col,human,1,1,3))//3 diagonal persona
            {
                puntaje -= prioridadAlta*10;
            }
            if(this->Consecutivos(fil,col,human,1,-1,3))//3 diagonal op persona
            {
                puntaje -= prioridadAlta*10;
            }
            
            if(this->Consecutivos(fil,col,human,1,0,2))//2 vertical persona
            {
                puntaje -= prioridadBaja*5;
            }
            if(this->Consecutivos(fil,col,human,0,1,2))//2 horizontal persona
            {
                puntaje -= prioridadMedia*5;
            }
            if(this->Consecutivos(fil,col,human,1,1,2))//2 diagonal  persona
            {
                puntaje -= prioridadAlta*2;
            }
            if(this->Consecutivos(fil,col,human,1,-1,2))//2 diagonal op persona
            {
                puntaje -= prioridadAlta*2;
            }

            
            //jugadas del CPU
            if(this->Consecutivos(fil,col,CPU,1,0,3))//3 vertical CPU
            {
                puntaje += prioridadBaja*2;//es una jugada predecible
            }
            if(this->Consecutivos(fil,col,CPU,0,1,3))//3 horizontal CPU
            {
                puntaje += prioridadMedia;//podria no ser tan predecible
            }
            if(this->Consecutivos(fil,col,CPU,1,1,3))//3 diagonales CPU
            {
                puntaje += prioridadAlta;//las diagonales son mas elaboradas
            }
            if(this->Consecutivos(fil,col,CPU,1,-1,3)){
                puntaje += prioridadAlta;
            }

            
            if(this->Consecutivos(fil,col,CPU,1,0,2))//2 vertical CPU
            {
                puntaje += prioridadBaja;//es una jugada predecible
            }
            if(this->Consecutivos(fil,col,CPU,0,1,2))//2 horizontal CPU
            {
                puntaje += prioridadMedia;//podria no ser tan predecible
            }
            if(this->Consecutivos(fil,col,CPU,1,1,2))//2 diagonal CPU
            {
                puntaje += prioridadAlta;//las diagonales son mas elaboradas
            }
            if(this->Consecutivos(fil,col,CPU,1,-1,2)){//2 diagonal opuesta CPU
                //diagonales
                puntaje += prioridadAlta;
            }

        }
    }
    return puntaje;
}

//settea todas las casillas como vacias para poder reiniciar el tablero
//este metodo sera usado cuando se elige la opcion 'nueva Partida'
void ConectaCuatro::limpiarTablero(){
    for(int fil = 0; fil < this->filas;fil++){
        for(int col = 0; col < this->columnas; col++){
            this->tablero[fil][col] = 0;
        }
    }
}