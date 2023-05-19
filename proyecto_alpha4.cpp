#include <iostream>
#include <string.h>

using namespace std;
class Dispositivo; //Cabecera para que no se glichee

class Relacion{
public: 
    int ping;
    string tipo;
    Dispositivo *con_quien; //Con que dispositivo existe la conexion
    Relacion *siguiente_R; //Puntero para la lista de relaciones de cada dispositivo

    Relacion(){
        ping=0;
        con_quien=NULL;
        siguiente_R=NULL;
    };
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
        lista_vecinos=NULL;
    };
    void conectar (int ping_1, string tipo_1, Dispositivo *pana){
        //Crea una nueva relacion en la lista de relaciones del dispositivo.

        Relacion *nueva_Relacion = new Relacion(); 
        nueva_Relacion->ping =ping_1;
        nueva_Relacion->tipo =tipo_1;

        nueva_Relacion->con_quien =pana;

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

    while((aux1!=NULL)&&(aux1->hostname <nombre)){ //Verificar si hay alguna ip menor a n y establecer el auxiliar para agregar el elemento en medio
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
        cout<<"("<<actual->ip<<"|"<<actual->hostname<<")-> ";
        actual= actual->siguiente_D;
    }
};
void mostrarrelacion(Relacion *lista){
    Relacion *actual = lista;
    //actual = lista;
    while(actual != NULL){
        cout<<"("<<actual->ping<<"|"<<actual->tipo<<"|"<<actual->con_quien->hostname<<")-> ";
        actual= actual->siguiente_R;
    }
};

Dispositivo* buscardispositivo(Dispositivo *lista, string busqueda){
    //Dentro de lista dispositivo buscar el string del hostname o ip
	Dispositivo *actual = lista; //Auxiliar para no romper nada

	while ((actual !=NULL)&&(actual->hostname != busqueda)&&(actual->ip != busqueda)){
		actual = actual->siguiente_D; //Voy recorriendo la lista siempre y cuando no consiga nada hasta el final (NULL)
	}
    if(actual==NULL) return NULL;

    return ((actual->hostname==busqueda)||(actual->ip==busqueda))? actual : NULL;
    //Retorno *actual si lo encontre, si no encontre nada retorno el puntero en NULL
};

bool verificar_relacion (Dispositivo *h1, Dispositivo *h2){
    Relacion *actual = h1->lista_vecinos; //Auxiliar pa no romper nada

    while ((actual !=NULL)&&(actual->con_quien!=h2)){ //Recorro la lista de relaciones del dispositivo mientras no consiga h2 hasta el final
		actual = actual->siguiente_R;
	}

    return (actual == NULL)? true : false;
    //Si llego al final de la lista retorno que no hay relacion existente (TRUE) si no llegue al final consegui h2 por ende (FALSE)
};

void establecer_conexion(int ping_1, string tipo_1, string h_1, string h_2, Dispositivo *listado){
    Dispositivo *host1 = buscardispositivo(listado,h_1); //Dado un nombre (host o ip) consigo la direccion del dispositivo en la lista
    Dispositivo *host2 = buscardispositivo(listado,h_2);

    if (host1!=NULL && host2!=NULL && host1!=host2){//Mientras los apuntadores no sean nulos, si son nulos no existe el dispositivo
        if(verificar_relacion(host1,host2)){ //Verifica que no exista relacion entre ambos dispositivos
            if(host1->cont_relacion<12 && host2->cont_relacion<12){ //Verifica que tengan espacio para conexion
                host1->conectar(ping_1,tipo_1,host2); //Establezco la relación en cada uno de los dispositivos
                host2->conectar(ping_1,tipo_1,host1);    
            }
        }
    }
    return;
};
///-------------------------------------------------------///

void eliminarDispositivo (Dispositivo *&lista, string nombre){
    if(lista!=NULL){ //Por si acaso
        Dispositivo  *aux_borrar = buscardispositivo(lista,nombre);
        Dispositivo  *anterior = lista;
        
        if(anterior==aux_borrar){
            lista=aux_borrar->siguiente_D;
        }
        else{
            while ((aux_borrar != NULL)&&(anterior->siguiente_D != aux_borrar)){
                anterior=anterior->siguiente_D; //Voy recorriendo la lista siempre y cuando no consiga nada hasta el final (NULL)
            }
            anterior->siguiente_D=aux_borrar->siguiente_D;
        }
        //respaldar el dispositivo a borrar
        delete aux_borrar;
    }
}

