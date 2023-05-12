#include <iostream>
#include <time.h>
using namespace std;
int const N = 4; //Numero de cables
int const MAX = 100; //Máxima longitud

void imprimir(int arreglo[]);

bool valido(int solucion[], int nivel, int cable[]) {
    //suma la logitud de las soldaduras, si es menor o igual al maximo true.
    int longitudesactuales = 0;
    for(int i = 0; i < N; i++){
        if(solucion[i] == 1){
            longitudesactuales += cable[i];
        }
    }
    return (longitudesactuales <= MAX);
}
void actualizarSolucion(int solucion[], int nivel, int cable[], int resultado[], int &soldaduras_finales) {
    int soldaduras_totales = 0;
    int longitudtotal = 0;

    for(int i = 0; i < N; i++){
        if(solucion[i] == 1){
            soldaduras_totales++;
        }
        longitudtotal += cable[i] * solucion[i];
    }
    if(soldaduras_totales < soldaduras_finales && longitudtotal == MAX){
        for(int i = 0; i < N; i++){
            resultado[i] = solucion[i];
        }
        soldaduras_finales = soldaduras_totales;
    }
}

void back(int solucion[], int nivel, int cable[], int resultado[], int &soldaduras_finales) {
    int i = 0;
    if (nivel > N-1) return; //llego al final de una rama salgo de la ejecución de back
    do{
        solucion[nivel] = i; //le asigna 0 al cable actual
        if(valido(solucion, nivel, cable)){ //valida si estoy por debajo o igual a la longitud maxima
            if(nivel == N-1){ //Si llegue al final de la rama
                actualizarSolucion(solucion, nivel, cable, resultado, soldaduras_finales);
            } else { //si no, meto mas cables
                back(solucion, nivel+1, cable, resultado, soldaduras_finales);
            }
        }
        i++; //hago i=1 porque si llegué aqui entonces reviso que pasaba si tomaba que es una solucion
    } while (solucion[nivel] !=1); 
    solucion[nivel] = -1; //pongo el estado de la solución en nulo
} 
void imprimir (int solucion[],int cables[]){
    for(int i=0; i<N;i++){
        if(solucion[i]==1){
            cout<<"Cable "<<i+1<<": "<<cables[i]<<endl;
        }
    }
};
int main (){
    srand(time(0));
    int cables [N];
    for (int i = 0; i < N; i++){
        cin>>cables[i];
    }
    
    int solucion [N], resultado[N];
    int soldaduras = MAX;
    int nivel=0;

    back(solucion, nivel, cables, resultado, soldaduras);

    cout<<endl<<"-----------"<<endl<<"finalmente"<<endl;
    imprimir(resultado, cables);

    return 0;    
}