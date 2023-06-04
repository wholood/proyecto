#include <iostream>
#include <string>
#include<fstream>
#include<cstdlib>//funcion para el exit

using namespace std;
//-----------Cabeceras-----------------//
class Dispositivo; //Cabecera para que no se glichee
int tipo_conexion_unaria(string h1, string h2);
int obtener_ping(string h1, string h2);
void Respaldar_Rutas(string h1, string h2);
Dispositivo* buscardispositivo(string busqueda);
bool buscarRutas(string h1, string h2);
string rut_resp = "rutas_resp.dat",Disp_resp ="Dispositivos_resp.dat";
fstream Rutas_resp(rut_resp,ios::in | ios:: out);
fstream Dispositivo_resp(Disp_resp, ios::in | ios:: out);
fstream Salidas("resultados.out", ios:: out);

//-----------------------///
int ContDisp = 0, ContRel = 0;
Dispositivo *lista = NULL;

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
    };

    void desconectar(Dispositivo *host){
        Relacion  *aux= lista_vecinos ;
        Relacion  *anterior=NULL;

        while((aux!=NULL)&&(aux->con_quien != host)){
            anterior = aux;
            aux =aux->siguiente_R;
        }

        if(aux == NULL){
            cout<<"<<Elemento no encontrado>>";
        }

        else if(anterior == NULL){
            lista_vecinos =lista_vecinos->siguiente_R;
            //Respaldar_Rutas_relacion(aux);
            delete aux;
        }
        else{
            anterior->siguiente_R = aux->siguiente_R;
            //Respaldar_Rutas_relacion(aux);
            delete aux;
        }
        //Respaldar_Rutas la relacion a borrar
        //delete aux;
        cont_relacion--;
    }
};

class Cola {
    public:

    struct Nodo {
        string dispositivo[501];
        int indice;
        Nodo *siguiente;
        int tipo;
        int ping_total;
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
        nuevo->ping_total = 0;
    
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

    int imprimir_desencolar(){//1=fibra, 2=aereo
        int m=0;
        while(frente !=NULL){           
            for(int i=0; i<=frente->indice;i++){
                cout<<frente->dispositivo[i]<<", ";
                Salidas<<frente->dispositivo[i]<<", ";
            }
            cout<<"\tPing: "<<frente->ping_total;
            cout<<"\tSaltos: "<<frente->indice;
            cout<<"\tTipo: "<<frente->tipo<<endl;

            Salidas<<"Ping: "<<frente->ping_total<<", ";
            Salidas<<"Saltos: "<<frente->indice<<", ";
            Salidas<<"Tipo: "<<frente->tipo<<endl;        
            m++;
            sacar_cola();                
        }
        return m;
    }

    void imprimir(){
        Nodo *aux = frente;
        while(aux !=NULL){
            for(int i=0; i<=aux->indice;i++){
                cout<<aux->dispositivo[i]<<", ";
            }
            cout<<"\tPing: "<<aux->ping_total<<endl;
            cout<<"\tSaltos: "<<frente->indice<<endl;
            aux=aux->siguiente;   
        }
    }

