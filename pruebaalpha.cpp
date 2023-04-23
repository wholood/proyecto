#include <iostream>
#include <string.h>
//novale
using namespace std;
class Dispositivo; //Cabecera para que no se glichee

struct Relacion{ 
    int ping;
    int saltos;
    string tipo;
    Dispositivo *con; //Con que dispositivo existe la conexion
    Relacion *siguiente_R; //Puntero para la lista de relaciones de cada dispositivo
};

class Dispositivo{
public:
    string hostname;
    string ip;
    int cont_relacion; //Contador de relaciones existentes
    Relacion *lista_vecinos; //Literalmente lista de las conexiones
    Dispositivo *siguiente_D; //Puntero para la lista de dispositivos del main
public:
    Dispositivo(){
        cont_relacion=0; //Por si acaso
    };
    void conectar (int salto_1, int ping_1, string tipo_1, Dispositivo *pana){
        //Crea una nueva relacion en la lista de relaciones del dispositivo.

        Relacion *nueva_Relacion = new Relacion(); 
        nueva_Relacion->saltos =salto_1;
        nueva_Relacion->ping =ping_1;
        nueva_Relacion->tipo =tipo_1;

        nueva_Relacion->con =pana;

        Relacion *aux1 = lista_vecinos; //Variable auxiliar para el ordenamiento
        Relacion *aux2;

        //ESTA PENDIENDTE EL ORDENAMIENTO POR TIPO
        while((aux1!=NULL)&&(aux1->ping <ping_1)){ //Verificar si hay algun ping menor al de la nueva relacion y establecer el auxiliar para agregar el elemento en medio
            aux2=aux1;
            aux1=aux1->siguiente_R;
        }

        if(lista_vecinos == aux1){ //Insertar al inicio, es decir no entro en el while
            lista_vecinos = nueva_Relacion; //Establece la relacion al inicio de la lista
        }
        else{
            aux2->siguiente_R = nueva_Relacion; //Establece la nueva relacion en medio de la lista
        }

        nueva_Relacion->siguiente_R=aux1; //se cumple siempre.
        cont_relacion++; //Incremento el contador de relacion.

        cout<<"Conexion establecida con "<<pana->hostname<<"."<<endl; //mensajito jiji
    };
};

///--------------------------------------------------------///
void insertarlista(Dispositivo *&lista, string nombre, string direccion){

    Dispositivo *nuevo_Dispositivo = new Dispositivo();
    nuevo_Dispositivo->hostname =nombre;
    nuevo_Dispositivo->ip =direccion;

    Dispositivo *aux1 = lista;
    Dispositivo *aux2;

    while((aux1!=NULL)&&(aux1->ip <direccion)){ //Verificar si hay alguna ip menor a n y establecer el auxiliar para agregar el elemento en medio
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

Dispositivo* buscardispositivo(Dispositivo *lista, string busqueda){
    //Dentro de lista dispositivo buscar el string del hostname o ip
	Dispositivo *actual = lista; //Auxiliar para no romper nada

	while ((actual !=NULL)&&((actual->hostname != busqueda)||(actual->ip != busqueda))){
		actual = actual->siguiente_D; //Voy recorriendo la lista siempre y cuando no consiga nada hasta el final (NULL)
	}

    return ((actual->hostname==busqueda)||(actual->ip==busqueda))? actual : NULL;
    //Retorno *actual si lo encontre, si no encontre nada retorno el puntero en NULL
};

bool verificar_relacion (Dispositivo *h1, Dispositivo *h2){
    Relacion *actual = h1->lista_vecinos; //Auxiliar pa no romper nada

    while ((actual !=NULL)&&(actual->con!=h2)){ //Recorro la lista de relaciones del dispositivo mientras no consiga h2 hasta el final
		actual = actual->siguiente_R;
	}

    return (actual == NULL)? true : false;
    //Si llego al final de la lista retorno que no hay relacion existente (TRUE) si no llegue al final consegui h2 por ende (FALSE)
};

void establecer_conexion(int salto_1, int ping_1, string tipo_1, string h_1, string h_2, Dispositivo *listado){
    Dispositivo *host1 = buscardispositivo(listado,h_1); //Dado un nombre (host o ip) consigo la direccion del dispositivo en la lista
    Dispositivo *host2 = buscardispositivo(listado,h_2);

    if (host1!=NULL && host2!=NULL){//Mientras los apuntadores no sean nulos, si son nulos no existe el dispositivo
        if(verificar_relacion(host1,host2)){ //Verifica que no exista relacion entre ambos dispositivos
            if(host1->cont_relacion<12 && host2->cont_relacion<12){ //Verifica que tengan espacio para conexion
                host1->conectar(salto_1,ping_1,tipo_1,host2); //Establezco la relación en cada uno de los dispositivos
                host2->conectar(salto_1,ping_1,tipo_1,host1);    
            }
        }
    }
};

///-------------------------------------------------------///
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