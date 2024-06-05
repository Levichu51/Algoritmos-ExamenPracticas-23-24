#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

void inicializar_semilla();
void aleatorio(int v[], int n);
void aleatorioAscendente(int v[], int n);
void aleatorioDescendente(int v[], int n);
void imprimirVector(int v[], int n);
int esOrdenado(int v[], int n);
double microsegundos();

void calculosTiempos(int n, void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n), double time);
void printCabecera(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n));
void analizarTiempos(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n),int num_sizes);

void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v [], int n) {
    int i, m = 2*n + 1;
    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void aleatorioAscendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++)
        v[i] = i;
}

void aleatorioDescendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++)
        v[i] = n - i - 1;
}

void imprimirVector(int v[], int n) {
    printf("[");
    int i;
    for (i = 0; i < n; ++i) {
        printf("%d ", v[i]);
    }
    printf("]  \t");
}

int esOrdenado(int v[], int n){
    if (n <= 1) {
        return 1;
    }

    for (int i = 1; i < n; i++) {
        if (v[i] < v[i - 1]) {
            return 0;
        }
    }

    return 1;
}

double microsegundos(){
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void auxOrd(int v[], int left, int right){
    int pivote;
    int i, j;
    int temp;
    int aux;
    int aux2;

    if(left < right){
        pivote = v[left + 1];
        aux = v[left];
        v[left] = v[left + 1];
        v[left + 1] = aux;
        //intercambiar v[left], v[left + 1]
        i = left + 1;
        j = right;

        while(i <= j){
            while(i <= right && v[i] < pivote){
                i++;
            }
            while(v[j] > pivote){
                j--;
            }
            if (i <= j) {
                temp = v[i];
                v[i] = v[j];
                v[j] = temp;
                i++;
                j--;
            }
        }

        auxOrd(v, left, j);
        auxOrd(v, i, right);

    }
}

void ordenar(int v[], int n){
    auxOrd(v, 0, n - 1);
}

void analizarTiempos(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n), int n) {
    inicializar_semilla();
    int k;
    double elapsed_time1, start_time, end_time, t1, t2;
    int *v;

    v = malloc(n * sizeof (int));

    generador(v, n);

    start_time = microsegundos();
    ordenacion(v, n);
    end_time = microsegundos();
    elapsed_time1 = end_time - start_time;

    if (elapsed_time1 < 500) {
        printf("(*)");
        start_time = microsegundos();

        for (k = 0; k < 100; k++) {
            generador(v, n);
            ordenacion(v, n);
        }
        end_time = microsegundos();
        t1 = end_time - start_time;
        start_time = microsegundos();

        for (k = 0; k < 100; k++) {
            generador(v, n);
        }
        end_time = microsegundos();
        t2 = end_time - start_time;

        elapsed_time1 = (t1 - t2) / 100;
        calculosTiempos(n, ordenacion, generador, elapsed_time1);

    } else {
        calculosTiempos(n, ordenacion, generador, elapsed_time1);
    }
    free(v);
}

void calculosTiempos(int n, void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n), double time){
    double time1, time2, time3;

    if(generador == aleatorioAscendente) {
            time1 = time / pow(n, 1.8);
            time2 = time / pow(n, 1.985);
            time3 = time / pow(n, 2.2);

    }else if (generador == aleatorioDescendente){
            time1 = time / pow(n, 1.8);
            time2 = time / pow(n, 2);
            time3 = time / pow(n, 2.2);
    }else{
        time1 = time / pow(n, 0.8);
        time2 = time / (pow(n,1.1));
        time3 = time / pow(n, 1.2);
    }

    printf("\t% 8d\t\t% 15.3lf\t\t% 14.8lf\t\t% 14.8lf\t\t% 14.8lf\n", n, time, time1, time2, time3);

}

void printCabecera(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n)){

    if(generador == aleatorioAscendente) {
            printf("\t       n\t\t\t  t(n)\t\t   t(n) / n^1.8\t\t   t(n) / n^1.985\t           t(n) / n^2.2\n");
    }else if(generador == aleatorioDescendente){
            printf("\t       n\t\t\t  t(n)\t\t   t(n) / n^1.8\t\t   t(n) / n^2\t           t(n) / n^2.2\n");
    }else{
        printf("\t       n\t\t\t  t(n)\t\t   t(n) / n^0.8\t\t   t(n) / n^1.1\t           t(n) / n^1.2\n");
    }

}

void test(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n), int n) {
    int v[n];

    generador(v, n);
    imprimirVector(v, n);

    printf("\n");

    if (esOrdenado(v, n)) {
        printf("ordenado? 1\n");
    } else {
        printf("ordenado? 0\n");
    }

    ordenacion(v, n);

    printf("Ordenacion\n");
    imprimirVector(v, n);
    printf("\n");

    if (esOrdenado(v, n)) {
        printf("ordenado? 1\n");
    } else {
        printf("ordenado? 0\n");
    }
}

void AuxOrd(){
    int i;

    printf("Ordenacion Aleatoria\n");
    printCabecera(ordenar, aleatorio);
    for(i = 500; i <= 64000; i = i * 2){
        analizarTiempos(ordenar, aleatorio, i);
    }
    printf("\n\n");

    printf("Ordenacion Ascendente\n");
    printCabecera(ordenar, aleatorioAscendente);
    for(i = 500; i <= 64000; i = i * 2){
        analizarTiempos(ordenar, aleatorioAscendente, i);
    }
    printf("\n\n");

    printf("Ordenacion Descendente\n");
    printCabecera(ordenar, aleatorioDescendente);
    for(i = 500; i <= 64000; i = i * 2){
        analizarTiempos(ordenar, aleatorioDescendente, i);
    }
    printf("\n\n");


}



int main() {
    ///test
    printf("Inicializacion aleatoria auxOrd\n");
    test(&ordenar, &aleatorio, 10);
    printf("\n\n");
    printf("Inicializacion descendente auxOrd\n");
    test(&ordenar, &aleatorioDescendente, 10);
    printf("\n\n");
    printf("Inicializacion ascendente auxOrd\n");
    test(&ordenar, &aleatorioAscendente, 10);

    printf("\n\n\n");

    ///Análisis del algoritmo auxOrd
    AuxOrd();
    AuxOrd();
















    return 0;
}

