#pragma once
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

template <class TYPE>
class vecteur
{
private:
	TYPE *_tab;		//pointeur sur le vecteur d’entier
	int _dim;		//dimension du vecteur
public:
	vecteur<TYPE>();				//constructeur sans paramètre.. Attention, un 
									//pointeur ne peut resté non initialisé.
	vecteur(int dim);				//construit le vecteur à la dimension de dim
	vecteur(const vecteur<TYPE>& v); //copieur de vecteur
	~vecteur();						//destructeur essentiel avec l’allocation dynamique
									//Attention, un pointeur ne peut conservé 
									//l’adresse d’un espace mémoire désallouée.
	void pushback(TYPE ele);		//ajoute un élément à la fin du vecteur

	int size()const;				//retourne la dimension du vecteur

	TYPE front()const;				//retourne le premier element du vecteur
	TYPE back()const;				//retourne le dernier element du vecteur

	void resize(int dim);			//change la taille du vecteur

	void print(ostream& sortie) const;//afficher le contenu de vecteur

	void clear();	//efface le vecteur en mémoire dynamique et efface l'adresse
					//ou il se trouvait

	TYPE& at(int pos);					//retourne la valeur à la position demandé	

	void insert(int pos, TYPE ele);		//insere un element a un position donner
	void erase(int pos, int pos2);		//efface les element entre 2 positions

	bool isSymetric(vecteur<TYPE> v);				//Verifie la symetrie de 2 vecteurs
	void shift(int k);								//Décale les éléments du vecteur
	void inverse();									//Inverse le vecteur
	void swap(vecteur<TYPE> &v1, vecteur<TYPE> &v2);//Permute le contenue de 2 vecteurs

	TYPE& operator[](int i);								//operateur []
	const vecteur<TYPE>& operator=(const vecteur<TYPE>& v);	//operateur =
	bool operator==(vecteur<TYPE> v);						//operateur ==
	bool operator!=(vecteur<TYPE> v);						//operateur !=
	vecteur<TYPE>& operator+=(const vecteur<TYPE> &v);		//operateur +=
	vecteur<TYPE> operator+(const vecteur<TYPE> &v) const;	//operateur +

};
//affichage avec operateur
template <class TYPE>
ostream& operator<<(ostream& sortie, const vecteur<TYPE>& v);


//constructeur sans paramètre.
template <class TYPE>
vecteur<TYPE>::vecteur<TYPE>() {
	_dim = 0;
	_tab = nullptr;
}

//construit le vecteur à la dimension de dim
template <class TYPE>
vecteur<TYPE>::vecteur<TYPE>(int dim) {
	assert(dim >= 0);
	_dim = dim;

	if (dim == 0)
		_tab = nullptr;
	else {	//Initialise les éléments du vecteur à 0		
		_tab = new TYPE[_dim];
		for (int i = 0; i < _dim; i++) {
			*(_tab + i) = 0;
		}
	}
}

//destructeur essentiel avec l’allocation dynamique
template <class TYPE>
vecteur<TYPE>::~vecteur<TYPE>() {
	clear();
}

//Copieur de vecteur
template <class TYPE>
vecteur<TYPE>::vecteur<TYPE>(const vecteur<TYPE>& v) {
	_dim = v._dim;
	_tab = nullptr;

	if (_dim != 0) {
		_tab = new TYPE[_dim];

		for (int i = 0; i < _dim; i++) {
			*(_tab + i) = *(v._tab + i);
		}
	}
}

//Ajoute un nombre à la fin du vecteur
template <class TYPE>
void vecteur<TYPE>::pushback(TYPE ele) {
	TYPE *newTab = new TYPE[++_dim];

	for (int i = 0; i < _dim - 1; i++) {	//Copie le contenue dans newTab
		*(newTab + i) = *(_tab + i);
	}

	*(newTab + _dim - 1) = ele;
	delete[]_tab;
	_tab = newTab;
}

//retourne la dimension du vecteur
template <class TYPE>
int vecteur<TYPE>::size()const {
	return _dim;
}

//retourne le premier element du vecteur
template <class TYPE>
TYPE vecteur<TYPE>::front()const {
	return *_tab;
}

//retourne le dernier element du vecteur
template <class TYPE>
TYPE vecteur<TYPE>::back()const {
	return *(_tab + _dim - 1);
}

//Change la grandeur du vecteur
template <class TYPE>
void vecteur<TYPE>::resize(int newDim) {
	assert(newDim >= 0);

	if (_dim == newDim)
		return;

	if (newDim == 0) {
		clear();
		return;
	}

	TYPE *newTab = new TYPE[newDim];

	for (int i = 0; i < _dim && i < newDim; i++)
		*(newTab + i) = *(_tab + i);

	//for (int i = _dim; i < newDim; i++)//rajoute des 0 pour le reste
	//		*(newTab + i) = 0;

	delete[] _tab;
	_tab = newTab;
	_dim = newDim;
}

//afficher le contenu de vecteur
template <class TYPE>
void vecteur<TYPE>::print(ostream& sortie)const {

	for (int i = 0; i < _dim; i++) {
		sortie << *(_tab + i) << " ";
	}
}

