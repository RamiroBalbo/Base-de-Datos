#include <iostream>
#include <fstream>
#include <iomanip>
#include "listas.hpp"
#include "rwstring.hpp"

using namespace std;

struct Registro{
    string ciud;
    int codT;
    string prod;
    int cantUnid;
    float monto;
};
const int dimCiud = 14;
const int dimProd = 10;


struct Envio{
    int codT;
    string prod;
    int cantUnid;
    float monto;
};
struct Ciudad{
    string ciud;
    int totalUnidDesp = 0;
    float sumatMontAseg = 0;
    Nodo<Envio>* listE = nullptr;

};

fstream& operator >> (fstream &fs, Registro& reg) {
    reg.ciud = readstring(fs, dimCiud);
    fs.read(reinterpret_cast<char *> (&reg.codT), sizeof(reg.codT));
    reg.prod = readstring(fs, dimProd);
    fs.read(reinterpret_cast<char *> (&reg.cantUnid), sizeof(reg.cantUnid));
    fs.read(reinterpret_cast<char *> (&reg.monto), sizeof(reg.monto));
	return fs;
}

int criterio(Ciudad c1, Ciudad c2){
    return c1.ciud.compare(c2.ciud);
}

int criterio2(Envio env1, Envio env2){
    return env1.prod.compare(env2.prod);
}

void mostrar(Nodo<Ciudad>* listC){
    Nodo<Ciudad>* aux = listC;
    while(aux){
        cout << "---------------- " << aux->dato.ciud << " ----------------" << endl;
        cout << "Cantidad de unidades despachadas: " << aux->dato.totalUnidDesp << endl;
        cout << "Suma total aseguradas: " << fixed<<setprecision(2)<<aux->dato.sumatMontAseg << endl;
        cout << "Envios hechos a esta ciudad: " << endl;
        cout << "+++++++++++++++++++++++++++++++++++"<<endl;
        while (aux->dato.listE){
            cout << "Codigo de transportista: " << aux->dato.listE->dato.codT << endl;
            cout << "Producto: " << aux->dato.listE->dato.prod << endl;
            cout << "Cantidad de unidades: " << aux->dato.listE->dato.cantUnid << endl;
            cout << "Monto: " << aux->dato.listE->dato.monto << endl;
            aux->dato.listE = aux->dato.listE->sig;
        }
        cout << "+++++++++++++++++++++++++++++++++++"<<endl;
        
        aux = aux->sig;
    }
}

int main(){
    fstream archi_datos;
    fstream archi_nom;
    const int nom = 6;
    string vec_nom[nom]{};
    Nodo<Ciudad>* listC = nullptr;
    //Abro archivo con los datos
    archi_datos.open("Datos.bin", ios::binary | ios::in);
    if(!archi_datos){
        cout << "No se pudo abrir el archivo con los datos" << endl;
        return 1;
    }

    archi_nom.open("Nombres.txt");
    if(!archi_nom){
        cout << "No se pudo abrir el archivo con los nombres" << endl;
        return 1;
    }

    //Cargo los archivos en las estructuras correspondientes
    Registro reg;
    Ciudad c;
    Envio env;
    Nodo<Ciudad>* aux;
    
    //Cargo archivo de nombres
    for(int i = 0; i<nom && getline(archi_nom, vec_nom[i]); i++);
    archi_nom.close();
    //Cargo archivo de datos
    while(archi_datos >> reg){
        c.ciud = reg.ciud;

        env.codT = reg.codT;
        env.prod = reg.prod;
        env.cantUnid = reg.cantUnid;
        env.monto = reg.monto;

        aux = insertar_unico(c, listC, criterio);

        aux->dato.totalUnidDesp += reg.cantUnid;
        aux->dato.sumatMontAseg += reg.monto;
        insertar(env, aux->dato.listE, criterio2);
        // agregar(aux->dato.listE, env);
    }
    archi_datos.close();
    //Punto 2
    aux = listC;
    Nodo<Envio>* auxE = nullptr;
    while(aux){
        cout << "------------------- " << aux->dato.ciud << " -------------------" << endl; 
        cout << "Total de unidades: " << aux->dato.totalUnidDesp;
        cout << " - Monto total: " << aux->dato.sumatMontAseg << endl;
        cout << "------------------------------------------------------------------" << endl;
        cout << "\tTransporte " << "\t" << " envios" << endl;

        for(int i = 0; i<nom; i++){
            int cont = 0;

            auxE = aux->dato.listE;
            while(auxE){
                if(auxE->dato.codT == i){
                    cont++;
                }
                auxE = auxE->sig;
            }
            cout << setw(19) << vec_nom[i] << setw(10) << " " << cont << endl;
        }

        //Corte de control para acumular monto de productos
        cout << "Produtos  " << setw(9) <<"Unidades "<< "  Monto"<< endl; 
        auxE = aux->dato.listE;
        while(auxE){
            env.prod = auxE->dato.prod;
            float acum = 0;
            int cont = 0;
            while(auxE && auxE->dato.prod == env.prod){
                acum += auxE->dato.monto;
                cont += auxE->dato.cantUnid;
                cout << auxE->dato.prod << setw(10) << auxE->dato.cantUnid << setw(10) << fixed<<setprecision(2)<<auxE->dato.monto << endl;
                auxE = auxE->sig;
            }
            cout << endl << "Sum " << env.prod << "    " << cont << " " << fixed<<setprecision(2)<<acum << endl << endl;
        }
        auxE = aux->dato.listE;
        aux = aux->sig;
    }

    //Punto 3
    Nodo<Ciudad> *listC_anterior = listC;
    Nodo<Ciudad> *listCsig = listC;
    Nodo<Ciudad>* ciudadMenorUnidades = nullptr;

    listCsig = listCsig->sig;
    while(listCsig) {
        if(listCsig->dato.totalUnidDesp < listC_anterior->dato.totalUnidDesp) {
            ciudadMenorUnidades = listCsig;
        }
        listC_anterior = listC_anterior->sig;
        listCsig = listCsig->sig;
    }
    cout << "La ciudad con menor cantidad total de unidades es: " << ciudadMenorUnidades->dato.ciud << endl;

    //Punto 4
    if (ciudadMenorUnidades) {
        string productoBuscado;
        cout << "Ingrese el producto a consultar: ";
        cin >> productoBuscado;
        do{
            while (productoBuscado != ciudadMenorUnidades->dato.listE->dato.prod && !ciudadMenorUnidades->dato.listE){
                ciudadMenorUnidades->dato.listE = ciudadMenorUnidades->dato.listE->sig;
            }
            if ( ciudadMenorUnidades->dato.listE != nullptr){
                int totalEnviosProducto = 0;
                Nodo<Envio>* enviosCiudadMenorUnidades = ciudadMenorUnidades->dato.listE;
                while (enviosCiudadMenorUnidades) {
                    if (enviosCiudadMenorUnidades->dato.prod == productoBuscado) {
                        totalEnviosProducto++;
                    }
                    enviosCiudadMenorUnidades = enviosCiudadMenorUnidades->sig;
                }
                cout << "Hubo " << totalEnviosProducto << " envios de " << productoBuscado << endl;
            }else{
                cout << "No hay producto " << productoBuscado << " en " << ciudadMenorUnidades->dato.ciud << endl;
            }
            cout << "Ingrese el producto a consultar: ";
            
        }while (cin >> productoBuscado);
        
    }else
        cout << "No hay ciudades registradas." << endl;

    return 0;
}