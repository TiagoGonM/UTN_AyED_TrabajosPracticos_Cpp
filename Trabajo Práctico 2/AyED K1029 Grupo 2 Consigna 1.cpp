#include <iostream>
#include <cstring>

#define CANTIDAD_CORREDORES 1000

#define LARGO_CAMPO_POSICIONES 16
#define LARGO_CAMPO_ID 6
#define LARGO_CAMPO_GENERO 10
#define LARGO_CAMPO_NOMBRE 40
#define LARGO_CAMPO_CATEGORIA 50
#define LARGO_CAMPO_LOCALIDAD 20
#define LARGO_CAMPO_TIEMPOS 20


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


struct HeadersInforme {
    char posGral[LARGO_CAMPO_POSICIONES] = "";
    char posGenero[LARGO_CAMPO_POSICIONES + 1] = "";
    char posCat[LARGO_CAMPO_POSICIONES] = "";
    char corredorId[LARGO_CAMPO_ID + 1] = "";

    char nombreApellido[LARGO_CAMPO_NOMBRE] = "";
    char categoria[LARGO_CAMPO_CATEGORIA + 1] = "";
    char genero[LARGO_CAMPO_GENERO + 1] = "";
    char localidad[LARGO_CAMPO_LOCALIDAD] = "";
    char total[LARGO_CAMPO_TIEMPOS] = "";
    char difPrimero[LARGO_CAMPO_TIEMPOS] = "";
    char difAnterior[LARGO_CAMPO_TIEMPOS] = "";
};

struct RegInforme {
    char posGral[LARGO_CAMPO_POSICIONES] = ""; // debe figurar como "Pos. Gral."
    char posGenero[LARGO_CAMPO_POSICIONES] = ""; // debe figurar como "Pos. Género"
    char posCat[LARGO_CAMPO_POSICIONES] = ""; // debe figurar como "Pos. Cat."
    
    char corredorId[LARGO_CAMPO_ID] = ""; // debe figurar como "N°"

    char nombreApellido[LARGO_CAMPO_NOMBRE] = "";
    char categoria[LARGO_CAMPO_CATEGORIA] = "";
    char genero[LARGO_CAMPO_GENERO] = "";
    char localidad[LARGO_CAMPO_LOCALIDAD] = "";

    char total[LARGO_CAMPO_TIEMPOS] = ""; // debe figurar como "Total"
    char difPrimero[LARGO_CAMPO_TIEMPOS] = ""; // debe figurar como "Diferencia primero"
    char difAnterior[LARGO_CAMPO_TIEMPOS] = ""; // debe figurar como "Diferencia anterior"
};

void establecerLargoCampo(char dest[], int destBuf, const char src[]) {
    strcpy(dest, src);
    
    for (int i = strlen(src); i < destBuf - 1; i++) {
        dest[i] = ' ';
    }
    dest[destBuf - 1] = '\0';
}

void establecerLargoCampo(char dest[], int destBuf, const char src) {
    dest[0] = src;

    for (int i = 1; i < destBuf - 1; i++) {
        dest[i] = ' ';
    }
    dest[destBuf - 1] = '\0';
}

void establecerLargoCampo(char dest[], int destBuf, int src) {
    snprintf(dest, destBuf, "%d", src); // easy int to char[] conversion
    
    for (int i = strlen(dest); i < destBuf - 1; i++) {
        dest[i] = ' ';
    }
    dest[destBuf - 1] = '\0';
}

void establecerLargoCampoCentrado(char dest[], int destBuf, const char src[]) {
    int srcLen = strlen(src);
    int availableSpace = destBuf - 1;

    if (srcLen >= availableSpace) {
        strcpy(dest, src);
    }
    
    int spacesToInsert = availableSpace - srcLen;
    int paddingLeft = spacesToInsert / 2;
    int paddingRight = spacesToInsert - paddingLeft; // No siempre seran ambos lados simétricos

    int idx = 0;
    for (int i = 0; i < paddingLeft; i++) {
        dest[idx++] = ' ';
    }

    
    for (int i = 0; i < srcLen; i++) {
        dest[idx++] = src[i];
    }

    for (int i = 0; i < paddingRight; i++) {
        dest[idx++] = ' ';
    }

    dest[availableSpace] = '\0';
}

