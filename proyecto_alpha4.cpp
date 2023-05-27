#include <iostream>
#include <string.h>
#include<fstream>
#include<cstdlib>//funcion para el exit

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
    bool check;

    Relacion(){
        ping=0;
        con_quien=NULL;
        siguiente_R=NULL;
        check=0;
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
                host1->conectar(ping_1,tipo_1,host2); //Establezco la relación en cada uno de los dispositivos
                host2->conectar(ping_1,tipo_1,host1);
                return 1;    
            }
        }
    }

    return 0;
};
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

void eliminarRelacion (Relacion *&listado, string nombre){ //PPPPPPPPPPENDIENTEEEEEEEEEEEmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
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
        Nodo *siguiente;
        int indice;
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

    void agregar_ruta() {
        Nodo *nuevo = new Nodo(); //Crea un nodo
        //nuevo->dispositivo=new string [500]; //Le asigna el apuntador al dispositivo
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


};


Cola Rutas;//Creo un arreglo para esta solucion


void back(Dispositivo *actual, Dispositivo *objetivo, Dispositivo *inicio, Dispositivo* dedonde){
    cout<<"Comienzo back. Actual "<<actual->hostname<<"| Objetivo "<<objetivo->hostname<<"| inicio "<<inicio->hostname<<endl;
    
     
    string *ruta_actual = Rutas.final->dispositivo;//Copio la direccion del arreglo de string actual
    if(ruta_actual[Rutas.final->indice-1] != actual->hostname){
        ruta_actual[Rutas.final->indice]=actual->hostname; //Guardo el nombre del dispositivo en la ruta actual (Usando el indice)
        Rutas.final->indice++;//Incremento el indice
    }
        

    Relacion *auxiliar=actual->lista_vecinos; //Establezco un auxiliar para recorrer a los vecinos

    while(auxiliar->con_quien!=NULL){ //Mientras que no llegue al final de los vecinos...
        if(auxiliar->con_quien==objetivo){ //Si llegue al objetivo chill
            ruta_actual[Rutas.final->indice]=auxiliar->con_quien->hostname; //Guardo el nombre del dispositivo en la ruta actual (Usando el indice)
            Rutas.final->indice++; //Incremento el indice
            
            for(int i=0;i<Rutas.final->indice;i++){
                cout<<"("<<ruta_actual[i]<<")"<<"->";
            }
            cout<<endl;

            Rutas.agregar_ruta(); //Creo una nueva ruta para almacenar
            string *aux1=ruta_actual;
            string *ruta_actual = Rutas.final->dispositivo;
    
            ruta_actual[Rutas.final->indice]=inicio->hostname; //Preparo el apuntador a la ruta actual  
            Rutas.final->indice++;
            if(ruta_actual[Rutas.final->indice-1] != actual->hostname){
                ruta_actual[Rutas.final->indice]=actual->hostname; //Guardo el nombre del dispositivo en la ruta actual (Usando el indice)
                Rutas.final->indice++;//Incremento el indice
            }
            
            break;//salgo del bucle
        }
        else if (auxiliar->siguiente_R!=NULL){
            auxiliar=auxiliar->siguiente_R; //Si no es el objetivo continuo revisando
        }
        else if(auxiliar->siguiente_R==NULL){
            break;
        }
    }
    
    Relacion *aux=actual->lista_vecinos; //Creo un auxiliar para iterar en la lista de vecinos del actual

    for(int i=1; i<=actual->cont_relacion;i++){
        
        if(aux->con_quien != objetivo && !aux->check){
            if(aux->con_quien == inicio || aux->con_quien == dedonde){
                aux=aux->siguiente_R;
            }
            else{
                cout<<actual->hostname<<"--"<<i<<endl;

                aux->check=true;
                back(aux->con_quien,objetivo, inicio, actual);
                
                aux=aux->siguiente_R;
            }
            
        }
        
    }
    aux=actual->lista_vecinos;
    for(int i=1; i<=actual->cont_relacion;i++){
        aux->check=false;
        aux=aux->siguiente_R;
    }

    return;
};

void buscarRutas(Dispositivo* origen, Dispositivo* objetivo) {
    Rutas.agregar_ruta();
    back(origen, objetivo, origen, NULL);
}

///-------------------------------------------------------/// 
void M_agregar_info(int &entrada){
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computación\n Orquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Agregar dispositivo.\n2.\t Agregar ruta.\n3.\t Volver a Principal.\n4.\t Salir de la aplicación."<<endl;
    string hostname,ip;
    string h2,tipo;

    int ping;
    bool flag; //Indicador de error de las funciones

    while(true){
        cout<<"Opción: ";
        cin>>entrada;
        switch(entrada){
            case 1:
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
                cin>>hostname>>h2>>ping>>tipo;
                flag=establecer_conexion(ping,tipo,hostname,h2);
                if(flag){
                    cout<<"Se agregó una ruta entre "<<hostname<<" y "<<h2<<endl;    
                }
                else{
                    cout<<"No fue posible establecer la conexión\n"; 
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
    cout<<" Universidad central de venezuela\nFacultad de Ciencias\nEscuela de Computación\nOrquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Eliminar dispositivo.\n2.\t Eliminar ruta.\n3.\t Volver a Principal.\n4.\t Salir de la aplicación."<<endl;
    string hostname;

    while(true){
        cout<<"Opción: ";
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
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computación\n Orquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Consultar dispositivo.\n2.\t Listado de Dispositivos.\n3.\t Buscar ruta(1: 5G, 2: fibra óptica, 3: ambas).\n4.\t Dispositivos adyacentes.\n5.\t Volver a Principal.\n6.\t Salir de la aplicación."<<endl;
    Dispositivo *buscado;
    string host_ip;

    while(true){
        cout<<"Opción: ";
        cin>>entrada;        
        switch(entrada){
            case 1:
                cin>>host_ip;
                buscado=buscardispositivo(host_ip);
                cout<<buscado->hostname<<", "<<buscado->ip;
                break;
            case 2:
                mostrarlista();
                break;
            case 3:
                //------------PUTO BACKTRAKING----------///
                break;
            case 4:
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
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computación\n Orquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Listado de dispositivos existentes.\n2.\t Listado de dispositivos eliminados.\n3.\t Listado de rutas eliminadas.\n4.\t Volver a Principal.\n5.\t Salir de la aplicación.";
    
    string host,ip;

    while(true){
        cout<<"Opción: ";
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
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computación\n Orquestador de Redes de conectividad\n\nMenú de opciones:"<<endl;
    cout<<"1.\t Agregar información.\n2.\t Eliminar información.\n3.\t Buscar y listar.\n4.\t Mostrar respaldos.\n5.\t Créditos.\n6.\t Salir de la aplicación."<<endl;
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
            cout<<"Carlos Cao \tC.I. 28.655.925\nErimar Reis \tC.I. 29.743.464\n";
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
    
    string name[14]={"a","b","c","d","e","f","g","h","i","k","l","m","n","o"};
    string dire[14]={"10","17","15","12","05","11","18","16","13","06","14","25","19","01"};
    for(int i=0; i<14; i++){
        //cout<<"Ingresa valor: \t"; 
        //cin>>valor;
        insertar_dispositivo(name[i],dire[i]);
    }
    establecer_conexion(1,"f","a","b");
    establecer_conexion(2,"f","b","c");
    establecer_conexion(3,"f","c","d");
    establecer_conexion(4,"f","d","e");
    //establecer_conexion(5,"f","e","b");
    //establecer_conexion(6,"f","e","c",lista);
    establecer_conexion(7,"f","a","g");
    establecer_conexion(8,"f","f","g");
    establecer_conexion(9,"f","e","f");
    establecer_conexion(10,"f","b","g");

    mostrarrelacion(buscardispositivo("a")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo("b")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo("c")->lista_vecinos);cout<<endl;
    /*mostrarrelacion(buscardispositivo(lista,"d")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"e")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"f")->lista_vecinos);cout<<endl;*/
    //mostrarrelacion(buscardispositivo("g")->lista_vecinos);cout<<endl;
    /*mostrarrelacion(buscardispositivo(lista,"h")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"i")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"k")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"l")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"m")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"n")->lista_vecinos);cout<<endl;
    mostrarrelacion(buscardispositivo(lista,"o")->lista_vecinos);cout<<endl;*/

     buscarRutas(buscardispositivo("a"),buscardispositivo("g"));
    cout<<endl;
    mostrarrelacion(buscardispositivo("b")->lista_vecinos);cout<<endl;

    //mostrarlista();
    
    /*while(true){
        M_inicio();
    }*/

    return 0;
}