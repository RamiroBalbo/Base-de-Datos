#ifndef LISTAS_HPP_INCLUDED
#define LISTAS_HPP_INCLUDED

template <typename T> struct Nodo {
	T dato; // valor que contiene el nodo
	Nodo<T>* sig; // puntero al siguiente nodo
};

template <typename T> void push(Nodo<T>* &pila, T valor)
{
	Nodo<T>* nuevo = new Nodo<T>;
	nuevo->dato = valor;
	nuevo->sig = pila;
	pila = nuevo;
}

template <typename T> T pop(Nodo<T>* &pila)
{
	if (pila == nullptr) {
		std::cout << "Error: pop en pila vacia" << std::endl;
		exit(EXIT_FAILURE);
		//modo no recomendado de terminar, en particular si uso objetos
	}
	T valor = pila->dato;
	Nodo<T>* aux_elim = pila;
	pila = pila->sig;
	delete aux_elim;
	return valor;
}

template <typename T> void agregar(Nodo<T>*& cola, T valor)
{
	Nodo<T>* nuevo = new Nodo<T>;
	nuevo->dato = valor;
	nuevo->sig = nullptr;
	if (cola == nullptr) {
		cola = nuevo;
	} else {
		Nodo<T>* aux = cola;
		while (aux->sig != nullptr) //mientras que no sea el último
			aux = aux->sig;  //avanzo al siguiente
		aux->sig = nuevo;
	}
}

template <typename T> void insertar(T valor, Nodo<T>*& lista, int (*criterio)(T, T)){
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->dato = valor;

    if (lista == nullptr || criterio (nuevo->dato, lista->dato) < 0){
        nuevo->sig = lista;
        lista = nuevo;
    }else{
        Nodo<T>* p = lista;
        while (p->sig != nullptr && criterio(nuevo->dato, p->sig->dato) >= 0){
            p = p->sig;
        }
        nuevo->sig = p->sig;
        p->sig = nuevo;
    }
}

template <typename T> Nodo<T>* insertar_unico(T valor, Nodo<T>*& lista, int (*criterio)(T, T))
{ // Inserta un nodo nulo si es que no existe y devuelve su direccion de memoria, 
//caso que exista solo devuelve la direccion de memoria del que existe, posteriormente hago la modificación
	if (lista == nullptr || criterio(valor, lista->dato) < 0) {
		Nodo<T>* nuevo = new Nodo<T>; //Genero nodo
		nuevo->dato = valor; // y guardo el valor
		nuevo->sig = lista;
		lista = nuevo;
		return nuevo;
	} else {
		if (criterio(valor, lista->dato) == 0) //Si justo es igual al primero
			return lista;
		Nodo<T>* p = lista;
		while (p->sig != nullptr && criterio(valor, p->sig->dato) > 0)
			p = p->sig;
		if (p->sig != nullptr && criterio(valor, p->sig->dato) == 0) {
			return p->sig; //devuelvo el que ya estaba en la lista
		} else {
			Nodo<T>* nuevo = new Nodo<T>; //Genero nodo
			nuevo->dato = valor; // y guardo el valor
			nuevo->sig = p->sig;
			p->sig = nuevo;
			return nuevo;
		}
	}
}

template <typename T> Nodo<T>* extraer(T clave, Nodo<T>*& lista, int (*criterio)(T, T))
{
	Nodo<T> *aux;
	Nodo<T> **pp = &lista;

	while (*pp != nullptr && criterio(clave, (*pp)->dato) > 0)
		pp = &((*pp)->sig);

	if (*pp != nullptr && criterio(clave, (*pp)->dato) == 0) {
		aux = *pp;
		*pp = (*pp)->sig;
		return aux;
	} else {
		return nullptr;
	}
}


#endif // LISTAS_HPP_INCLUDED