void establecerLargoCampoCentrado(char dest[], int destBuf, const char src) {
    int availableSpace = destBuf - 1;
    
    int spacesToInsert = availableSpace - 1;
    int paddingLeft = spacesToInsert / 2;
    int paddingRight = spacesToInsert - paddingLeft; // No siempre seran ambos lados simétricos

    int idx = 0;
    for (int i = 0; i < paddingLeft; i++) {
        dest[idx++] = ' ';
    }

    dest[idx++] = src;

    for (int i = 0; i < paddingRight; i++) {
        dest[idx++] = ' ';
    }

    dest[availableSpace] = '\0';
}

void establecerLargoCampoCentrado(char dest[], int destBuf, int src) {
    char aux[6] = "";
    snprintf(aux, destBuf, "%d", src);
    int srcLen = strlen(aux);

    int availableSpace = destBuf - 1;

    if (srcLen >= availableSpace) {
        strcpy(dest, aux);
    }
    
    int spacesToInsert = availableSpace - srcLen;
    int paddingLeft = spacesToInsert / 2;
    int paddingRight = spacesToInsert - paddingLeft; // No siempre seran ambos lados simétricos

    int idx = 0;
    for (int i = 0; i < paddingLeft; i++) {
        dest[idx++] = ' ';
    }
    
    for (int i = 0; i < srcLen; i++) {
        dest[idx++] = aux[i];
    }

    for (int i = 0; i < paddingRight; i++) {
        dest[idx++] = ' ';
    }

    dest[availableSpace] = '\0';
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
    if (llegada[0] == 'D') return -1; // Contempla DNF y DSP + variantes
    int h = (llegada[0]-'0')*10 + (llegada[1]-'0');
    int m = (llegada[3]-'0')*10 + (llegada[4]-'0');
    int s = (llegada[6]-'0')*10 + (llegada[7]-'0');
    int d = (llegada[9]-'0');
    return ((h*3600 + m*60 + s)*10 + d);
}

void establecerLargos(HeadersInforme& headers) {
    establecerLargoCampoCentrado(headers.posGral, LARGO_CAMPO_POSICIONES, "Pos. Gral.");
    establecerLargoCampoCentrado(headers.posGenero, LARGO_CAMPO_POSICIONES + 1, "Pos. Género."); // Tildes ocupan 2 bytes también
    establecerLargoCampoCentrado(headers.posCat, LARGO_CAMPO_POSICIONES, "Pos. Cat.");
    establecerLargoCampoCentrado(headers.corredorId, LARGO_CAMPO_ID + 1, "N°"); // "°" ocupa 2 bytes
    establecerLargoCampoCentrado(headers.nombreApellido, LARGO_CAMPO_NOMBRE, "Nombre");
    establecerLargoCampoCentrado(headers.categoria, LARGO_CAMPO_CATEGORIA + 1, "Categoría");
    establecerLargoCampoCentrado(headers.genero, LARGO_CAMPO_GENERO + 1, "Género");
    establecerLargoCampoCentrado(headers.localidad, LARGO_CAMPO_LOCALIDAD, "Localidad");
    establecerLargoCampoCentrado(headers.total, LARGO_CAMPO_TIEMPOS, "Total");
    establecerLargoCampoCentrado(headers.difPrimero, LARGO_CAMPO_TIEMPOS, "Diferencia primero");
    establecerLargoCampoCentrado(headers.difAnterior, LARGO_CAMPO_TIEMPOS, "Diferencia anterior");
}

