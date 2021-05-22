/*********************************************
				CREADO POR
	HUMBERTO ALEJANDRO NAVARRO ANDUJO
     RAUL ALEJANDRO DIAZ GUTIERREZ

  https://github.com/fanpug/Boggle-en-C-
*********************************************/
//Manejo de colisiones -> Sondeo Cuadratico
//diccionario20202.txt

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <string>
#include <bits/stdc++.h>
#include <iomanip>

//Definicion de la longitud del abecedario del trie
#define TA 27
#define FILAS 8
#define COLUMNAS 8
#define M 133 //numero primo no cercano a potencia de 2 o 10 para hash de division

using namespace std;

//Estructura del nodo
typedef struct snodo
{
    char letra;
    struct snodo *hijos[28];
    struct snodo *padre;
    bool juega;
    int fin;
} tnodo;

//ESTRUCTURA DE TABLA HASH
typedef struct hashnodo
{
    char llave[20]; //Caracteres del usuario
    bool palabraEncontrada;
    bool ocupado; //si casilla ocupada == true
} trabajadores;

trabajadores arr[M];

//********Funciones para el funcionamiento del Trie********
int indice(char letra);
tnodo *nuevo(char letra);
int buscar(char *cade);
int insertar(char *cade);

//********Variables globales para el Trie********
tnodo *raiz = NULL;
tnodo *aux;
char *letra;

//********Funciones para Boggle********
void leerDiccionario();
int menuPrincipal();
void inicializarTablero();
void generarTablero();
void imprimirTablero();
char generateRandom();
void gameLoop();
void guardarPuntaje();

void validarPalabra(char *cadena);
void vecinos(int i, int j, char *siguiente, int s);
void reiniciar();

//********Variables globales para el Boggle********
char tablero[FILAS][COLUMNAS];
int banderas[FILAS][COLUMNAS];
bool encontrada;

//FUNCIONES DE HASH
void inicializarHash();
int fhash(char *llave);
int insertarHash(char *llave);
int buscarHash(char *llave);
int eliminarHash();

int puntos = 0;
string alias;
string respuesta;

/**************************************************************
                        FUNCION MAIN
**************************************************************/
int main(int argc, const char *argv[])
{
    srand(time(0));
    int opc;
    raiz = nuevo(0);
    raiz->padre = NULL;

    leerDiccionario();
    inicializarHash();

    printf("Creado por Humberto Navarro y Alejandro Diaz\t\tMayo 2021\n\n");

    do
    {
        //Imprime el menu principal y entramos al switch
        switch (opc = menuPrincipal())
        {
        case 1:
            system("CLS");
            printf("\nGenerando tablero...");
            gameLoop();
            break;

        case 2:
            //se imprime la tabla de puntaciones de un usuario con un ancho de 24 espacios para que quede bonito.
            cout << setw(24) << "ALIAS" << setw(10) << "PUNTAJE" << endl;
            cout << setw(24) << alias << setw(10) << puntos << endl;
            system("pause");
            system("cls");
            break;

        case 3: //salir
            break;

        default:
            printf("\nEsa no es una opcion\n");
            system("pause");
            system("cls");
        }
    } while (opc != 3);

    return 0;
}

