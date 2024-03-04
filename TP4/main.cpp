#include <iostream>
#include "arboles.hpp"

using namespace std;

struct Producto {
    string nombre;
    float precio;
};

int criterio(Producto a, Producto b) {
    return (a.nombre > b.nombre) ? -1 : (a.nombre < b.nombre);
}

int main()
{
    const int dim = 7;
    Producto vec[dim] = {{"fideos", 6.6}, 
                        {"arroz", 3.3}, 
                        {"conejo", 5.5},
                        {"aceite", 2.2}, 
                        {"papa", 10.1}, 
                        {"harina", 8.8},
                        {"pollo", 14.4}};

    NodoAr<Producto>* arbol = nullptr;

    for (int i = 0; i < dim; i++) {
        insertar(vec[i], arbol, criterio);
    }

    Producto prod;
    NodoAr<Producto>* arnodo;
    cout << "Ingrese un producto" << endl;
    while (cin >> prod.nombre) {
        arnodo = buscar(prod, arbol, criterio);
        if (arnodo)
        {
            cout << "Encontramos " << prod.nombre << ": " << arnodo->dato.precio << endl;
        } 
        else 
        {
            cout << "No se encontró " << prod.nombre << endl;
        }
        cout << "Ingrese un producto" << endl;
    }

    return 0;
}