    void verificar_tipo(){
        Nodo * auxiliar = frente;
        while(auxiliar!=NULL){
            int  f=0,a=0;
            for(int i=0;i<auxiliar->indice;i++){
                if(tipo_conexion_unaria(auxiliar->dispositivo[i],auxiliar->dispositivo[i+1]) == 1){
                    f++;
                }else if(tipo_conexion_unaria(auxiliar->dispositivo[i],auxiliar->dispositivo[i+1])== 2){
                    a++;
                }
                auxiliar->ping_total+= obtener_ping(auxiliar->dispositivo[i],auxiliar->dispositivo[i+1]);
            }
            if(f > 0 && a > 0){
                auxiliar->tipo=3;
            }else if(a == 0){
                auxiliar->tipo=1;
            }else{
                auxiliar->tipo=2;
            }
            auxiliar = auxiliar->siguiente;
        }           
    }
    int Eliminar(){
        int cont=0;
        while(frente!=NULL){
            for(int i = 0; i <=frente->indice;i++){
                Respaldar_Rutas(frente->dispositivo[i],frente->dispositivo[i+1]);
            }
            cont++;
            sacar_cola();
        }
        return cont;
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

//------------------------------------------------------------//Respaldos

void Respaldar_Rutas(string h1,string h2){
    Dispositivo* Disp1 = buscardispositivo(h1);
    Dispositivo* Disp2 = buscardispositivo(h2);

    Relacion *actual = Disp1->lista_vecinos;

    while(actual != NULL && actual->con_quien->hostname != h2){
        actual = actual->siguiente_R;
    }
    if(actual!= NULL){
        if(Rutas_resp.is_open()){
            Rutas_resp<<h1<<", ";
            Rutas_resp<<h2<<", ";
            Rutas_resp<<actual->ping<<", ";
            Rutas_resp<<actual->tipo<<endl;      
        }
        ContRel--;
        Disp1->desconectar(Disp2);
        Disp2->desconectar(Disp1);
    } 
}

void Respaldar_Dispositivo(Dispositivo* Disp1){
    Relacion *actual = Disp1->lista_vecinos;
    ContDisp--;
    if(Dispositivo_resp.is_open()){
        Dispositivo_resp<<Disp1->hostname<<", ";
        Dispositivo_resp<<Disp1->ip<<endl;            
    }
    while(actual != NULL){
        if(Rutas_resp.is_open()){
            Rutas_resp<<Disp1->hostname<<", ";
            Rutas_resp<<actual->con_quien->hostname<<", ";
            Rutas_resp<<actual->ping<<", ";
            Rutas_resp<<actual->tipo<<endl;      
        }
        ContRel--;
        Disp1->desconectar(actual->con_quien);
        actual->con_quien->desconectar(Disp1);
        actual = actual->siguiente_R;
    }      
}

void Respaldar_Archivo(){
    fstream Dispositivo_dat("Dispositivos.dat", ios:: out);
    Dispositivo* Disp1 = lista;
    Relacion *actual = Disp1->lista_vecinos;
    Dispositivo_dat<<ContDisp<<endl;
    while(Disp1 != NULL){
        Dispositivo_dat<<Disp1->hostname<<", "<<Disp1->ip<<endl;
        Disp1 =Disp1->siguiente_D;
    }
    Dispositivo_dat<<ContRel<<endl;
    Disp1 = lista;
    while(Disp1 != NULL){
        while(actual != NULL){   
            if(Dispositivo_dat.is_open()){
                Dispositivo_dat<<Disp1->hostname<<", ";
                Dispositivo_dat<<actual->con_quien->hostname<<", ";
                Dispositivo_dat<<actual->ping<<", ";                    Dispositivo_dat<<actual->tipo<<endl;      
            }
            Disp1->desconectar(actual->con_quien);
            actual->con_quien->desconectar(Disp1);
            actual = actual->siguiente_R;            
        }
        Disp1 = Disp1->siguiente_D;
        actual = Disp1->lista_vecinos;
    } 
    Dispositivo_dat.close();   
}

void Respaldar_DispositivoExistente(){
    fstream Dispositivo_dat("Dispositivos.dat", ios:: out);
    Dispositivo* Disp1 = lista;
    Relacion *actual = Disp1->lista_vecinos;
    Dispositivo_dat<<ContDisp<<endl;
    while(Disp1 != NULL){
        Dispositivo_dat<<Disp1->hostname<<", "<<Disp1->ip<<endl;
        Disp1 =Disp1->siguiente_D;
    }
}

///--------------------------------------------------------///Funciones lista
bool insertar_dispositivo(string nombre, string direccion){
    if(buscardispositivo(nombre)==NULL && buscardispositivo(direccion)== NULL){
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
        ContDisp++;
        return 1;
    }
    return 0;
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
                ContRel++;
                return 1;    
            }
        }
    }

    return 0;
};

int tipo_conexion_unaria(string inicio, string busqueda){
    Dispositivo *h1=buscardispositivo(inicio);
    Dispositivo *h2= buscardispositivo(busqueda);
    Relacion *actual = h1->lista_vecinos; //Auxiliar pa no romper nada

    while ((actual !=NULL)&&(actual->con_quien!=h2)){ //Recorro la lista de relaciones del dispositivo mientras no consiga h2 hasta el final
		actual = actual->siguiente_R;
	}
    if(actual->tipo=="Fibra"){
        return 1;
    }
    return 2;
}