/**************************************************************
                FUNCION PRINCIPAL DEL JUEGO
**************************************************************/
void gameLoop()
{
    int vidas = 3;
    int x = 0;
    string palab = " ";
    char cadena[25];
    inicializarTablero();
    generarTablero();

    cout << "\n\nSi desea regresar al menu principal, en cualquier momento" << endl;
    cout << "puede escribir 'Exit' \n\n";
    system("pause");

    //LOOP PRINCIPAL DEL JUEGO
    do
    {
        system("CLS");
        reiniciar();

        //se imprime el tablero y los puntos del jugador
        imprimirTablero();
        cout << "\t\t\t\tPuntos Actuales: " << puntos << "\n";
        cout << "\t\t\t\tVidas Restantes: " << vidas << "\n";
        cout << "\tIntroduzca la palabra: ";
        cin >> palab;

        //revisa si la palabra es Exit para salirse del while
        if (palab == "Exit")
        {
            break;
        }
        else
        {
            //convierte la variable palab de string a char
            strcpy(cadena, palab.c_str());
            //manda la cadena para verificar si la palabra es posible en el tablero
            validarPalabra(cadena);

            //si la palabra es posible
            if (encontrada == true)
            {
                //la buscamos dentro del Trie
                x = buscar(cadena);
                if (x == 1)
                {
                    x = -1;
                    //la buscamos dentro de la lista de palabras encontradas
                    x = buscarHash(cadena);
                    if (x > M || x < 0)
                    {
                        //la palabra no ha sido encontrada aun, le decimos felicidades al usuario por ser muy listo
                        printf("\nCorrecto! WOW!!!");
                        puntos += strlen(cadena);
                        x = insertarHash(cadena);
                        if (x > M || x < 0)
                        {
                            //no pudo insertar la palabra por alguna razon
                            printf("\n\nNO HAY SISTEMA JOVEN\n");
                        }
                        else
                        {
                            //se inserto la palabra en la lista de palabras encontradas
                            printf("\n\nSe inserto en la posicion %i de la lista de palabras encontradas!\n", x);
                        }
                    }
                    else
                    {
                        //la palabra ya fue encontrada, se lo decimos
                        printf("\nBuen intento, pero esa palabra ya habia sido encontrada!!");
                    }
                }
                else
                {
                    //si no le decimos que es listo pero no tanto
                    printf("\nIncorrecto! Esa palabra no es parte del diccionario!!");
                    vidas--;
                }
            }
            else
            {
                //si no le decimos que esta tonto
                printf("\nIncorrecto! No se puede escribir esa palabra!!");
                vidas--;
            }
        }
        if (vidas <= 0)
        {
            system("cls");
            printf("\n\nGAME OVER!\n\n");
            guardarPuntaje();
            system("pause");
            break;
        }
        printf("\n\n");
        system("pause");
    } while (true);
    eliminarHash();
    system("CLS");
}

/**************************************************************
                FUNCION PARA DECLARAR EL ALFABETO
**************************************************************/
int indice(char letra)
{
    int x = 0;
    //A-1, B-2, ...
    if (letra >= 'A' && letra <= 'Z')
        x = (letra - 'A') + 1;
    if (letra >= 'a' && letra <= 'z')
        x = (letra - 'a') + 1;

    return x;
}

/**************************************************************
                FUNCION PARA CREAR UN NODO DEL TRIE
**************************************************************/
tnodo *nuevo(char letra)
{
    tnodo *aux;
    int i;
    aux = (tnodo *)malloc(sizeof(tnodo));
    if (aux != NULL)
    {
        aux->letra = letra;
        aux->fin = 0;
        for (i = 0; i < TA; i++)
            aux->hijos[i] = NULL;
    }
    return aux;
}

/**************************************************************
                    FUNCION BUSCAR DEL TRIE
**************************************************************/
int buscar(char *cade)
{
    int exito = 0;
    aux = raiz;
    letra = cade;

    while (aux->hijos[indice(*letra)] != NULL) //si hay un camino
    {
        aux = aux->hijos[indice(*letra)];
        letra++;
    }
    if (aux->fin && *letra == 0)
        exito = 1; //palabra existe

    return exito;
}

/**************************************************************
                    FUNCION INSERTAR DEL TRIE
**************************************************************/
int insertar(char *cade)
{
    int x;
    tnodo *aux2;
    x = buscar(cade);
    if (!x)
    {
        if (!aux->fin && *letra == 0) //Es un prefijo, no es fin de palabra, caso 2
            aux->fin = 1;
        while (*letra != 0) //me faltan nodos
        {
            aux2 = nuevo(*letra);
            aux2->padre = aux;
            aux->hijos[indice(*letra)] = aux2;
            aux = aux2;
            letra++;
        }
        aux->fin = 1;
    }
    return x;
}

/**************************************************************
                FUNCION PARA LEER EL ARCHIVO
                Y ALMACENAR PALABRAS EN TRIE
**************************************************************/
void leerDiccionario()
{
    char cadena[50];
    FILE *archivo;

    //ejemplo();
    archivo = fopen("diccionario20202.txt", "r"); //abre el archivo con permisos de solo lecta

    if (archivo == NULL)
        exit(1);
    else
    {
        while (!feof(archivo)) //si existe algo en el archivo
        {
            fgets(cadena, 50, archivo); //se obtiene cada cadena y se guarda en cadena
            insertar(cadena);
        }
    }
    fclose(archivo); //se cierra el archivo
    int t = 0;
    while (cadena[t] != '\0') //para que no quede guardada la cadena
    {
        cadena[t] = '\0';
        t++;
    }
}

