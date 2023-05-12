#include <iostream>

using namespace std;
class Dispositivo; //Cabecera para cola_controlue no se glichee

struct Relacion{ 
    int ping;
    string tipo;
    Dispositivo *con; //Con cola_controlue dispositivo existe la conexion
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
    void conectar (int ping_1, string tipo_1, Dispositivo *pana){
        //Crea una nueva relacion en la lista de relaciones del dispositivo.

        Relacion *nueva_Relacion = new Relacion(); 
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

class Cola {
public:
    struct Nodo {
        Dispositivo *dispositivo;
        Nodo *siguiente;
    };

    Nodo *frente;
    Nodo *final;

    Cola(){ //Constructor para la cola vacia
        frente=NULL; 
        final=NULL;
    }

    bool vacia() { //Chequea si la cola no tiene elementos
        return frente == NULL;
    }

    void agregar_cola(Dispositivo *dispositivo) {
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
};

class lista_visitado {
public:
    struct Nodo {
        Dispositivo *disp_clave; //Dispositivo al que apunta
        bool valor; //fue revisado?
        Nodo *siguiente;
    };

    Nodo *inicio; //Principio de la lista

    lista_visitado(){
        inicio=NULL; //inicializa en nulo
    }

    bool contiene(Dispositivo *disp_buscado) { //Verifica si el dispositivo que estamos buscando se encuentra en la lista
        Nodo *actual = inicio;
        while (actual != NULL) {
            if (actual->disp_clave == disp_buscado) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    void insertar(Dispositivo *disp_buscado, bool valor2) { //Si un dispositivo no esta en la lista entonces lo añado a los ya revisados
        Nodo *nuevo = new Nodo{disp_buscado, valor2, inicio};
        inicio = nuevo;
    }
};

void BFS(Dispositivo *inicio, Dispositivo *objetivo) {
    Cola cola_control;
    lista_visitado visitados;

    cola_control.agregar_cola(inicio); //Dispositivo ruta
    visitados.insertar(inicio, true); //Decimos que el primer elemento fue verificado

    while (!cola_control.vacia()) { //Mientras existan elementos que revisar
        Dispositivo *actual = cola_control.quien_al_frente(); //Tomamos el dispositivo que esta al frente
        cola_control.sacar_cola(); //Como ya tengo el puntero al dispositivo en actual entonces lo puedo sacar de la cola. 

        if (actual == objetivo) {
            cout << "Ruta encontrada" << endl;
            //-------------------- Imprime o almacena la ruta encontrada
            return;
        }

        Relacion *relacion_actual = actual->lista_vecinos; //Tomo toda la lista de vecinos del dispositivo actual para analizar
        while (relacion_actual != NULL) { //Siempre que tenga conexiones para verificar..
            Dispositivo *vecino = relacion_actual->con; //Extrae el dispositivo 
            if (!visitados.contiene(vecino)) { //si el dispositivo no esta revisado
                cola_control.agregar_cola(vecino); //Agrego el dispositivo a la cola para revisar
                visitados.insertar(vecino, true); //Lo marco como revisado
            }
            relacion_actual = relacion_actual->siguiente_R; //Paso al siguiente dispositivo
        }
    }

    cout << "No se encontró una ruta de conexión entre los dispositivos" << endl;
}