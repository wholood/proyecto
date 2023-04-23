#include <iostream>

using namespace std;

struct Nodo {
    int dato;
    Nodo *siguiente;
};

void insertarlista(Nodo *&lista,int n){
    Nodo *nuevo_nodo = new Nodo();
    nuevo_nodo->dato =n;

    Nodo *aux1 = lista;
    Nodo *aux2;

    while((aux1!=NULL)&&(aux1->dato <n)){ //Verificar si hay algun dato menor a n y establecer el auxiliar para agregar el elemento en medio
        aux2=aux1;
        aux1=aux1->siguiente;
    }

    if(lista ==aux1){ //Insertar al inicio, es decir no entro en el while
        lista = nuevo_nodo;
    }
    else{
        aux2->siguiente = nuevo_nodo;
    }
    nuevo_nodo->siguiente=aux1; //se cumple siempre.
    cout<<"Valor ingresado "<<n<<"."<<endl;
}

void mostrarlista(Nodo *lista){
    Nodo *actual = lista;
    //actual = lista;
    while(actual != NULL){
        cout<<actual->dato<<"->";
        actual= actual->siguiente;
    }
};

int main (){
    Nodo *lista = NULL;
    int valor[10]={9,4,5,8,10,1,6,7,2,3};
    for(int i=0; i<10; i++){
        //cout<<"Ingresa valor: \t"; 
        //cin>>valor;
        insertarlista(lista,valor[i]);
    }
    mostrarlista(lista);

    return 0;
}