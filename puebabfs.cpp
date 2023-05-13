class Ruta {
public:
    Dispositivo *dispositivo;
    Ruta *anterior;

    Ruta(Dispositivo *disp, Ruta *ant) : dispositivo(disp), anterior(ant) {}
};

void BFS(string ini, string obj, Dispositivo *lista, Cola<Ruta*>& soluciones) {
    Dispositivo *inicio = buscardispositivo(lista, ini);
    Dispositivo *objetivo ueva_ru;

    cola_control.agregar_cola(new Ruta(inicio, nullptr)); // Agregamos la ruta inicial al dispositivo
    visitados.insertar(inicio,= buscardispositivo(lista, obj);
    Cola<Ruta*> cola_control;
    lista_visitado visitadosvo) {
            soluciones.agregar_cola(ruta_actual); // Almacenamos la solución encontrada
        } else {
            Relacion *relacion_actual = actual->lista_vecinos; // Tomamos la lista de vecinos del dispositivo actual
            while (relacion_actual != NULL) { // Siempre que tenga conexiones para verificar
                Dispositivo *vecino = relacion_actual->con; // Extraemos el dispositivo
                if (!visitados.contiene(vecino)) { // Si el dispositivo no está revisado
                    Ruta *n true); // Marcamos el primer elemento como visitado

    while (!cola_control.vacia()) { // Mientras existan elementos que revisar
        Ruta *ruta_actual = cola_control.quien_al_frente(); // Tomamos la ruta que está al frente
        Dispositivo *actual = ruta_actual->dispositivo; // Tomamos el último dispositivo de la ruta actual
        cola_control.sacar_cola(); // Sacamos la ruta de la cola

        if (actual == objetita = new Ruta(vecino, ruta_actual); // Creamos una nueva ruta con el vecino
                    cola_control.agregar_cola(nueva_ruta); // Agregamos la nueva ruta a la cola
                    visitados.insertar(vecino, true); // Marcamos el vecino como visitado
                }
                relacion_actual = relacion_actual->siguiente_R; // Pasamos al siguiente dispositivo
            }
        }
    }
}

void imprimir_soluciones(Cola<Ruta*>& soluciones) {
    if (soluciones.vacia()) {
        cout << "No se encontró una ruta de conexión entre los dispositivos" << endl;
        return;
    }

    int contador = 1;
    while (!soluciones.vacia()) {
        Ruta *ruta_actual = soluciones.quien_al_frente();
        soluciones.sacar_cola();

        cout << "Solución " << contador << ": ";
        vector<Dispositivo*> dispositivos;
        while (ruta_actual != nullptr) {
            dispositivos.push_back(ruta_actual->dispositivo);
            ruta_actual = ruta_actual->anterior;
        }

        for (int i = dispositivos.size() - 1; i >= 0; i--) {
            cout << dispositivos[i]->hostname << (i > 0 ? " -> " : "");
        }
        cout << endl;
        contador++;
    }
}