/**************************************************************
            FUNCION PARA IMPRIMIR EL MENU PRINCIPAL
**************************************************************/
int menuPrincipal()
{
    int opc = 0;

    printf("\n    ____     ____     ______   ______    __      ______  ");
    printf("\n   / __ )   / __ \\   / ____/  / ____/   / /     / ____/  ");
    printf("\n  / __  |  / / / /  / / __   / / __    / /     / __/     ");
    printf("\n / /_/ /  / /_/ /  / /_/ /  / /_/ /   / /___  / /___     ");
    printf("\n/_____/   \\____/   \\____/   \\____/   /_____/ /_____/     ");

    cout << "\n\n\n1)Jugar\n2)Puntuaciones\n3)Salir\n"
         << endl;
    cout << "Introduzca el numero de la opcion deseada: ";
    cin >> opc;

    return opc;
}

/**************************************************************
            FUNCION PARA INICIALIZAR EL TABLERO
**************************************************************/
void inicializarTablero()
{
    //Se inicializa el tablero vacio
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
            tablero[i][j] = 0;
    }
}

/**************************************************************
                FUNCION PARA GENERAR EL TABLERO
**************************************************************/
void generarTablero()
{
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            tablero[i][j] = generateRandom();
        }
    }
}

/**************************************************************
                FUNCION PARA IMPRIMIR EL TABLERO
**************************************************************/
void imprimirTablero()
{
    //Imprime el tablero de forma bonita
    printf("\n\n");
    cout << "\t    ____________________________ \n";
    cout << "\t   ||--------------------------||\n";
    for (int i = 0; i < FILAS; i++)
    {
        cout << "\t   ||  ";
        for (int j = 0; j < COLUMNAS; j++)
        {
            printf("%c  ", tablero[i][j]);
        }
        cout << "||";
        printf("\n");
    }
    cout << "\t   ||--------------------------||\n";
    cout << "\t   ||__________________________|| \n";
    printf("\n\n");
}

/**************************************************************
                    FUNCION PARA GENERAR LETRAS
**************************************************************/
char generateRandom()
{
    //Funcion para generar letras del tablero y mantener un balance entre
    //vocales y consonantes. (Sacado de internet)

    string consonants = "bcdfghjklmnpqrstvwxyz";
    string vowels = "aeiou";
    int num = (rand() % (4 - 0 + 1)) + 0;

    if (num == 0 || num == 1)
    {
        num = (rand() % (4 - 0 + 1)) + 0;
        return vowels[num];
    }
    else
    {
        num = (rand() % (20 - 0 + 1)) + 0;
        return consonants[num];
    }
}

/**************************************************************
        FUNCIONES PARA VALIDAR LA PALABRA EN EL TABLERO
**************************************************************/
void validarPalabra(char *cadena)
{
    int s = 0;

    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            s = 1;
            if (tablero[i][j] == cadena[0]) //se checa si una letra en el tablero es igual a la primera a buscar
            {
                banderas[i][j] = 1;       //se levanta una bandera en el lugar de la letra inicial
                vecinos(i, j, cadena, s); //funci�n para checar alrededor de la letra si existe un camino a las dem�s
                if (encontrada == true)   //salir de los ciclos
                {
                    i = 7;
                    j = 7;
                }
                else
                {
                    reiniciar();
                }
            }
        }
    }
}

//funci�n recursiva para checar si existe el camino en todas las direcciones hacia una palabra buscada
void vecinos(int i, int j, char *siguiente, int s)
{
    if (s == strlen(siguiente)) //si se lleg� al final, se marca la palabra como encontrada
    {
        encontrada = true;
    }

    for (int k = 1; k >= -1; k--)
    {
        for (int l = 1; l >= -1; l--)
        {
            if (i - k == -1 || i - k == FILAS)
            {
            }
            else if (j - l == -1 || j - l == COLUMNAS)
            {
            }
            else
            {
                if (tablero[i - k][j - l] == siguiente[s] && banderas[i - k][j - l] == 0) //se busca alrededor de cada letra
                {
                    banderas[i][j] = 1;
                    vecinos(i - k, j - l, siguiente, s + 1); //avanzar entre letras
                }
            }
        }
    }
}

