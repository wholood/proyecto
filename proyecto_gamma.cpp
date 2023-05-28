#include <iostream>
#include <string.h>
#include<fstream>
#include<cstdlib>//funcion para el exit
#include<windows.h>//funciona para el clear

using namespace std;



class Dispositivo; //Cabecera para que no se glichee
Dispositivo *lista = NULL;

Dispositivo* buscardispositivo(string busqueda);

ofstream EscRutas_Resp("rutas_resp.dat.txt"), EscDispositivos_resp("Dispositivos_resp.dat");
ifstream LeeRutas_Resp("rutas_resp.dat.txt"), LeeDispositivos_resp("Dispositivos_resp.dat");
//“Dispositivos.dat” 
//rutas_resp.dat
//Dispositivos_resp.dat


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
bool insertar_dispositivo(string nombre, string direccion){
    if(buscardispositivo(nombre)==NULL){
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
        return 1;
    }
    return 0;
    //----------------------Linea para error por dispositivo duplicado----------------------------------------------//
}

void mostrarlista(){
    Dispositivo *actual = lista;
    int i = 1;
    //actual = lista;
    while(actual != NULL){
        cout<<i<<": "<<actual->hostname<<", "<<actual->ip<<endl;
        actual= actual->siguiente_D;
        i++;
    }
};
void mostrarrelacion(Relacion *listado){
    Relacion *actual = listado;
    //actual = lista;
    while(actual != NULL){
        cout<<"("<<actual->ping<<"|"<<actual->tipo<<"|"<<actual->con_quien->hostname<<")-> ";
        actual= actual->siguiente_R;
    }
};

