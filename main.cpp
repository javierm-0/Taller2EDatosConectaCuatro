#include <iostream>

//using namespace std;
//tuve que quitar el using std porque 
//tenia un problema que decia cout is ambiguous, no crasheaba pero se veia peligroso
//asi que puse manualmente std:: para cada cout que era donde decia salir el problema

#include <limits>
#include "ConectaCuatro.h"
#include "ComputerPlayer.h"

//Nombre: Javier Moraga Sanhueza
//RUT: 20.028.499-2

//si al menos un char no es un numero retorna false
//sino retorna true y ahora puedo convertirlo a int de manera segura
bool soloDigitos(std::string numero){
    for(int i = 0;i < numero.size();i++){
        if(!isdigit(numero[i])) return false;
    }
    return true;
}

//extrae y concatena todo el contenido anterior en un string llamado r
//lo concatena con el string del resultadoMasReciente
//Y crea el archivo resultados.csv 
//al que le ingresa el string r
void actualizarResultadoDelJuego(std::string resultadoMasReciente){
    std::string r = "";
    std::ifstream archivoAntiguo("resultados.csv");
    if(archivoAntiguo.good()){//si existe y se puede acceder
        std::string linea;
        while(std::getline(archivoAntiguo,linea)){
            r += linea+"\n";//concatena todo el contenido existente
        }
    }
    
    r += resultadoMasReciente;//concatena con lo mas reciente
    std::ofstream archivo("resultados.csv");
    if(archivo.is_open()){
        archivo<<r;
        return;
    }
    std::cout<<"Error?"<<std::endl;
}

