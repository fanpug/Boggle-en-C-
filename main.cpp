/************************************************
				CREADO POR
	HUMBERTO ALEJANDRO NAVARRO ANDUJO
     RAUL ALEJANDRO DIAZ GUTIERREZ

https://github.com/fanpug/Boggle-en-C-/
************************************************/
//diccionario20202.txt

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

//Definicion de la longitud del abecedario del trie
#define TA 25
#define FILAS 8
#define COLUMNAS 8

using namespace std;

//Estructura del nodo
typedef struct nodot
{
    struct nodot *rutas[TA];
    int fin;
    char letra;
    struct nodot *papa;

} tnodo;

//********Funciones para el funcionamiento del Trie********
int indice(char letra);
tnodo *nuevo(char letra);
int buscar(char *cade);
int insertar(char *cade);

//********Variables globales para el Trie********
tnodo *trie=NULL;
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
bool verificarPalabra(string palabra);

//********Variables globales para el Boggle********
char tablero[FILAS][COLUMNAS];


/**************************************************************
                        FUNCION MAIN
**************************************************************/
int main(int argc, const char * argv[])
{
    srand(time(0));
    int opc;
    trie = nuevo(0);
    leerDiccionario();
    printf("Creado por Humberto Navarro y Alejandro Diaz\n\n");

    do
    {
        //Imprime el menu principal y entramos al switch
        switch(opc = menuPrincipal())
        {
        case 1:
            system("CLS");
            printf("\nGenerando tablero...");
            gameLoop();
            break;

        case 2:
            printf("\nSi existe");
            break;

        case 3: //salir
            break;

        default:
            printf("\nEsa no es una opcion\n");
            system("pause");
            system("cls");
        }
    }
    while(opc!=3);


    return 0;
}


/**************************************************************
                FUNCION PRINCIPAL DEL JUEGO
**************************************************************/
void gameLoop()
{
    system("CLS");

    inicializarTablero();
    generarTablero();


    imprimirTablero();

    verificarPalabra("Hola");

    int o;
    cin >> o;

    system("CLS");
}


/**************************************************************
                FUNCION PARA DECLARAR EL ALFABETO
**************************************************************/
int indice(char letra)
{
    int x=0;
    //A-1, B-2, ...
    if(letra>='A' && letra<='Z')
        x = (letra - 'A') +1;
    if(letra>='a' && letra<='z')
        x = (letra - 'a') +1;

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
    if(aux!=NULL)
    {
        aux->letra = letra;
        aux->fin=0;
        for(i=0; i<TA; i++)
            aux->rutas[i]=NULL;
    }
    return aux;
}


/**************************************************************
                    FUNCION BUSCAR DEL TRIE
**************************************************************/
int buscar(char *cade)
{
    int exito=0;
    aux=trie;
    letra=cade;

    while(aux->rutas[indice(*letra)] !=NULL) //si hay un camino
    {
        aux=aux->rutas[indice(*letra)];
        letra++;
    }
    if(aux->fin && *letra==0)
        exito=1;

    return exito;

}

/**************************************************************
                    FUNCION INSERTAR DEL TRIE
**************************************************************/
int insertar(char *cade)
{
    int x;
    tnodo *aux2;
    x= buscar(cade);
    if(!x)
    {
        if(!aux->fin && *letra==0) //Es un prefijo, no es fin de palabra, caso 2
            aux->fin=1;
        while(*letra!=0) //me faltan nodos
        {
            aux2=nuevo(*letra);
            aux2->papa=aux;
            aux->rutas[indice(*letra)]=aux2;
            aux=aux2;
            letra++;
        }
        aux->fin=1;

    }
    return x;

}


/**************************************************************
                FUNCION PARA LEER EL ARCHIVO
                Y ALMACENAR PALABRAS EN TRIE
**************************************************************/
void leerDiccionario()
{

    char cadena[128];

    ifstream fe("diccionario20202.txt");

    while (!fe.eof())
    {

        fe >> cadena;
        //cout << cadena << endl;
        insertar(cadena);
    }

    fe.close();
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

    cout << "\n\n\n1)Jugar\n2)Puntuaciones\n3)Salir\n" << endl;
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
    printf ("\n\n");
    for (int i = 0; i < FILAS; i++)
    {
        printf ("\t");
        for (int j = 0; j < COLUMNAS; j++)
        {
            printf ("%c ", tablero[i][j]);
        }
        printf ("\n");
    }
    printf ("\n\n");
}


/**************************************************************
                    FUNCION PARA GENERAR LETRAS
**************************************************************/
char generateRandom()
{
    //Funcion para generar letras del tablero y mantener un balance entre
    //vocales y consonantes. (Sacado de internet)

    string consonants = "BCDFGHJKLMNPQRSTVWXYZ";
    string vowels = "AEIOU";
    int num = (rand() % (4 - 0 + 1)) + 0;

    if(num == 0)
    {
        num = (rand() % (4 - 0 + 1)) + 0;
        return vowels[num];
    }
    else{
        num = (rand() % (20 - 0 + 1)) + 0;
        return consonants[num];
    }
}


/**************************************************************
                FUNCIONES PARA RESOLVER EL TABLERO
**************************************************************/
bool verificarPalabra(string palabra){
    return false;
}