Dispositivo* buscardispositivo(string busqueda){
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

bool establecer_conexion(int ping_1, string tipo_1, string h_1, string h_2){
    Dispositivo *host1 = buscardispositivo(h_1); //Dado un nombre (host o ip) consigo la direccion del dispositivo en la lista
    Dispositivo *host2 = buscardispositivo(h_2);

    if (host1!=NULL && host2!=NULL && host1!=host2){//Mientras los apuntadores no sean nulos, si son nulos no existe el dispositivo
        if(verificar_relacion(host1,host2)){ //Verifica que no exista relacion entre ambos dispositivos
            if(host1->cont_relacion<12 && host2->cont_relacion<12){ //Verifica que tengan espacio para conexion
                host1->conectar(ping_1,tipo_1,host2); //Establezco la relacion en cada uno de los dispositivos
                host2->conectar(ping_1,tipo_1,host1);
                return 1;    
            }
        }
    }

    return 0;
};

string tipo_conexion(string inicio, string busqueda){
    Dispositivo *h1=buscardispositivo(inicio);
    Dispositivo *h2= buscardispositivo(busqueda);
    Relacion *actual = h1->lista_vecinos; //Auxiliar pa no romper nada

    while ((actual !=NULL)&&(actual->con_quien!=h2)){ //Recorro la lista de relaciones del dispositivo mientras no consiga h2 hasta el final
		actual = actual->siguiente_R;
	}

    return (actual->tipo);
    
}
///-------------------------------------------------------///

void eliminarDispositivo (string nombre){
    if(lista!=NULL){ //Por si acaso
        Dispositivo  *aux_borrar = buscardispositivo(nombre);
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

void eliminarRelacion (Relacion *&listado, string nombre){//PPPPPPPPPPENDIENTEEEEEEEEEEEmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
    if(lista!=NULL){ //Por si acaso
        Relacion  *aux_borrar ;//BUSCARRELACION
        Relacion  *anterior = listado;
        
        //Modificacion de la lista de relaciones del dispositivo
        if(anterior==aux_borrar){
            listado=aux_borrar->siguiente_R;
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
        string dispositivo[500];
        int indice;
        Nodo *siguiente;
        string tipo;
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

    void agregar_ruta() {
        Nodo *nuevo = new Nodo(); //Crea un nodo
        nuevo->indice=0;
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

    int imprimir(int indicador){//1=fibra, 2=aereo
        int f=0, a=0, m=0;
        while(frente !=NULL){
            if(indicador==1 && frente->tipo=="Fibra"){
                for(int i=0; i<=frente->indice;i++){
                    cout<<"("<<frente->dispositivo[i]<<") "<<"->";
                }
                cout<<endl;
                f++;
                sacar_cola();   
            }
            else if(indicador==2 && frente->tipo=="Aereo"){
                for(int i=0; i<=frente->indice;i++){
                    cout<<"("<<frente->dispositivo[i]<<") "<<"->";
                }
                cout<<endl;
                a++;
                sacar_cola();   
            }
            else if(indicador==0){
                for(int i=0; i<=frente->indice;i++){
                    cout<<"("<<frente->dispositivo[i]<<") "<<"->";
                }
                cout<<endl;
                m++;
                sacar_cola();   
            }
                
        }
        if(indicador==1) return f;
        if(indicador==2) return a;
        return m;
    }

    void verificar_tipo(){
        Nodo * auxiliar = frente;
        while(auxiliar!=NULL){
            int  f=0,a=0;
            for(int i=0;i<auxiliar->indice;i++){
                if(tipo_conexion(auxiliar->dispositivo[i],auxiliar->dispositivo[i+1]) == "Fibra"){
                    f++;
                }else if(tipo_conexion(auxiliar->dispositivo[i],auxiliar->dispositivo[i+1])== "Aereo"){
                    a++;
                }
            }
            if(f > 0 && a > 0){
                auxiliar->tipo="Mixto";
            }else if(a == 0){
                auxiliar->tipo="Fibra";
            }else{
                auxiliar->tipo="Aereo";
            }
            auxiliar = auxiliar->siguiente;
        }           
    }
};

Cola soluciones;

class Pila{
    public:
    struct Nodo{
        string Disp;
        Nodo* Siguiente;
    };

    Nodo* StackPointer;


    Pila(){
       StackPointer = NULL;
    };


    void push(string subir){
        Nodo *nuevo = new Nodo(); //Crea un nodo

        nuevo->Disp=subir;
        nuevo->Siguiente=StackPointer;

        StackPointer=nuevo;
    }   
    void pop(){
        Nodo *borrar = StackPointer;
        StackPointer = borrar->Siguiente;

        delete borrar;
    }
    bool Verificar(string Buscado){
        Nodo* aux = StackPointer; 
        while(aux != NULL){
            if(aux->Disp == Buscado){
                return true;
            }
            aux = aux->Siguiente;
        }
        return false;
    }
    void almacenar_solucion(string objetivo){
        Nodo* aux = StackPointer;
        soluciones.agregar_ruta();
        string *arreglo = soluciones.final->dispositivo;

        int i = 0;
        while(aux != NULL){
            aux = aux->Siguiente;
            i++;
        }
        aux = StackPointer;
        arreglo[i]=objetivo;

        for(int j=i-1; j>=0; j--){
            arreglo[j]=aux->Disp;
            aux = aux->Siguiente;
        }
        soluciones.final->indice=i;
    }
    
};


Pila almacen; //Creo un arreglo para esta solucion

void Back(Dispositivo *Actual, Dispositivo *Objetivo){

    Relacion *ReActual = Actual->lista_vecinos;
    while(ReActual != NULL){
        if(!almacen.Verificar(ReActual->con_quien->hostname)){
            if(ReActual->con_quien->hostname == Objetivo->hostname){
                almacen.almacenar_solucion(Objetivo->hostname);
            }
            else{
                almacen.push(ReActual->con_quien->hostname);
                Back(ReActual->con_quien,Objetivo);
            }
        }
        ReActual =ReActual->siguiente_R;
    }
    almacen.pop();
}

void buscarRutas(Dispositivo* origen, Dispositivo* objetivo) {
    almacen.push(origen->hostname);
    Back(origen,objetivo);
    soluciones.verificar_tipo();
}
//----------------------------------------------------// Refinamiento de soluciones---------------

///-------------------------------------------------------/// 
void M_agregar_info(int &entrada){
    system("cls");
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computacion\n Orquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Agregar dispositivo.\n2.\t Agregar ruta.\n3.\t Volver a Principal.\n4.\t Salir de la aplicacion."<<endl;
    string hostname,ip;
    string h2,tipo;

    int ping;
    bool flag; //Indicador de error de las funciones

    while(true){
        cout<<"Opcion: ";
        cin>>entrada;
        switch(entrada){
            case 1:
                cout<<"Ingresa el hostname seguido de la ip: "<<endl;
                cin>>hostname>>ip;
                flag=insertar_dispositivo(hostname,ip);
                if(flag){
                    cout<<"El dispositivo "<<hostname<<" ha sido agregado\n";    
                }
                else{
                    cout<<"El dispositivo "<<hostname<<" ya existe en el sistema. No fue agregado\n"; 
                }
                break;
            case 2:
                cout<<"Ingresa el hostname1, hostname2, ping, tipo: "<<endl;
                cin>>hostname>>h2>>ping>>tipo;
                flag=establecer_conexion(ping,tipo,hostname,h2);
                if(flag){
                    cout<<"Se agrego una ruta entre "<<hostname<<" y "<<h2<<endl;    
                }
                else{
                    cout<<"No fue posible establecer la conexion\n"; 
                }
                break;
            case 3:
                return;
            case 4:
                exit(0);

        }    
    }
    
};

void M_eliminar_info(int &entrada){
    system("cls");
    cout<<" Universidad central de venezuela\nFacultad de Ciencias\nEscuela de Computacion\nOrquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Eliminar dispositivo.\n2.\t Eliminar ruta.\n3.\t Volver a Principal.\n4.\t Salir de la aplicacion."<<endl;
    string hostname;

    while(true){
        cout<<"Opcion: ";
        cin>>entrada;
        switch(entrada){
        case 1:
            cin>>hostname;
            if(buscardispositivo(hostname)== NULL){
                cout<<"El dispositivo "<<hostname<<" No existe en el sistema\n";
                break;
            }
            // -------Respaldar-----------//
            //                            //
            //---------Eliminar-----------//


            break;
        case 2:
            //---------Eliminar Ruta------//
            break;
        case 3:
            return;
        case 4:
            exit(0);
    }    
    }
    
};

void M_buscar_listar(int &entrada){
    system("cls");
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computacion\n Orquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Consultar dispositivo.\n2.\t Listado de Dispositivos.\n3.\t Buscar ruta(1: 5G, 2: fibra optica, 3: ambas).\n4.\t Dispositivos adyacentes.\n5.\t Volver a Principal.\n6.\t Salir de la aplicacion."<<endl;
    Dispositivo *buscado;
    string host_ip, h2, tipo;
    int aux=0;

    while(true){
        cout<<"Opcion: ";
        cin>>entrada;        
        switch(entrada){
            case 1:
                cout<<"Ingrese Hostname o IP"<<endl;
                cin>>host_ip;
                buscado=buscardispositivo(host_ip);
                cout<<buscado->hostname<<", "<<buscado->ip<<endl;
                break;
            case 2:
                mostrarlista();
                break;
            case 3:
                cout<<"Ingrese los identificadores:"<<endl;
                cin>>host_ip>>h2;
                buscarRutas(buscardispositivo(host_ip),buscardispositivo(h2));
                cout<<"Ingrese el tipo de ruta que desea:"<<endl;
                cin>>tipo;
                
                if(tipo=="Fibra" || tipo=="FibraOptica" || tipo=="fibra"){
                    aux=soluciones.imprimir(1);
                    cout<<"numero de rutas encontradas: "<<aux<<endl;
                }
                else if (tipo=="5G" || tipo=="5g" || tipo=="Aereo" || tipo=="aereo"){
                    aux=soluciones.imprimir(2);
                    cout<<"numero de rutas encontradas: "<<aux<<endl;
                }
                else{
                    aux=soluciones.imprimir(0);
                    cout<<"numero de rutas encontradas: "<<aux<<endl;
                }
                break;
            case 4:
                cout<<"Ingresa el hostname o ip: "<<endl;
                cin>>host_ip;
                mostrarrelacion(buscardispositivo(host_ip)->lista_vecinos);
                break;
            case 5:
                return;
            case 6:
                exit(0);
        }
    }
        
};

void M_respaldar(int &entrada){
    system("cls");
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computacion\n Orquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Listado de dispositivos existentes.\n2.\t Listado de dispositivos eliminados.\n3.\t Listado de rutas eliminadas.\n4.\t Volver a Principal.\n5.\t Salir de la aplicacion.";
    
    string host,ip;

    while(true){
        cout<<"Opcion: ";
        cin>>entrada;
        switch(entrada){
            case 1: 
                mostrarlista();
                break;
            case 2:
                //Muestra el contenido del archivo de respaldo Dispositivo_resp.dat
                if(LeeDispositivos_resp.is_open()){
                    while(LeeDispositivos_resp.eof()){
                        LeeDispositivos_resp>>host;
                        LeeDispositivos_resp>>ip;
                        cout<< host <<", "<< ip<< endl;
                    } 
                }
                break;
            case 3:
                
                break;
            case 4:
                return;
            case 5:
                exit(0);
        }    
    }
    

};

void M_inicio(){
    int entrada;
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computacion\n Orquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Agregar informacion.\n2.\t Eliminar informacion.\n3.\t Buscar y listar.\n4.\t Mostrar respaldos.\n5.\t Créditos.\n6.\t Salir de la aplicacion."<<endl;
    cin>>entrada;

    switch(entrada){
        case 1:
            M_agregar_info(entrada);
            break;
        case 2:
            M_eliminar_info(entrada);
            break;
        case 3:
            M_buscar_listar(entrada);
            break;
        case 4:
            M_respaldar(entrada);
            break;
        case 5:
            cout<<endl<<endl<<"---------"<<endl;
            cout<<"Carlos Cao \tC.I. 28.655.925\nErimar Reis \tC.I. 29.743.464\n";
            cout<<endl<<endl<<"---------"<<endl;
            break;
        case 6:
            exit(0);
    }
};

void Inicio(){
    ifstream Entrada("Dispositivo.dat.txt");
    
    if(Entrada.is_open()){
        int Dis = 0, Rel = 0, ping = 0;
        string aux1,aux2,aux3;
        Entrada>>Dis;
        for(int i = 0; i < Dis; i++){
            Entrada>>aux1;//Host
            Entrada>>aux2;//ip
            insertar_dispositivo(aux1,aux2);
        }
        Entrada>>Rel;
        for(int i = 0; i < Rel; i++){
            Entrada>>aux1;//host1
            Entrada>>aux2;//host2
            Entrada>>aux3;//tipo
            Entrada>>ping;//ping
            establecer_conexion(ping,aux3,aux1,aux2);
        }
    }
    Entrada.close();
}



///-------------------------------------------------------///
int main (){
    
    /*string name[14]={"a","b","c","d","e","f","g","h","i","k","l","m","n","o"};
    string dire[14]={"10","17","15","12","05","11","18","16","13","06","14","25","19","01"};
    for(int i=0; i<14; i++){
        //cout<<"Ingresa valor: \t"; 
        //cin>>valor;
        insertar_dispositivo(name[i],dire[i]);
    }
    establecer_conexion(141,"Fibra","a","b");
    establecer_conexion(134,"Fibra","a","c");
    establecer_conexion(144,"Fibra","a","d");
    establecer_conexion(132,"Fibra","c","b");
    establecer_conexion(121,"Fibra","b","d");*/
    //establecer_conexion(1,"f","e","c");
    //establecer_conexion(1,"f","e","b");
    //establecer_conexion(1,"f","d","c");
    //establecer_conexion(1,"f","c","b");
    //establecer_conexion(1,"f","b","g");
/*
    mostrarrelacion(buscardispositivo("a")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo("b")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo("c")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"d")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"e")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"f")->lista_vecinos);cout<<endl;*/
    //mostrarrelacion(buscardispositivo("g")->lista_vecinos);cout<<endl;
    /*mostrarrelacion(buscardispositivo(lista,"h")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"i")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"k")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"l")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"m")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"n")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"o")->lista_vecinos);cout<<endl;

    int aux;
    
    buscarRutas(buscardispositivo("a"),buscardispositivo("d"));
    aux=soluciones.imprimir(1);
    cout<<"numero de rutas encontradas: "<<aux<<endl;
 
    cout<<endl;
*/
    Inicio();

    while(true){
        M_inicio();
    }

    return 0;
}