//metodo encargado de correr hasta que se determine un ganador (o un empate)
//o si se desea guardar partida, se llamara a otro metodo que creara el archivo .csv que contenga la matriz del tablero actual
void jugarConectaCuatro(ConectaCuatro* conectaCuatro,ComputerPlayer* computador,std::string dificultad,bool p1Inicia){
    std::string resultadoDelJuego = "";
    int col;
    bool jugadaHecha = false;
    int player1 = 1; int CPU = 2;
    std::string resp = "";

    int turno = 1+42-conectaCuatro->contarCasillasVacias();//el turno se determina por la cantidad de casillas vacias
    //asi al cargar una partida antigua estara en un numero que coincida con donde se dejo la partida
    int puntajeTablero = 0;
    bool eligioGuardar = false;
    while(true){
        while(p1Inicia){//p1Inicia true entonces le toca al jugador
            turno = 1+42-conectaCuatro->contarCasillasVacias();
            col = 0;
            resp = "a";
            std::cout<<conectaCuatro->verTableroConMejorFormato()<<std::endl;
            std::cout<<"Turno "<<turno<<std::endl;
            std::cout<<"player1('X'): "<<std::endl;
            std::cout<<"Ingrese la columna a la que ingresara una ficha(1~7): "<<std::endl;
            std::cout<<"Para guardar la partida ingrese'S'"<<std::endl;
            std::cout<<"Para salir ingrese '-1' (esta accion guardara su partida)"<<std::endl;
            std::getline(std::cin,resp);
            if(resp == "-1") 
            {
                conectaCuatro->guardarPartida();
                std::cout<<"Se ha guardado la partida"<<std::endl;
                std::cout<<"Regresando al menu"<<std::endl;
                return;
            }
            if(toupper(resp[0]) == 'S'){
                conectaCuatro->guardarPartida();
                std::cout<<"Se ha guardado la partida"<<std::endl;
                eligioGuardar = true;
            }

            if(resp != "" && conectaCuatro->soloDigitos(resp)){//si no esta vacio y es un digito int valido
                col = stoi(resp)-1;//lo convierte a int(y le resta uno para ajustarlo)
                if(col >= 0 && col <= 6){//y si entra al rango continua
                    jugadaHecha = conectaCuatro->ingresarFicha(col,player1);
                    std::cout<<conectaCuatro->verTableroConMejorFormato()<<std::endl;
                    if(jugadaHecha){
                        p1Inicia = false;
                        std::cout<<"Tu jugada tiene un ptj de "<<puntajeTablero<<std::endl;
                        if(toupper(dificultad[0]) == 'E'){
                            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadFacil()*(conectaCuatro->contarCasillasVacias()+1);
                        }
                        else if(toupper(dificultad[0]) == 'M'){
                            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadNormal()*(conectaCuatro->contarCasillasVacias()+1)*2;
                        }
                        else if(toupper(dificultad[0]) == 'H'){
                            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadDificil()*(conectaCuatro->contarCasillasVacias()+1)*3;
                        }
                        else{
                            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadImposible()*(conectaCuatro->contarCasillasVacias()+1)*4;
                        }
                        if(conectaCuatro->existeGanador(player1)){
                            std::cout<<"ptj total de "<<puntajeTablero<<" (este puntaje representa que tanto acerca al CPU a la victoria)"<<std::endl;
                            
                            resultadoDelJuego = "player1: WINS";
                            actualizarResultadoDelJuego(resultadoDelJuego);
                            std::cout<<resultadoDelJuego<<std::endl; return;
                        }
                        if(conectaCuatro->tableroLleno()){
                            std::cout<<"empate"<<std::endl; return;
                        }
                    }
                }
            }
            else {
                if(!eligioGuardar){//no se muestra este msj si es que se entra con 'S' para guardar partida
                    std::cout<<"columna invalida o respuesta inesperada(string?)"<<std::endl;
                }

            }
        }
        if(toupper(dificultad[0]) == 'E'){
            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadFacil()*(conectaCuatro->contarCasillasVacias()+1);
        }
        else if(toupper(dificultad[0]) == 'M'){
            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadNormal()*(conectaCuatro->contarCasillasVacias()+1)*2;
        }
        else if(toupper(dificultad[0]) == 'H'){
            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadDificil()*(conectaCuatro->contarCasillasVacias()+1)*3;
        }
        else{
            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadImposible()*(conectaCuatro->contarCasillasVacias()+1)*4;
        }
        

        turno = 1+42-conectaCuatro->contarCasillasVacias();
        std::cout<<conectaCuatro->verTableroConMejorFormato()<<std::endl;
        std::cout<<"Turno "<<turno<<std::endl;
        std::cout<<"Turno de CPU('O')"<<std::endl;
        NodoConectaCuatro* jugadaCPU = NULL;
        if(jugadaCPU){
            jugadaCPU->~NodoConectaCuatro();
        }
        int alfa = -999999; int beta = 999999;//valores iniciales para alfa y beta
        if(toupper(dificultad[0]) == 'E'){
            jugadaCPU = computador->miniMaxPodaAlfaBeta(conectaCuatro,3,true,alfa,beta,dificultad);
            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadFacil()*(conectaCuatro->contarCasillasVacias()+1);
        }
        else if(toupper(dificultad[0]) == 'M'){
            jugadaCPU = computador->miniMaxPodaAlfaBeta(conectaCuatro,6,true,alfa,beta,dificultad);
            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadNormal()*(conectaCuatro->contarCasillasVacias()+1)*2;
        }
        else if(toupper(dificultad[0]) == 'H'){
            jugadaCPU = computador->miniMaxPodaAlfaBeta(conectaCuatro,9,true,alfa,beta,dificultad);
            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadDificil()*(conectaCuatro->contarCasillasVacias()+1)*3;
        }
        else{
            jugadaCPU = computador->miniMaxPodaAlfaBeta(conectaCuatro,11,true,alfa,beta,dificultad);
            puntajeTablero = conectaCuatro->calcularPuntajeHeuristicoDificultadImposible()*(conectaCuatro->contarCasillasVacias()+1)*4;
        }
        
        conectaCuatro->ingresarFicha(jugadaCPU->columnaElegida,CPU);
        //luego de ingresar ficha recalcula el puntaje del tablero considerando la dificultad elegida
        std::cout<<"CPU ha decidido usar la columna: "<<jugadaCPU->columnaElegida+1<<", PTJ: "<<puntajeTablero<<std::endl;
        std::cout<<"\n"<<std::endl;
        if(conectaCuatro->existeGanador(CPU)){
            resultadoDelJuego = "CPU: WINS";
            actualizarResultadoDelJuego(resultadoDelJuego);
            std::cout<<resultadoDelJuego<<std::endl; 
            std::cout<<conectaCuatro->verTableroConMejorFormato()<<std::endl;
            return;
        }
        if(conectaCuatro->tableroLleno()){
            std::cout<<"puntaje final: "<<puntajeTablero<<std::endl;
            std::cout<<"EMPATE"<<std::endl; return;
        }
        p1Inicia = true;
        std::cout<<"\n"<<std::endl;
    }
}

//si el caracter es 'E' o 'M' o 'H' o 'I' retorna true
//sino false
//ademas de que por usar toupper el char siempre vendra como mayuscula, permitiendo que 'e' o 'm' o 'h' o 'i' tambien sean validos
bool dificultadValida(std::string resp){
    if(resp == "") return false;
    if(resp.length() > 1) return false;
    return toupper(resp[0]) == 'E'|| toupper(resp[0]) == 'M' ||toupper(resp[0]) == 'H' || toupper(resp[0]) == 'I';
}

void mostrarResultados(){
    std::string resultado = "";
    std::ifstream archivoAntiguo("resultados.csv");
    if(archivoAntiguo.good()){//si existe y se puede acceder
        std::string linea;
        while(std::getline(archivoAntiguo,linea)){
            resultado += linea+"\n";//concatena todo el contenido existente
        }
        std::cout<<"Resultados\n"<<resultado<<std::endl;
    }
    else{
        std::cout<<"No hay resultados todavia"<<std::endl;
    }
}