RegInforme generarRegistroInforme(
    RegCorredores corredor, 
    int posGral,
    int posGenero,
    int posCat,
    const char total[], 
    const char difPrimero[], 
    const char difAnterior[]
) {
    RegInforme reg;

    establecerLargoCampoCentrado(reg.posGral, LARGO_CAMPO_POSICIONES, posGral);
    establecerLargoCampoCentrado(reg.posGenero, LARGO_CAMPO_POSICIONES, posGenero);
    establecerLargoCampoCentrado(reg.posCat, LARGO_CAMPO_POSICIONES, posCat);

    establecerLargoCampo(reg.corredorId, LARGO_CAMPO_ID, corredor.numero);
    
    establecerLargoCampo(reg.nombreApellido, LARGO_CAMPO_NOMBRE, corredor.nombreApellido);
    establecerLargoCampo(reg.categoria, LARGO_CAMPO_CATEGORIA, corredor.categoria);
    establecerLargoCampoCentrado(reg.genero, LARGO_CAMPO_GENERO, corredor.genero);
    establecerLargoCampo(reg.localidad, LARGO_CAMPO_LOCALIDAD, corredor.localidad);

    establecerLargoCampoCentrado(reg.total, LARGO_CAMPO_TIEMPOS, total);
    establecerLargoCampoCentrado(reg.difPrimero, LARGO_CAMPO_TIEMPOS, difPrimero);
    establecerLargoCampoCentrado(reg.difAnterior, LARGO_CAMPO_TIEMPOS, difAnterior);

    return reg;
}

void generarInforme(FILE* file, RegCorredores v[], int n) {
    HeadersInforme headers;

    establecerLargos(headers);

    // fwrite(id, sizeof(id - 1), 1, file);
    // fwrite(nombreApellido, sizeof(nombreApellido - 1), 1, file);
    // fwrite(total, sizeof(total - 1), 1, file);
    cout << headers.posGral 
         << headers.posGenero 
         << headers.posCat
         << headers.corredorId
         << headers.nombreApellido 
         << headers.categoria 
         << headers.genero
         << headers.localidad 
         << headers.total 
         << headers.difPrimero 
         << headers.difAnterior
         << endl;

    for (int i = 0; i < 5; i++) {
        RegInforme reg = generarRegistroInforme(
            v[i],
            -1,
            -1,
            -1,
            tiempoADecimas(v[i].llegada) == -1 ? "No termino" : v[i].llegada,
            "-1",
            "-1"
        );

        cout << reg.posGral 
             << reg.posGenero 
             << reg.posCat
             << reg.corredorId
             << reg.nombreApellido 
             << reg.categoria 
             << reg.genero 
             << reg.localidad 
             << reg.total 
             << reg.difPrimero 
             << reg.difAnterior
             << endl;

        // fwrite(id, sizeof(id), 1, file);
        // fwrite(nombre, sizeof(nombre), 1, file);
        // fwrite(total, sizeof(total), 1, file);
    }
}

// Convierte decimas a "HH:MM:SS.D" . Si es -1 devuelve "DNF".
void pasajeDecimasACadena(int decimasTotal, char destino[]) {
    
    if (decimasTotal == -1) {
        strcpy(destino, "No termino");
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

void setIfEmpty(char dest[], const char src[]) {
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
    strcat(rutaInformeClasica, ".bin");

    cout << "Ruta resultante: " << rutaInformeClasica << endl << endl;
    
    cout << "Ingrese ruta donde se generará y nombre del informe de la Carrera 'NonStop' [sin extensión] (Predeterminado: <carpeta raíz>/" << NOMBRE_INFORME_NONSTOP_DEFAULT << "): ";
    cin.getline(rutaInformeNonStop, nonStopSize);
    setIfEmpty(rutaInformeNonStop, NOMBRE_INFORME_NONSTOP_DEFAULT);
    strcat(rutaInformeNonStop, ".bin");

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

    FILE* fListadoClasica = fopen(rutaArchivoInformeClasica, "wb");
    generarInforme(fListadoClasica, clasica, nC);
    fclose(fListadoClasica);

    FILE* fListadoNonStop = fopen(rutaArchivoInformeNonStop, "wb");
    generarInforme(fListadoNonStop, nonstop, nC);
    fclose(fListadoNonStop);
}
