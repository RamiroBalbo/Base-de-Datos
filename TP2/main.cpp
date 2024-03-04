#include <iostream>
#include <fstream>
#include <iomanip>
#include "rwstring.hpp"

using namespace std;

const int lprod = 10;

struct Almacen {
    string prod;
    char pas;
    int pos;
    int stock;
};

fstream& operator << (fstream &fs, Almacen& alm) {
    writestring(fs, alm.prod, lprod);
    fs.write(reinterpret_cast<const char *> (&alm.pas), sizeof(alm.pas));
    fs.write(reinterpret_cast<const char *> (&alm.pos), sizeof(alm.pos));
    fs.write(reinterpret_cast<const char *> (&alm.stock), sizeof(alm.stock));
    return fs;
}

fstream& operator >> (fstream &fs, Almacen& alm) {
    alm.prod = readstring(fs, lprod);
    fs.read(reinterpret_cast<char *> (&alm.pas), sizeof(alm.pas));
    fs.read(reinterpret_cast<char *> (&alm.pos), sizeof(alm.pos));
    fs.read(reinterpret_cast<char *> (&alm.stock), sizeof(alm.stock));
	return fs;
}


void mostrar_todo(Almacen vec1[], Almacen vec2[], int dim) { //Punto 4
    cout << setw(10) << "Producto" << setw(11) << "Pas." << setw(10) << "Pos." << setw(10) << "Stock" 
         << setw(10) << "Movs" << setw(10) << "Final" << endl;
    for (int i = 0; i < dim; i++)
        cout << setw(10) << vec1[i].prod << setw(10) << vec1[i].pas << setw(10)
             << vec1[i].pos << setw(10) << vec1[i].stock << setw(10) << vec2[i].stock << setw(10) << vec1[i].stock + vec2[i].stock <<endl;
}

void mostrar_Final(fstream& fs, Almacen aux) { //Punto 5
    cout << setw(10) << "Producto" << setw(11) << "Pas." << setw(10) << "Pos." << setw(10) << "Stock" << setw(10) << "Movs" << setw(10) << "Final" << endl;
    while(fs >> aux){
        cout << setw(10) << aux.prod << setw(10) << aux.pas << setw(10)
             << aux.pos << setw(10) << aux.stock << setw(10) << 0 << setw(10) << aux.stock << setw(10) << endl;
    }
}



void criterio2(Almacen vec, Almacen vecpos){
    if(vec.prod < vecpos.prod){
        swap(vec, vecpos);
    }else if(vec.prod == vecpos.prod && vec.pas < vecpos.pas){
        swap(vec, vecpos);
    }else if(vec.prod == vecpos.prod && vec.pas == vecpos.pas && vecpos.pos < vecpos.pos){
        swap(vec, vecpos);
    }
}

template <typename T> void ordenar(T vec[], int dim, void criterio(T, T)) {
    for (int pos = 0; pos < dim - 1; pos++) {
        for (int i = pos + 1; i < dim; i++) {
            criterio(vec[i], vec[pos]); // Ordenamos de acuerdo al criterio            
        }
    }
}

int criterio(Almacen mov, Almacen v_ext){
    if(v_ext.prod == mov.prod && v_ext.pas == mov.pas && v_ext.pos == mov.pos){
        if(v_ext.stock>= mov.stock){
            return 1;
        }
    }
    return 0;
}
template <typename T> int buscar_modificar(T mov, T v_ext[], T v_mov[], int dim, int criterio(T, T)){
    int cont = 0;
    for(int i = 0; i<dim; i++){
        if(criterio(mov, v_ext[i]) == 1){
            v_mov[i].prod = mov.prod;
            v_mov[i].pas = mov.pas;
            v_mov[i].pos = mov.pos;
            v_mov[i].stock += mov.stock;
            cont++;
        }
    }
    return cont;
}

int main()
{
    const int dim = 40;
    Almacen v_ext[dim]{}; //vector vacio de existencias
    Almacen v_mov[dim]{}; //vector vacio de movimientos
    int i;

    //Punto 1
    fstream arch_ext;
    arch_ext.open("Existencias.bin", ios::binary | ios::in);
    if (!arch_ext) {
		cout << "No se pudo abrir el archivo de escritura" << endl;
		return 1;
	}

	cout << endl << "Leyendo y cargando el archivo Existencias" << endl;
    for (i = 0; i < dim && arch_ext >> v_ext[i]; ++i){}
	arch_ext.close();

    //Punto 2
    fstream arch_mov;
    arch_mov.open("Movimientos.bin", ios::binary | ios::in);
    if (!arch_mov) {
		cout << "No se pudo abrir el archivo de Movimientos" << endl;
		return 1;
	}

	cout << endl << "Leyendo y cargando el archivo de Movimientos" << endl;
    Almacen mov;
    int error;
    while (arch_mov >> mov) { //Recorro el archivo y guardo todos los campos del Almacen en movimientos
        error = buscar_modificar(mov, v_ext, v_mov, dim, criterio);
        if (error == 0) {
            cout << "Error, clave no encontrada: " << mov.prod << " " << mov.pas 
             << "-" << mov.pos << endl;
        }
    }
	arch_ext.close();

    //Punto 3
    fstream arch_fin;
	arch_fin.open("Final.bin", ios::binary | ios::out);
	if (!arch_fin) {
		cout << "No se pudo abrir el archivo de escritura" << endl;
		return 1;
	}

	cout << endl << "Escribiendo al archivo:" << endl; //Lo abrimos una vez para escritura y otra para lectura
    Almacen fin;
	for (i = 0; i < dim; ++i){
        fin.prod = v_ext[i].prod;
        fin.pas = v_ext[i].pas;
        fin.pos = v_ext[i].pos;
        fin.stock = v_ext[i].stock + v_mov[i].stock;
        arch_fin << fin;
    }
	arch_fin.close();

    //Punto 4
    cout << "Informe de existencias actualizadas" << endl;
    ordenar(v_ext, dim, criterio2);
    ordenar(v_mov, dim, criterio2);
    mostrar_todo(v_ext, v_mov, dim);

    cout << "---------------------------------------------------------------------------------" << endl;

    //Punto 5
    arch_fin.open("Final.bin", ios::binary | ios::in);
    if(!arch_fin){
        cout << "No se pudo abrir el archivo final.bin correctamente";
        return 1;
    }

    Almacen final;
    mostrar_Final(arch_fin, final);
    arch_fin.close();

    return 0;
}
