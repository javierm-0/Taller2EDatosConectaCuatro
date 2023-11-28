# Taller2EDatosConectaCuatro
Segundo taller de Estructura de datos

comandos para ejecutar
crear el ejecutable
g++ -o main main.cpp
ejecutar el ejecutable
./main

Nombre: Javier Moraga Sanhueza
RUT:20.028.499-2

Comparación entre minimax con y sin poda alfa beta:
sin poda alfa beta el algoritmo es capaz de funcionar bien hasta un maximo de 5 de profundidad
con la poda alfa beta el algoritmo funciona bien hasta 10 de profundidad

Decisión de diseño:
Para las jugadas posibles del tablero se considera un ciclo de tamaño 7 que son las 7 columnas, pero debido a que estas columnas se pueden llenar
Para asegurar una jugada valida se tiene que el metodo ingresarFicha retorne true si es que la columna a la que se le quiere ingresar una ficha tiene al menos una casilla vacía
Entonces si la jugada es valida se ingresa la ficha y ahora se puede calcular el puntaje llamando recursivamente al metodo minimax
Cuando llegue a un nodo terminal y retorne un valor, entonces el nodo que es padre o mejor dicho el que llamo minimax y llego a un nodo terminal
Comparara el puntaje mayor si está maximizando o puntaje menor si está minimizando de manera que en cada iteracion podría actualizar el puntajeActual y la columna elegida que es responsable de esa puntuación
Cuando se va a apuntar a otro Nodo, se pregunta si es que el puntero esta apuntando a un Nodo distinto de NULL
De ser así entonces llama a su destructor para liberar la memoria antes de apuntar al nuevo Mayor(si está Maximizando) o al nuevo menor(si está minimizando)


(...)
//pendiente: explicar como funciona la poda
//pendiente: explicar mas la implementacion
