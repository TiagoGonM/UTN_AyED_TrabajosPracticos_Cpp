#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

struct Materia {
    int codigo;
    char nombre[50];
    char correlatividad[50];
    char cursadaRequerida[3];
    char finalRequerido[3];
};

struct Nodo {
    Materia materia;
    Nodo* sig = NULL;
};

void agregarAlFinal(Nodo*& lista, int codigo, const char* nombre, const char* correlatividad, const char* reqCursada, const char* reqFinal) {
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->materia.codigo = codigo;
    
    // Usamos strcpy para copiar las cadenas de forma segura a los arreglos fijos
    strcpy(nuevoNodo->materia.nombre, nombre);
    strcpy(nuevoNodo->materia.correlatividad, correlatividad);
    strcpy(nuevoNodo->materia.cursadaRequerida, reqCursada); 
    strcpy(nuevoNodo->materia.finalRequerido, reqFinal);
    
    nuevoNodo->sig = NULL;
         
    if (!lista) {
        lista = nuevoNodo;
        return;
    }
    
    Nodo* nodoActual = lista;
    while (nodoActual->sig) {
        nodoActual = nodoActual->sig;
    }
    nodoActual->sig = nuevoNodo;
}

Materia dequeue(Nodo*& lista) {
    Nodo* nodoRemovido = lista;
    Materia materia = nodoRemovido->materia;
    
    lista = lista->sig;
    delete nodoRemovido;

    return materia;
}

void cargarArchivo(Nodo* lista) {
    FILE* f = fopen("materia_correlativas.bin", "wb");
    
    while (lista->sig) {
        Materia materia = dequeue(lista);
        fwrite(&materia, sizeof(Materia), 1, f);
        cout << "Escribiendo " << materia.nombre << endl;
    }

    fclose(f);
}

int main() {
    Nodo* materias = NULL;

    agregarAlFinal(materias, 1, "Algebra y Geometria Analitica", "Ninguna", "-", "-");
    agregarAlFinal(materias, 2, "Analisis Matematico I", "Ninguna", "-", "-");
    agregarAlFinal(materias, 3, "Fisica I", "Algebra y Geometria Analitica", "Si", "No");
    agregarAlFinal(materias, 3, "Fisica I", "Analisis Matematico I", "Si", "No");
    agregarAlFinal(materias, 4, "Matematica Discreta", "Ninguna", "-", "-");
    agregarAlFinal(materias, 5, "Sistemas y Organizaciones", "Ninguna", "-", "-");
    agregarAlFinal(materias, 6, "Algoritmos y Estructuras de Datos", "Ninguna", "-", "-");
    agregarAlFinal(materias, 7, "Arquitectura de Computadoras", "Fisica I", "Si", "No");
    agregarAlFinal(materias, 7, "Arquitectura de Computadoras", "Algoritmos y Estructuras de Datos", "Si", "No");
    agregarAlFinal(materias, 8, "Analisis Matematico II", "Algebra y Geometria Analitica", "Si", "Si");
    agregarAlFinal(materias, 8, "Analisis Matematico II", "Analisis Matematico I", "Si", "Si");
    agregarAlFinal(materias, 9, "Probabilidad y Estadistica", "Analisis Matematico II", "Si", "No");
    agregarAlFinal(materias, 10, "Sintaxis y Semantica de los Lenguajes", "Algoritmos y Estructuras de Datos", "Si", "Si");
    agregarAlFinal(materias, 11, "Paradigmas de Programacion", "Algoritmos y Estructuras de Datos", "Si", "Si");
    agregarAlFinal(materias, 11, "Paradigmas de Programacion", "Sintaxis y Semantica de los Lenguajes", "Si", "No");
    agregarAlFinal(materias, 12, "Sistemas Operativos", "Arquitectura de Computadoras", "Si", "Si");
    agregarAlFinal(materias, 12, "Sistemas Operativos", "Paradigmas de Programacion", "Si", "No");
    agregarAlFinal(materias, 13, "Diseno de Sistemas", "Sistemas y Organizaciones", "Si", "Si");
    agregarAlFinal(materias, 13, "Diseno de Sistemas", "Algoritmos y Estructuras de Datos", "Si", "Si");
    agregarAlFinal(materias, 13, "Diseno de Sistemas", "Matematica Discreta", "Si", "No");
    agregarAlFinal(materias, 14, "Bases de Datos", "Algoritmos y Estructuras de Datos", "Si", "Si");
    agregarAlFinal(materias, 14, "Bases de Datos", "Diseno de Sistemas", "Si", "No");
    agregarAlFinal(materias, 15, "Comunicaciones", "Fisica I", "Si", "Si");
    agregarAlFinal(materias, 15, "Comunicaciones", "Analisis Matematico II", "Si", "Si");
    agregarAlFinal(materias, 16, "Ingenieria de Software", "Diseno de Sistemas", "Si", "Si");
    agregarAlFinal(materias, 16, "Ingenieria de Software", "Paradigmas de Programacion", "Si", "Si");
    agregarAlFinal(materias, 17, "Redes de Informacion", "Sistemas Operativos", "Si", "No");
    agregarAlFinal(materias, 17, "Redes de Informacion", "Comunicaciones", "Si", "No");
    agregarAlFinal(materias, 18, "Administracion de Recursos", "Diseno de Sistemas", "Si", "Si");
    agregarAlFinal(materias, 18, "Administracion de Recursos", "Bases de Datos", "Si", "No");
    agregarAlFinal(materias, 19, "Proyecto Final", "Ingenieria de Software", "Si", "Si");
    agregarAlFinal(materias, 19, "Proyecto Final", "Redes de Informacion", "Si", "Si");
    agregarAlFinal(materias, 19, "Proyecto Final", "Administracion de Recursos", "Si", "Si");
    agregarAlFinal(materias, 19, "Proyecto Final", "Bases de Datos", "Si", "Si");

    cargarArchivo(materias);

    cout << "\nPresione ENTER para salir...";
    cin.ignore();
    cin.get();
    return 0;
}
