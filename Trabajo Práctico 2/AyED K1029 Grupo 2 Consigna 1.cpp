#include <iostream>
#include <cstring>

#define CANTIDAD_CORREDORES 1000
#define TAMAÑO_CAMPO 40
#define NOMBRE_ARCHIVO "Archivo corredores 4Refugios.bin"

using namespace std;

struct RegCorredores {
    int numero;

    char nombreApellido[50];
    char categoria[50]; // 4 Refugios <clásica/nonstop> - <DAMAS/CABALLEROS> (<rango de edad>)
    char genero; // M / F
    char localidad[40];
    char llegada[11]; // DNF, DNF (NL), DSP (FE) o el tiempo de llegada (formato HH:MM:SS.D)
};

struct RegInforme {
    int corredorId; // debe figurar como "N°"

    int posGral; // debe figurar como "Pos. Gral."
    int posGenero; // debe figurar como "Pos. Género"
    int posCat; // debe figurar como "Pos. Cat."
    int tiempoTotal; // debe figurar como "Total"
};

void establecerTamaño(char dest[], int destBuf, const char src[]) {
    strcpy(dest, src);
    
    for (int i = strlen(src); i < destBuf; i++) {
        dest[i] = ' ';
    }
    dest[destBuf - 1] = '\0';
}

void leerCorredores(RegCorredores corredores[], FILE* file) {
    RegCorredores reg;
    int i = 0;

    while (fread(&reg, sizeof(RegCorredores), 1, file)) {
        corredores[i++] = reg;
    };
}

void mostrarInforme(RegCorredores* corredores) {
}

int main() {
    RegCorredores corredores[CANTIDAD_CORREDORES] = {};
    RegCorredores reg;

    char path[100] = ""; 
    strcat(path, NOMBRE_ARCHIVO); // "<ruta>/<archivo>"
    
    FILE* fCorredores = fopen(NOMBRE_ARCHIVO, "rb");
    leerCorredores(corredores, fCorredores);
    fclose(fCorredores);

    // Ejemplo de uso
    // char str[TAMAÑO_CAMPO] = "";
    // establecerTamaño(str, TAMAÑO_CAMPO, reg.nombreApellido);
    // cout << str << "separado" << endl;
}
