#include <iostream>
#include <string.h>

using namespace std;
struct Relacion;

struct Dispositivo{
    string hostname;
    string ip;
    int cont_relacion;
    Relacion *lista_vecinos; //Literalmente lista de las conexiones
    Dispositivo *siguiente_D; //Puntero para la lista de dispositivos del main
};

struct Relacion{
    int ping;
    int saltos;
    string tipo;
    Dispositivo *con;
    Relacion *siguiente_R; //Puntero para la lista de relaciones de cada dispositivo
};


void insertarlista(Dispositivo *&lista, string nombre, string direccion){
    Dispositivo *nuevo_Dispositivo = new Dispositivo();
    nuevo_Dispositivo->hostname =nombre;
    nuevo_Dispositivo->ip =direccion;

    Dispositivo *aux1 = lista;
    Dispositivo *aux2;

    while((aux1!=NULL)&&(aux1->ip <direccion)){ //Verificar si hay algun hostname menor a n y establecer el auxiliar para agregar el elemento en medio
        aux2=aux1;
        aux1=aux1->siguiente_D;
    }

    if(lista ==aux1){ //Insertar al inicio, es decir no entro en el while
        lista = nuevo_Dispositivo;
    }
    else{
        aux2->siguiente_D = nuevo_Dispositivo;
    }
    nuevo_Dispositivo->siguiente_D=aux1; //se cumple siempre.
    cout<<"Valor ingresado "<<nombre<<"."<<endl;
}

void mostrarlista(Dispositivo *lista){
    Dispositivo *actual = lista;
    //actual = lista;
    while(actual != NULL){
        cout<<actual->ip<<"||"<<actual->hostname<<"-> ";
        actual= actual->siguiente_D;
    }
};

Dispositivo* buscardispositivo(string busqueda){
    //Dentro de lista dispositivo buscar el string del hostname o ip
};
void conectar (int salto_1, int ping_1, string tipo_1, string nombre_conexion){
    Relacion *nueva_Relacion = new Relacion();
    nueva_Relacion->saltos =salto_1;
    nueva_Relacion->ping =ping_1;
    nueva_Relacion->tipo =tipo_1;

    Dispositivo *pana = buscardispositivo(nombre_conexion);
    nueva_Relacion->con =pana;
/*
    Dispositivo *aux1 = lista;
    Dispositivo *aux2;

    while((aux1!=NULL)&&(aux1->ip <direccion)){ //Verificar si hay algun hostname menor a n y establecer el auxiliar para agregar el elemento en medio
        aux2=aux1;
        aux1=aux1->siguiente_D;
    }

    if(lista ==aux1){ //Insertar al inicio, es decir no entro en el while
        lista = nuevo_Dispositivo;
    }
    else{
        aux2->siguiente_D = nuevo_Dispositivo;
    }
    nuevo_Dispositivo->siguiente_D=aux1; //se cumple siempre.
    cout<<"Valor ingresado "<<nombre<<"."<<endl;*/
};

int main (){
    Dispositivo *lista = NULL;
    string name[5]={"a","b","c","d","e"};
    string dire[5]={"172.16.1.1","172.16.5.1","172.18.1.100","172.16.111.1","172.15.2.1"};
    for(int i=0; i<5; i++){
        //cout<<"Ingresa valor: \t"; 
        //cin>>valor;
        insertarlista(lista,name[i],dire[i]);
    }
    mostrarlista(lista);

    return 0;
}