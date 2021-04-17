/*********************************************
				CREADO POR
	HUMBERTO ALEJANDRO NAVARRO ANDUJO
     RAUL ALEJANDRO DIAZ GUTIERREZ

  https://github.com/fanpug/Boggle-en-C-
*********************************************/
//diccionario20202.txt

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

//Definicion de la longitud del abecedario del trie
#define TA 27
#define FILAS 8
#define COLUMNAS 8

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

//********Funciones para el funcionamiento del Trie********
int indice(char letra);
tnodo *nuevo(char letra);
int buscar(char *cade);
int insertar(char *cade);

//********Variables globales para el Trie********
tnodo *raiz=NULL;
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

void validarPalabra(char *cadena);
void vecinos(int i, int j, char *siguiente, int s);
void reiniciar();

//********Variables globales para el Boggle********
char tablero[FILAS][COLUMNAS];
int banderas[FILAS][COLUMNAS];
bool encontrada;

/**************************************************************
                        FUNCION MAIN
**************************************************************/
int main(int argc, const char * argv[])
{
    srand(time(0));
    int opc;
    raiz = nuevo(0);
    raiz->padre = NULL;

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
            printf("\nCooming Soon!");//jaja salu2
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
    }
    while(opc!=3);


    return 0;
}


/**************************************************************
                FUNCION PRINCIPAL DEL JUEGO
**************************************************************/
void gameLoop()
{
    int puntos = 0, vidas = 3;
    int x = 0;
    string palab = " ";
    char cadena[25];
    inicializarTablero();
    generarTablero();

    cout << "\n\nSi desea regresar al menu principal, en cualquier momento" << endl;
    cout << "puede escribir 'Exit' \n\n";
    system("pause");

    //LOOP PRINCIPAL DEL JUEGO
    do{
        system("CLS");
        reiniciar();

        //se imprime el tablero y los puntos del jugador
        imprimirTablero();
        cout << "\t\t\t\tPuntos Actuales: " << puntos << "\n";
        cout << "\t\t\t\tVidas Restantes: " << vidas << "\n";
        cout << "\tIntroduzca la palabra: ";
        cin >> palab;

        //revisa si la palabra es Exit para salirse del while
        if(palab == "Exit")
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
            if(encontrada == true)
            {
                //la buscamos dentro del Trie
                x = buscar(cadena);
                if(x == 1)
                {
                    //si existe le decimos felicidades al usuario
                    printf("\nCorrecto! WOW!!!");
                    puntos += strlen(cadena);

                }
                else
                {
                    //si no le decimos que es listo pero no tanto
                    printf("\nIncorrecto! Esa palabra no es parte del Trie!!");
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
        if(vidas <= 0){
            system("cls");
            printf("\n\nGAME OVER!\n\n");
            system("pause");
            break;
        }
        printf("\n\n");
        system("pause");
    }while(true);

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
            aux->hijos[i]=NULL;
    }
    return aux;
}


/**************************************************************
                    FUNCION BUSCAR DEL TRIE
**************************************************************/
int buscar(char *cade)
{
    int exito=0;
    aux=raiz;
    letra=cade;

    while(aux->hijos[indice(*letra)] != NULL) //si hay un camino
    {
        aux = aux->hijos[indice(*letra)];
        letra++;
    }
    if(aux->fin && *letra == 0)
        exito = 1;  //palabra existe

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
            aux2->padre=aux;
            aux->hijos[indice(*letra)]=aux2;
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

    char cadena[50];
    FILE *archivo;

    //ejemplo();
    archivo = fopen("diccionario20202.txt","r");//abre el archivo con permisos de solo lecta

    if(archivo == NULL)
        exit(1);
    else
    {
        while(!feof(archivo)) //si existe algo en el archivo
        {
            fgets(cadena,50,archivo);//se obtiene cada cadena y se guarda en cadena
            insertar(cadena);
        }
    }
    fclose(archivo);//se cierra el archivo
    int t = 0;
    while(cadena[t] != '\0') //para que no quede guardada la cadena
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
    cout << "\t    ____________________________ \n";
    cout << "\t   ||--------------------------||\n";
    for (int i = 0; i < FILAS; i++)
    {
        cout << "\t   ||  ";
        for (int j = 0; j < COLUMNAS; j++)
        {
            printf ("%c  ", tablero[i][j]);
        }
        cout << "||";
        printf ("\n");
    }
    cout << "\t   ||--------------------------||\n";
    cout << "\t   ||__________________________|| \n";
    printf ("\n\n");
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

    if(num == 0)
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

    for(int i = 0; i < FILAS; i++)
    {
        for(int j = 0; j < COLUMNAS; j++)
        {
            s = 1;
            if(tablero[i][j] == cadena[0]) //se checa si una letra en el tablero es igual a la primera a buscar
            {
                banderas[i][j] = 1;//se levanta una bandera en el lugar de la letra inicial
                vecinos(i, j, cadena, s);//función para checar alrededor de la letra si existe un camino a las demás
                if(encontrada == true) //salir de los ciclos
                {
                    i=7;
                    j=7;
                }
                else
                {
                    reiniciar();
                }
            }
        }
    }
}

//función recursiva para checar si existe el camino en todas las direcciones hacia una palabra buscada
void vecinos(int i, int j, char *siguiente, int s)
{
    if(s == strlen(siguiente)) //si se llegó al final, se marca la palabra como encontrada
    {
        encontrada = true;
    }

    for(int k=1; k>=-1; k--)
    {
        for(int l=1; l>=-1; l--)
        {
            if(i-k == -1 || i-k == FILAS) {}
            else if(j-l == -1 || j-l == COLUMNAS) {}
            else
            {
                if(tablero[i-k][j-l] == siguiente[s] && banderas[i-k][j-l] == 0) //se busca alrededor de cada letra
                {
                    banderas[i][j] = 1;
                    vecinos(i-k, j-l, siguiente, s+1);//avanzar entre letras
                }
            }
        }
    }
}

//funcion para reiniciar las banderas del tablero
void reiniciar()
{
    for(int i=0; i<FILAS; i++)
    {
        for(int j=0; j<COLUMNAS; j++)
        {
            banderas[i][j] = 0;
        }
    }
    encontrada = false;
}









