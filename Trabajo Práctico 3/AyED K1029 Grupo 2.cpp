/*
Funciones próximas a implementar:
1. importación del archivo.bin con las materias y sus respectivas correlativas
2. Diseño y desarrollo de una consola interactiva para poder obtener información sobre una materia específica
*/

#include <iostream>

using namespace std;

struct NodoCorrelativa {
    int codMateria;
    bool cursadaAprobada;
    bool finalAprobado;
    NodoCorrelativa* sig;
};

struct Materia {
    int codigo;
    char nombre[50];
    NodoCorrelativa* correlativa;
    Materia* sig;
};

int main() {

}