int obtener_ping(string inicio, string busqueda){
    Dispositivo *h1=buscardispositivo(inicio);
    Dispositivo *h2= buscardispositivo(busqueda);
    Relacion *actual = h1->lista_vecinos; //Auxiliar pa no romper nada

    while ((actual !=NULL)&&(actual->con_quien!=h2)){ //Recorro la lista de relaciones del dispositivo mientras no consiga h2 hasta el final
		actual = actual->siguiente_R;
	}

    return (actual->ping);
}
///-------------------------------------------------------///Seccion de delete

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
        Respaldar_Dispositivo(aux_borrar);
        delete aux_borrar;
    }
}

void eliminarRelacion (string h1, string h2){
    if(lista!=NULL){ //Por si acaso
        bool flag=buscarRutas(h1,h2);
        if(flag){
            cout<<"Entre los dispositivos "<<h1<<" y "<<h2<<" se eliminaron las siguientes rutas: "<<endl;
            soluciones.imprimir();
            
            cout<<"Total de rutas eliminadas : "<<soluciones.Eliminar()<<endl;
        }
        else{
            cout<<"No fue posible eliminar la relacion"<<endl;
        }
        
    }
}

///-------------------------------------------------------/// BUSQUEDA DE RUTAS DIABOLICAS


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

bool buscarRutas(string h1, string h2){
    Dispositivo *origen = buscardispositivo(h1);
    Dispositivo *objetivo = buscardispositivo(h2);
    if(origen==NULL &&  objetivo == NULL){
        Salidas<<"-1"<<endl;
    }
    else if(objetivo==NULL){
        Salidas<<"-2"<<endl;
    }
    else if(origen==NULL){
        Salidas<<"-3"<<endl;
    }
    else{
        almacen.push(origen->hostname);
        Back(origen,objetivo);
        
        if(soluciones.frente==NULL&&soluciones.final==NULL){
            Salidas<<"0"<<endl;
            return 0;
            //no fue posible jaja
        }
        else{
            soluciones.verificar_tipo();
            return 1;
        }
    }
    return 0;
}

void Verificar_coma(string &Entrada){
    if(Entrada.at(Entrada.length()- 1) == ','){
        Entrada.pop_back();
    }
}

int str_int(string entero){
    Verificar_coma(entero);
    int aux = stoi(entero);
    return aux;
}

///-------------------------------------------------------/// MENU 
void M_agregar_info(char &entrada){
    system("clear");system("cls");
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computacion\n Orquestador de Redes de conectividad\n\nMenu de opciones:"<<endl;
    cout<<"1.\t Agregar dispositivo.\n2.\t Agregar ruta.\n3.\t Volver a Principal.\n4.\t Salir de la aplicacion."<<endl;
    string hostname,ip;
    string h2,tipo;

    int ping;
    bool flag; //Indicador de error de las funciones

    while(true){
        cout<<"Opcion: ";
        cin>>entrada;
        switch(entrada){
            case '1':
                cout<<"Ingresa el hostname seguido de la ip: "<<endl;
                cin>>hostname>>ip;
                flag=insertar_dispositivo(hostname,ip);
                if(flag){
                    cout<<"El dispositivo "<<hostname<<" ha sido agregado con exito\n"; 
                    Salidas<<"1"<<endl;   
                }
                else{
                    cout<<"El dispositivo ya existe en el sistema. No fue agregado\n"; 
                    Salidas<<"0"<<endl;
                }
                break;
            case '2':
                cout<<"Ingresa el hostname1, hostname2, ping, tipo: "<<endl;
                cin>>hostname>>h2>>ping>>tipo;
                flag=establecer_conexion(ping,tipo,hostname,h2);
                if(flag){
                    cout<<"Se agrego una ruta entre "<<hostname<<" y "<<h2<<endl;
                    Salidas<<"1"<<endl;    
                }
                else{
                    cout<<"No fue posible establecer la conexion\n";
                    Salidas<<"0"<<endl; 
                }
                break;
            case '3':
                return;
            case '4':
                Respaldar_Archivo();
                Salidas.close();
                exit(0);
            default:
                cout<<"Opcion invalida"<<endl;
                break;
        }    
    }
    
};

