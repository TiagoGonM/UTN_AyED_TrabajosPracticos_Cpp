#include <iostream>
#include <cstring>

#define CANTIDAD_CORREDORES 1000
#define LARGO_CAMPO 40
#define NOMBRE_ARCHIVO_CORREDORES_DEFAULT "Archivo corredores 4Refugios"
#define NOMBRE_INFORME_CLASICA_DEFAULT "Informe Carrera Clásica"
#define NOMBRE_INFORME_NONSTOP_DEFAULT "Informe Carrera NonStop"

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

void establecerLargoCampo(char dest[], int destBuf, const char src[]) {
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

// Convierte "HH:MM:SS.D" a decimas. Si es DNF devuelve -1.
int tiempoADecimas(char llegada[]) {
    if (llegada[0] == 'D') return -1;
    int h = (llegada[0]-'0')*10 + (llegada[1]-'0');
    int m = (llegada[3]-'0')*10 + (llegada[4]-'0');
    int s = (llegada[6]-'0')*10 + (llegada[7]-'0');
    int d = (llegada[9]-'0');
    return ((h*3600 + m*60 + s)*10 + d);
}

void mostrarInforme(RegCorredores v[], int n) {
    cout << "N  Nombre                    Total" << endl;
    for (int i = 0; i < n; i++) {
        char nombre[LARGO_CAMPO] = "";
        establecerLargoCampo(nombre, LARGO_CAMPO, v[i].nombreApellido);

        char t[15];
        if (tiempoADecimas(v[i].llegada) == -1)
            strcpy(t, "No Termino");
        else
            strcpy(t, v[i].llegada);

        cout << v[i].numero << " " << nombre << " " << t << endl;
    }
}

// Convierte decimas a "HH:MM:SS.D" . Si es -1 devuelve "DNF".
void pasajeDecimasACadena(int decimasTotal, char destino[]) {
    
    if (decimasTotal == -1) {
        strcpy(destino, "DNF");
        return;
    }

    int horas = decimasTotal / 36000;
    decimasTotal = decimasTotal - horas * 36000;

    int minutos = decimasTotal / 600;
    decimasTotal = decimasTotal - minutos * 600;

    int segundos = decimasTotal / 10;
    int decimas = decimasTotal % 10;

    destino[0] = horas / 10 + '0';
    destino[1] = horas % 10 + '0';
    destino[2] = ':';

    destino[3] = minutos / 10 + '0';
    destino[4] = minutos % 10 + '0';
    destino[5] = ':';

    destino[6] = segundos / 10 + '0';
    destino[7] = segundos % 10 + '0';
    destino[8] = '.';

    destino[9] = decimas + '0';
    destino[10] = '\0';
}

// Ordena por tiempo, los -1 van al final
void ordenar(RegCorredores v[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            int t1 = tiempoADecimas(v[j].llegada);
            int t2 = tiempoADecimas(v[j+1].llegada);
            
            if (t1 == -1) t1 = 99999999;
            if (t2 == -1) t2 = 99999999;
            
            if (t1 > t2) {
                RegCorredores aux = v[j];
                v[j] = v[j+1];
                v[j+1] = aux;
            }
        }
    }
}

void setIfEmpty(char dest[], char src[]) {
    if (strlen(dest) > 0) return;
    strcpy(dest, src);
}

void loadData(char ruta[], int rutaSize, char rutaInformeClasica[], int clasicaSize, char rutaInformeNonStop[], int nonStopSize) {
    cout << "Ingrese ruta y nombre del archivo .bin de corredores [sin extensión] (Predeterminado: <carpeta raíz>/" << NOMBRE_ARCHIVO_CORREDORES_DEFAULT << "): ";
    cin.getline(ruta, rutaSize);
    setIfEmpty(ruta, NOMBRE_ARCHIVO_CORREDORES_DEFAULT);
    strcat(ruta, ".bin");
    
    cout << "Ruta resultante: " << ruta << endl << endl;

    cout << "Ingrese ruta donde se generará y nombre del informe de la Carrera 'Clásica' [sin extensión] (Predeterminado: <carpeta raíz>/" << NOMBRE_INFORME_CLASICA_DEFAULT << "): ";
    cin.getline(rutaInformeClasica, clasicaSize);
    setIfEmpty(rutaInformeClasica, NOMBRE_INFORME_CLASICA_DEFAULT);
    strcat(rutaInformeClasica, ".txt");

    cout << "Ruta resultante: " << rutaInformeClasica << endl << endl;
    
    cout << "Ingrese ruta donde se generará y nombre del informe de la Carrera 'NonStop' [sin extensión] (Predeterminado: <carpeta raíz>/" << NOMBRE_INFORME_NONSTOP_DEFAULT << "): ";
    cin.getline(rutaInformeNonStop, nonStopSize);
    setIfEmpty(rutaInformeNonStop, NOMBRE_INFORME_NONSTOP_DEFAULT);
    strcat(rutaInformeNonStop, ".txt");

    cout << "Ruta resultante: " << rutaInformeNonStop << endl << endl;
}

int main() {
    RegCorredores corredores[CANTIDAD_CORREDORES] = {};
    RegCorredores clasica[CANTIDAD_CORREDORES], nonstop[CANTIDAD_CORREDORES];
    RegCorredores reg;

    char rutaArchivoCorredores[300] = "";
    char rutaArchivoInformeClasica[80] = "";
    char rutaArchivoInformeNonStop[80] = "";

    loadData(rutaArchivoCorredores, 300, rutaArchivoInformeClasica, 80, rutaArchivoInformeNonStop, 80);

    FILE* fCorredores = fopen(rutaArchivoCorredores, "rb");
    leerCorredores(corredores, fCorredores);
    fclose(fCorredores);

    int nC = 0, nN = 0;
    for (int i = 0; i < CANTIDAD_CORREDORES; i++) {
        if (corredores[i].numero == 0) break;

        if (strstr(corredores[i].categoria, "Clasica") != NULL)
            clasica[nC++] = corredores[i];
        else
            nonstop[nN++] = corredores[i];
    }

    ordenar(clasica, nC);
    ordenar(nonstop, nN);

    cout << "CLASICA:" << endl;
    mostrarInforme(clasica, nC);

    cout << "NONSTOP:" << endl;
    mostrarInforme(nonstop, nN);
}