//efface le vecteur en mémoire dynamique et efface l'adresse
template <class TYPE>
void vecteur<TYPE>::clear() {
	delete[] _tab;
	_tab = nullptr;
	_dim = 0;
}

//retourne la valeur à la position demandé	
template <class TYPE>
TYPE& vecteur<TYPE>::at(int pos) {
	assert(pos >= 0);
	assert(pos <= _dim);

	return _tab[pos];
}

//efface les element entre 2 positions
template <class TYPE>
void vecteur<TYPE>::insert(int pos, TYPE ele) {
	assert(pos <= _dim);
	assert(pos >= 0);

	//_dim++;
	TYPE *newTab = new TYPE[++_dim];

	for (int i = 0; i < pos; i++) {		//Copie le debut du vecteur
		*(newTab + i) = *(_tab + i);	//jusqu'a la pos
	}

	*(newTab + pos) = ele;		//insere l'element a la pos

	for (int i = pos + 1; i < _dim; i++) { // copy le restant du vecteur
		*(newTab + i) = *(_tab + i - 1);
	}

	delete[]_tab;
	_tab = newTab;
}

//Verifie la symetrie de 2 vecteurs
template <class TYPE>
void vecteur<TYPE>::erase(int pos1, int pos2) {
	assert(pos1 <= _dim && pos2 <= _dim);

	int newDim = _dim - (pos2 - pos1 + 1);
	TYPE*newTab = new TYPE[newDim];
	newTab = new TYPE[newDim];
	int k = 0;

	for (int i = 0; i < _dim; i++) {	//copie le vecteur sans 
		if (i < pos1 || i > pos2) {		//les element aux position voulu
			*(newTab + k) = *(_tab + i);
			k++;
		}
	}
	delete[] _tab;
	_dim = newDim;
	_tab = newTab;
}

//Verifie la symetrie de 2 vecteurs
template <class TYPE>
bool vecteur<TYPE>::isSymetric(vecteur<TYPE> v) {

	if (_dim != v._dim)	//Si les dimension ne sont pas egale on retourne faux
		return false;

	//On verifie si les élément du vecteur sont symétrique
	for (int i = 0; i < _dim - 1; i++)
		if (*(_tab + i) != *(v._tab + _dim - 1 - i))
			return false;

	return true;
}

//Décale les éléments du vecteur
template <class TYPE>
void vecteur<TYPE>::shift(int k) {
	vecteur temp = *this; //vecteur temporaire

	if (k > _dim)		//trouve le modulo
		k = k % _dim;
	if (k < 0)			//si negatif il devient positif
		k = (k % _dim) + _dim;

	temp.erase(0, (_dim - k - 1)); //efface les nombres egale au modulo - 1
								   // dans la copie
	for (int i = 0; i < (_dim - k); i++)
		temp.pushback(at(i));	//rajoute les elements effacer a la fin du vecteur

	*this = temp;
	temp.clear();
}

//Inverse le vecteur ******
template <class TYPE>
void vecteur<TYPE>::inverse() {

	for (int i = 0; i < round(_dim / 2); i++) { //swap les elements en partant des extremité		
		std::swap(at(i), at(_dim - i - 1));
	}

}

//Permute le contenue de 2 vecteurs
template <class TYPE>
void vecteur<TYPE>::swap(vecteur &v1, vecteur &v2) {
	std::swap(v1._tab, v2._tab);
	std::swap(v1._dim.v2._dim);
}

//operateur []
template <class TYPE>
TYPE& vecteur<TYPE>::operator[](int i) {
	return	at(i);
}

//operateur =
template <class TYPE>
const vecteur<TYPE>& vecteur<TYPE>::operator=(const vecteur <TYPE>& v)
{
	_dim = v._dim;
	if (_dim == 0)
		_tab = nullptr;
	else {
		_tab = new TYPE[_dim];
		assert(_tab != nullptr);
		for (int i = 0; i < _dim; i++) //le contenue du vecteur devient egale au
			*(_tab + i) = v._tab[i];   // contenue de l'autre
	}

	return v;
}

//operateur ==
template <class TYPE>
bool vecteur<TYPE>::operator==(vecteur<TYPE> v) {
	if (_dim == v._dim && _tab == v._tab)
		return true;
}

//operateur !=
template <class TYPE>
bool vecteur<TYPE>::operator!=(vecteur<TYPE> v) {
	if (_dim != v._dim)
		return true;
}

//operateur +=
template <class TYPE>
vecteur<TYPE>& vecteur<TYPE>::operator+=(const vecteur<TYPE> &v) {
	vecteur temp = v;

	for (int i = 0; i < v._dim; i++)
		pushback(temp[i]);	//rajoute le contenue du vecteur v dans notre vecteur

	temp.clear();
	return *this;
}

//operateur +
template <class TYPE>
vecteur<TYPE> vecteur<TYPE>::operator+(const vecteur<TYPE> &v) const {
	vecteur<TYPE> result = *this;

	result += v;
	return result;
}

//affichage avec operateur
template <class TYPE>
ostream& operator<<(ostream& sortie, const vecteur<TYPE>& v) {

	v.print(sortie);
	return sortie;
}