//funcion para reiniciar las banderas del tablero
void reiniciar()
{
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            banderas[i][j] = 0;
        }
    }
    encontrada = false;
}

/**************************************************************
                    FUNCIONES PARA EL HASH
**************************************************************/
void inicializarHash()
{
    for (int i = 0; i < M; i++)
    {
        //todas las casillas inician desocupadas
        arr[i].ocupado = false;
    }
}

int fhash(char *llave)
{
    int indice = 0, modulo = 0;

    //Usa las primeras 4 letras (letra * numeroMaximoDeLetras*10 * posicion de letra)
    indice = llave[0] * (200 * 1);
    indice += llave[1] * (200 * 2);
    indice += llave[2] * (200 * 3);
    indice += llave[3] * (200 * 4);

    modulo = indice % M;
    return modulo;
}

int insertarHash(char *llave)
{
    int pos = 0, orig = 0; //posicion actual, casilla original
    int intento = 1;       //numero de intento de insercion
    orig = pos = fhash(llave);
    int x = -1; //-1 = no se pudo insertar

    //mientras la casilla este ocupada y no me haya pasado del limite maximo del arreglo, sigue intentando
    while (arr[pos].ocupado == true && pos < M)
    {
        intento++;
        pos = orig + (intento * intento) % M;
    }

    //si nos pasamos del arreglo, pero no encontramos espacio
    if (arr[pos].ocupado == true)
    {
        while (arr[pos].ocupado == true && pos > -1)
        {
            intento++;
            pos = orig - (intento * intento) % M;
        }
    }

    //si el lugar no esta ocupado
    if (arr[pos].ocupado == false)
    {
        strcpy(arr[pos].llave, llave);
        arr[pos].palabraEncontrada = true; //la palabra ha sido encontrada
        arr[pos].ocupado = true;           //si no estaba ocupado, ahora si
        x = pos;
    }

    return x;
}

int buscarHash(char *llave)
{
    int pos = 0, orig = 0, i = -1; //posicion actual, posicion original, variable equis para regresar posicion
    int bandera = 0, intento = 1;  //bandera para bucle, intentos de sondeo
    orig = fhash(llave);
    pos = orig;

    do
    {
        //mejor caso, la llave se encuentra luego luego y la casilla esta ocupada
        if (strcmp(llave, arr[pos].llave) == 0 && arr[pos].ocupado == true)
        {
            i = pos;
            bandera = 1; //se encontro la llave
        }
        else
        {
            intento++;
            pos = orig + (intento * intento) % M;
        }

    } while (bandera == 0 && pos < M);

    //si nos pasamos del arreglo, pero no encontramos el que buscamos
    if (bandera == 0)
    {
        intento = 1;
        do
        {
            pos = orig - (intento * intento) % M;
            if (strcmp(llave, arr[pos].llave) == 0 && arr[pos].ocupado == true)
            {
                i = pos;
            }
            intento++;
        } while (arr[pos].ocupado == true || pos > -1);
    }

    return i;
}

int eliminarHash()
{
    int x = 0;

    while (x < M)
    {
        if (arr[x].ocupado = true)
        {
            arr[x].ocupado = false; //si estaba ocupado entonces ya no
        }
        x++;
    }

    return x;
}

/**************************************************************
            FUNCIONES PARA GUARDAR EL PUNTAJE
**************************************************************/
void guardarPuntaje()
{
    cout << "Desea guardar su puntuacion? Si / No" << endl;
    cin >> respuesta;
    transform(respuesta.begin(), respuesta.end(), respuesta.begin(), ::tolower); //para convertir el input a minusculas y poder manipular mejor la opcion del usuario.
    if (respuesta == "si")
    {
        do
        {
            respuesta = "";
            cout << "Introduce un nombre o alias para guardar el puntaje: ";
            cin >> alias;
        } while (respuesta == "si");
    }
    else if (respuesta == "no")
    {
        cout << "Gracias por jugar nuestro Boogle." << endl;
    }
    else
    {
        cout << "Esa no es una opcion, perdiste todos tus puntos por no leer bien." << endl;
    }
}
