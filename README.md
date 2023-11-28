# Taller2EDatosConectaCuatro
Segundo taller de Estructura de datos

comandos para ejecutar
crear el ejecutable
g++ -o main main.cpp
ejecutar el ejecutable
./main

Nombre: Javier Moraga Sanhueza
RUT:20.028.499-2

nombre de los archivos que se puedan crear:
resultados.csv
tablero.csv

Comparación entre minimax con y sin poda alfa beta:
Sin poda alfa beta el algoritmo es capaz de funcionar muy rapido hasta un maximo de 5 de profundidad
con la poda alfa beta el algoritmo funcionar muy rapido hasta 10 de profundidad, como este es un algoritmo exponencial debido a que se llama recursivamente 7 veces, sin la poda alfa beta sería un O(7^n)
Se podría decir que la poda alfa beta reduce el valor del exponente a la mitad aproximadamente O(7^(n/2)), siendo n la profundidad.

Decisiones de diseño:
ConectaCuatro: En una matriz de 6 filas x 7 columnas se representa el tablero del juego conecta cuatro
En el cual se tienen métodos para ingresarFicha donde se evalua de arriba hacia abajo si la casilla está vacía para ingresar la ficha del jugador especificado, retorna true de ser así o false si la columna no tiene casillas disponibles
Se tienen 4 metodos para calcula el puntaje a favor del CPU que es usado para la elección de minimax en 4 dificultades distintas
Se tienen estrategias básicas como otorgar ptj positivo cuando se crean lineas consecutivas de 2 o de 3 fichas de parte del CPU
Defender las columnas cercanas al centro tiene más posibilidades de ganar por lo que tienen mayor puntaje que jugar en los rincones
Ademas de un ptj mucho mayor si es que en el estado actual gana el CPU
Y en el caso opuesto se tienen ptj negativos para estrategias que haga el jugadorHumano o que vaya a ganar el jugadorHumano

Guardado de archivos csv:
Para el caso del tablero se guarda conservando la forma de matriz a partir de un metodo que recorre para cada fila todas sus columnas y las va concatenando en un string, cuando esto termine se lo paso al archivo creado que se llamará 'tablero.csv'
Como al crear un archivo con el mismo nombre se sobreescribe el anterior lo que quedará guardado será la última partida guardada.
Para el caso de crear el archivo 'resultados.csv' que tiene escrito el resultado final de un tablero, pero se necesita el total de resultados
Lo que hice fue si existe el archivo 'resultados.csv', extraer todo su contenido y concatenarlo en un string, que al final lo concateno con el texto nuevo y creo el archivo 'resultados.csv' que contendrá todo lo anterior más la nueva linea de texto
Si no existe el archivo, entonces lo crea y le ingresa la nueva linea de texto.

Minimax:
Para las jugadas posibles del tablero se considera un ciclo de tamaño 7 que son las 7 columnas, pero debido a que estas columnas se pueden llenar
Para asegurar una jugada valida se tiene que el metodo ingresarFicha retorne true si es que la columna a la que se le quiere ingresar una ficha tiene al menos una casilla vacía.
Entonces si la jugada es valida se ingresa la ficha, se calcula recursivamente el puntaje llamando al propio metodo minimax y como este es un juego por turnos, se llama con un bool opuesto, si estaba maximizando llama a minimax con dicho bool en false para que su hijo entre a minimizar y luego el hijo del hijo entre a maximizar de nuevo
Cuando llegue a un nodo terminal y retorne el nodo con el estado del tablero, se le calculara el puntaje, y se deshará el movimiento con un metodo llamado quitarFicha que retorna true cuando quita un tipo de ficha especifico(CPU si maximiza/human si minimiza) en la columna especifica
Entonces tenemos el puntaje del primer nodo terminal, se compara con el mejor o peor puntaje si está maximizando o minimizando y se actualiza si se encuentra un mejor candidato a mejorTablero para el CPU(turno de CPU, maximizar) o peorTablero para el CPU(en el turno del jugador, minimizar)
Cuando se actualiza el mejor candidato se debe preguntar si ya estaba apuntando a uno antes, de ser así se llama a su destructor y luego lo hago apuntar al nuevoMejorCandidato a jugada
Como al tablero se le deshace la ultima jugada hecha para no tener que crear multiples tableros, Se necesito guardar la columna a la que representa el puntaje de la jugada hecha.
Es por esto que a la clase NodoConectaCuatro se le asigna una variable llamada 'columnaElegida' que es la iteracion actual y la ultima jugada hecha en ese Nodo retornado.

Poda alfa beta:
El valor de alfa que empieza desde el inf negativo(en este caso -999999) y se va actualizando cada vez que se encuentra un mejor candidato
Alfa representa el mejor puntaje conocido por el maximizador, y Beta representa el mejor puntaje para el minimizador
Entonces cuando Beta sea menor que Alfa en el minimizador implica que el maximizador que llamo al minimizador ya tiene una eleccion distinta, por lo que se puede saltar esta parte
Y de igual forma en el Maximizador cuando Alfa sea mayor que Beta implica que el minimizador que llamo al maximizador ya tiene una eleccion con el ptj mas bajo posible, por lo que se puede saltar esta parte.