void M_eliminar_info(char &entrada){
    system("clear");system("cls");
    cout<<" Universidad central de venezuela\nFacultad de Ciencias\nEscuela de Computacion\nOrquestador de Redes de conectividad\n\nMenu de opciones:"<<endl;
    cout<<"1.\t Eliminar dispositivo.\n2.\t Eliminar ruta.\n3.\t Volver a Principal.\n4.\t Salir de la aplicacion."<<endl;
    string hostname,host2;

    while(true){
        cout<<"Opcion: ";
        cin>>entrada;
        switch(entrada){
        case '1':
            cout<<"Ingrese el hostname o ip del dispositivo a eliminar"<<endl;
            cin>>hostname;
            if(buscardispositivo(hostname)== NULL){
                cout<<"El dispositivo "<<hostname<<" No existe en el sistema\n";
                Salidas<<"0"<<endl;
                break;
            }
            eliminarDispositivo(hostname);
            cout<<"El dispositivo "<<hostname<<" fue eliminado del sistema."<<endl;
            Salidas<<"1"<<endl;
            break;
        case '2':
            cout<<"Ingrese los nombres o ip de los dispositivos: ";
            cin>>hostname>>host2;
            eliminarRelacion(hostname,host2);
            break;
        case '3':
            return;
        case '4':
            Respaldar_Archivo();
            Salidas.close();
            exit(0);
        default:
            cout<<"Opcion invalida"<<endl;
            break;
        }    
    }
    
};

void M_buscar_listar(char &entrada){
    system("clear");system("cls");
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computacion\n Orquestador de Redes de conectividad\n\nMenu de opciones:"<<endl;
    cout<<"1.\t Consultar dispositivo.\n2.\t Listado de Dispositivos.\n3.\t Buscar ruta(1: 5G, 2: fibra optica, 3: ambas).\n4.\t Dispositivos adyacentes.\n5.\t Volver a Principal.\n6.\t Salir de la aplicacion."<<endl;
    Dispositivo *buscado;
    string host_ip, h2; 
    int tipo;
    int aux=0;

    while(true){
        cout<<"Opcion: ";
        cin>>entrada;        
        switch(entrada){
            case '1':
                cout<<"Ingrese Hostname o IP"<<endl;
                cin>>host_ip;
                buscado=buscardispositivo(host_ip);
                if(buscado != NULL){
                    cout<<buscado->hostname<<", "<<buscado->ip<<endl;  
                }
                else{
                    cout<<"El dispositivo indicado no existe";
                }
                
                break;
            case '2':
                mostrarlista();
                break;
            case '3':
                cout<<"Ingrese los identificadores:"<<endl;
                cin>>host_ip>>h2;
                buscarRutas(host_ip,h2);
                aux=soluciones.imprimir_desencolar();
                cout<<"numero de rutas encontradas: "<<aux<<endl;

                break;
            case '4':
                cout<<"Ingresa el hostname o ip: "<<endl;
                cin>>host_ip;
                mostrarrelacion(buscardispositivo(host_ip)->lista_vecinos);
                break;
            case '5':
                return;
            case '6':
                Respaldar_Archivo();
                Salidas.close();
                exit(0);
            default:
                cout<<"Opcion invalida"<<endl;
                break;
        }
    }
        
};