//opcion de nuevaPartida
//Se debe limpiar el tablero
//se hace la llamada al metodo para limpiar el tablero, y se pide que ingrese la dificultad deseada
//en un loop hasta que ingrese una respuesta valida o ingrese '-1' para retroceder al menu anterior
void subMenu(ConectaCuatro* juego, ComputerPlayer* bot){
    std::string dificultad = ""; std::string repartirTurno = "";
    juego->limpiarTablero();
    while(!dificultadValida(dificultad)){
        std::cout<<"Ingrese dificultad del juego: 'E'(Easy) 'M'(Medium) 'H'(Hard) 'I'(Very Hard)"<<std::endl;
        std::cout<<"'-1' para regresar"<<std::endl;
        std::getline(std::cin,dificultad);
        if(dificultad == "-1"){
            std::cout<<"Regresando al menu"<<std::endl;
            break;
        }
        if(dificultad != "" && dificultadValida(dificultad)){
            std::cout<<"Ha elegido: "<<dificultad<<std::endl;
            while(true){
                std::cout<<"Ingrese '1' para empezar primero"<<std::endl;
                std::cout<<"'2' para permitir que el CPU empiece primero"<<std::endl;
                std::cout<<"'-1' para volver al menu inicial"<<std::endl;
                std::getline(std::cin,repartirTurno);
                if(repartirTurno == "1"){
                    jugarConectaCuatro(juego,bot,dificultad,true);//llama a la funcion para jugar con el jugador iniciando primero
                    break;
                }
                else if(repartirTurno == "2"){
                    jugarConectaCuatro(juego,bot,dificultad,false);//llama a la funcion para jugar con el CPU iniciando primero
                    break;
                }
                else if(repartirTurno == "-1"){
                    break;
                }
                else{
                    std::cout<<"ERROR: respuesta invalida"<<std::endl;
                }
            }
        }
        else{
            std::cout<<"ERROR: "<<dificultad<<" no es una dificultad valida"<<std::endl;
        }
    }
}

//Se llama a esta funcion para iniciar la partida ya existente
//por lo que no se pregunta por quien parte primero, dado que siempre se guarda en el turno del jugador
void subMenu2(ConectaCuatro* juego, ComputerPlayer* bot){
    std::string dificultad = "";
    while(!dificultadValida(dificultad)){
        std::cout<<"Ingrese dificultad del juego: 'E'(Easy) 'M'(Medium) 'H'(Hard) 'I'(Very Hard)"<<std::endl;
        std::getline(std::cin,dificultad);
        if(dificultad != "" && dificultadValida(dificultad)){
            std::cout<<"Ha elegido: "<<dificultad<<std::endl;
            jugarConectaCuatro(juego,bot,dificultad,true);//llama a la funcion para jugar con el jugador iniciando primero
        }
        else{
            std::cout<<"ERROR: "<<dificultad<<" no es una dificultad valida"<<std::endl;
        }
    }
}

void menu(ConectaCuatro* juego, ComputerPlayer* bot){
    bool invalidResp = true;
    while(invalidResp){
        std::string menu = "==============================";
        std::cout<<menu<<std::endl;
        std::cout<<"Bienvenido al inicio de Conecta Cuatro"<<std::endl;
        std::cout<<"Ingrese '1' para jugar nueva partida"<<std::endl;
        std::cout<<"Ingrese '2' para continuar la partida en cualquier dificultad"<<std::endl;
        std::cout<<"Ingrese '3' para ver los resultados de partidas anteriores"<<std::endl;
        std::cout<<"Ingrese '-1' salir"<<std::endl;
        std::cout<<menu<<std::endl;
        std::string opcion = "";
        getline(std::cin,opcion);
        if(opcion == "1"){
            std::cout<<"Nueva partida ha sido elegida"<<std::endl;
            subMenu(juego,bot);
        }
        else if(opcion == "2"){
            std::ifstream arch("tablero.csv");
            if(arch.is_open()){
                std::cout<<"cargando partida..."<<std::endl;
                juego->cargarPartida();
                subMenu2(juego,bot);
            }
            else{
                std::cout<<"No hay partidas guardadas o no se puede acceder al archivo"<<std::endl;
            }
        }
        else if(opcion == "3"){
            mostrarResultados();
        }
        else if(opcion == "-1"){
            std::cout<<"Cerrando todo"<<std::endl;
            return;
        }
        else{
            std::cout<<"Opcion invalida"<<std::endl;
        }
    }
}

int main()
{
    ConectaCuatro* juego = new ConectaCuatro();
    ComputerPlayer* bot = new ComputerPlayer();
    menu(juego,bot);
    bot->~ComputerPlayer();
    return 0;
}