void eliminarRelacion (Relacion *&lista, string nombre){ //PPPPPPPPPPENDIENTEEEEEEEEEEEmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
    if(lista!=NULL){ //Por si acaso
        Relacion  *aux_borrar ;//BUSCARRELACION
        Relacion  *anterior = lista;
        
        //Modificacion de la lista de relaciones del dispositivo
        if(anterior==aux_borrar){
            lista=aux_borrar->siguiente_R;
        }
        else{
            while ((aux_borrar != NULL)&&(anterior->siguiente_R != aux_borrar)){
                anterior=anterior->siguiente_R; //Voy recorriendo la lista siempre y cuando no consiga nada hasta el final (NULL)
            }
            anterior->siguiente_R=aux_borrar->siguiente_R;
        }
        //respaldar la relacion a borrar
        delete aux_borrar;
    }
}

///-------------------------------------------------------/// BUSQUEDA DE RUTAS DIABOLICAS
class Cola {
public:

struct Nodo {
    Dispositivo *dispositivo;
    Cola *Solu;
    Nodo *siguiente;
};
    Nodo *frente;
    Nodo *final;
    int aux_ping;

    Cola(){ //Constructor para la cola vacia
        frente=NULL; 
        final=NULL;
        aux_ping=0;
    }

    bool vacia() { //Chequea si la cola no tiene elementos
        return frente == NULL;
    }

    void agregar_dispCola(Dispositivo *dispositivo) {
        Nodo *nuevo = new Nodo(); //Crea un nodo
        nuevo->dispositivo=dispositivo; //Le asigna el apuntador al dispositivo
        nuevo->siguiente=NULL; //Establece como null el siguiente (fin de cola)

        if (final != NULL) {
            final->siguiente = nuevo; //Añadimos a nuevo atras del ultimo de la cola
        }
        final = nuevo; //Nuevo será el final de la cola
        if (frente == NULL) {
            frente = nuevo; //Si la cola esta vacia entonces nuevo va a ser el principio y el fin
        }
    }
    void agregar_colaCola(Cola *cola) {
        Nodo *nuevo = new Nodo(); //Crea un nodo
        Cola aux;

        nuevo->Solu=cola; //Le asigna el apuntador al dispositivo
        nuevo->siguiente=NULL; //Establece como null el siguiente (fin de cola)

        if (final != NULL) {
            final->siguiente = nuevo; //Añadimos a nuevo atras del ultimo de la cola
        }
        final = nuevo; //Nuevo será el final de la cola
        if (frente == NULL) {
            frente = nuevo; //Si la cola esta vacia entonces nuevo va a ser el principio y el fin
        }
    }

    void clonar(Cola *ruta){
        Cola aux;
        aux

    };

    void sacar_cola() {
        if (frente != NULL) { //Si el frente NO es nulo
            Nodo *aux = frente; //Creo un auxiliar al elemento
            frente = frente->siguiente; //Establezco como primero al que iba atras del del frente
            if (frente == NULL) { //Si solo estaba el elemento entonces todo es nulo
                final = NULL;
            }
            delete aux; //Elimino el elemento
        }
    }

    Dispositivo *quien_al_frente() {
        return frente->dispositivo; //Me dice que dispositivo esta al frente de la cola
    }
    Cola *solucion_al_frente(){
        return frente->Solu; //Me dice cual es la cola de relaciones que esta al frente.
    }
};

Cola Soluciones;

void back(Dispositivo *actual, Dispositivo *objetivo, Cola *soluciones){
    Cola Ruta;
    Ruta.agregar_dispCola(actual);

    Relacion *auxiliar=actual->lista_vecinos;

    while(auxiliar->con_quien!=NULL){
        if(auxiliar->con_quien==objetivo){
            Ruta.agregar_dispCola(auxiliar->con_quien);
            Soluciones.agregar_colaCola(Ruta);
        }
        

    }
    
};

///-------------------------------------------------------/// 
int main (){
    Dispositivo *lista = NULL;
    string name[14]={"a","b","c","d","e","f","g","h","i","k","l","m","n","o"};
    string dire[14]={"10","17","15","12","05","11","18","16","13","06","14","25","19","01"};
    for(int i=0; i<14; i++){
        //cout<<"Ingresa valor: \t"; 
        //cin>>valor;
        insertarlista(lista,name[i],dire[i]);
    }
    establecer_conexion(1,"f","a","b",lista);
    establecer_conexion(2,"f","a","c",lista);
    establecer_conexion(3,"f","a","g",lista);
    establecer_conexion(4,"f","b","c",lista);
    establecer_conexion(5,"f","c","g",lista);

    mostrarrelacion(buscardispositivo(lista,"a")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"b")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"c")->lista_vecinos);cout<<endl;
    /*mostrarrelacion(buscardispositivo(lista,"d")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"e")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"f")->lista_vecinos);cout<<endl;*/
    mostrarrelacion(buscardispositivo(lista,"g")->lista_vecinos);cout<<endl;
    /*mostrarrelacion(buscardispositivo(lista,"h")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"i")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"k")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"l")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"m")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"n")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"o")->lista_vecinos);cout<<endl;*/

    cout<<endl;
    mostrarlista(lista);

    return 0;
}