void M_Respaldar_Rutas(char &entrada){
    system("clear");system("cls");
    
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computacion\n Orquestador de Redes de conectividad\n\nMenu de opciones:"<<endl;
    cout<<"1.\t Listado de dispositivos existentes.\n2.\t Listado de dispositivos eliminados.\n3.\t Listado de rutas eliminadas.\n4.\t Volver a Principal.\n5.\t Salir de la aplicacion."<<endl;
    
    string host,ip;

    while(true){
        cout<<"Opcion: ";
        cin>>entrada;
        ifstream Entrada_2("Dispositivo.dat");
        Dispositivo_resp.seekg(0);
        Rutas_resp.seekg(0);
        switch(entrada){
            case '1': 
                Respaldar_DispositivoExistente();
                cout<<"Los dispositivos almacenados en el archivo Dispositivos.dat son: "<<endl;
                if(Entrada_2.is_open()){
                    int Dis = 0, Rel = 0, ping = 0;
                    string aux1,aux2,aux3;
                    Entrada_2>>Dis;
                    for(int i = 0; i < Dis; i++){
                        Entrada_2>>aux1;//Host
                        Entrada_2>>aux2;//ip
                        cout<<aux1<<" "<<aux2<<endl;
                    }
                }
                break;
            case '2':
                cout<<"Los dispositivos eliminados almacenados en el archivo Dispositivos_resp.dat son: "<<endl;
                //Muestra el contenido del archivo de respaldo Dispositivo_resp.dat
                if(Dispositivo_resp.is_open()){
                    do{
                        Dispositivo_resp>>host;
                        Dispositivo_resp>>ip;
                        cout<< host <<" "<< ip<< endl;
                        host =" "; ip=" ";
                    }while(!Dispositivo_resp.eof());
                        
                    
                }
                break;
            case '3':
                cout<<"Las rutas eliminadas almacenadas en el archivo Rutas_resp.dat son: "<<endl;
                if(Rutas_resp.is_open()){
                    do{
                        Rutas_resp>>host;
                        cout<<host<<" ";
                        Rutas_resp>>host;
                        cout<<host<<" ";
                        Rutas_resp>>host;
                        Rutas_resp>>ip;
                        cout<<host<<" "<<ip<<endl;
                        host=" ", ip = " ";
                    }while(!Rutas_resp.eof());
                }                
                break;
            case '4':               
                return;
            case '5':
                Respaldar_Archivo();
                Salidas.close();
                exit(0);
            default:
            cout<<"Opcion invalida"<<endl;
            break;
        }    
    }
    

};

void M_inicio(){
    char entrada;
    cout<<" Universidad central de venezuela\n Facultad de Ciencias\n Escuela de Computacion\n Orquestador de Redes de conectividad\n\nMenu de opciones:"<<endl;
    cout<<"1.\t Agregar informacion.\n2.\t Eliminar informacion.\n3.\t Buscar y listar.\n4.\t Mostrar respaldos.\n5.\t Creditos.\n6.\t Salir de la aplicacion."<<endl;
    cin>>entrada;

    switch(entrada){
        case '1':
            M_agregar_info(entrada);
            break;
        case '2':
            M_eliminar_info(entrada);
            break;
        case '3':
            M_buscar_listar(entrada);
            break;
        case '4':
            M_Respaldar_Rutas(entrada);
            break;
        case '5':
            cout<<endl<<endl<<"---------"<<endl;
            cout<<"Carlos Cao \tC.I. 28.655.925\nErimar Reis \tC.I. 29.743.464\n";
            cout<<endl<<endl<<"---------"<<endl;
            break;
        case '6':
            Respaldar_Archivo();
            Salidas.close();
            exit(0);
        default:
            cout<<"Opcion invalida"<<endl;
            break;
    }
};

void Inicio(){
    
    ifstream Entrada("Dispositivos.dat");
    if(Entrada.is_open()){
        int disp = 0, rel = 0;
        string aux1,aux2,aux3,ping;
        Entrada>>disp;
        for(int i = 0; i < disp; i++){
            Entrada>>aux1;//Host
            Entrada>>aux2;//ip
            Verificar_coma(aux1);
            Verificar_coma(aux2);
            insertar_dispositivo(aux1,aux2);
        }
        Entrada>>rel;
        for(int i = 0; i < rel; i++){
            Entrada>>aux1;//host1
            Entrada>>aux2;//host2        
            Entrada>>ping;//ping
            Entrada>>aux3;//tipo
            Verificar_coma(aux1);
            Verificar_coma(aux2);
            Verificar_coma(aux3);
            establecer_conexion(str_int(ping),aux3,aux1,aux2);
        }
    }
    Entrada.close();
}

///-------------------------------------------------------///
int main (){
    Inicio();
    while(true){
        M_inicio();    
    }

    return 0;
}