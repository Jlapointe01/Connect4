#pragma once
#include <cassert>


template <typename TYPE> class list;

//CLASSE CELLULE
template <typename TYPE>
class cellule
{
	friend class list<TYPE>;	//la seule classe ayant accès à la 
								//représentation de la cellule
	TYPE element;				//Contenu de la cellule
	cellule <TYPE> *next;		//Pointeur vers la cellule suivante 
	cellule <TYPE> *previous;	//Pointeur vers la cellule precedente

	cellule(const TYPE &e, cellule<TYPE> *n = NULL, cellule<TYPE> *p = NULL) {
		element = e;
		next = n;
		previous = p;
	}
};

template <typename TYPE>
class list
{
private:
	cellule <TYPE> *_first;		//Adresse du premier élément 

	list(const list<TYPE> &l);	//Copieur désactivé
public:
	class iterator
	{
		friend class list<TYPE>;
	private:
		cellule<TYPE>* 	_current;	//adresse de la cellule
		const list<TYPE>* _list;	//adresse de la liste
	public:
		iterator();					//constructeurs
		~iterator();				//destructeur

		const iterator& operator=(const iterator& I);//pour j=i

		bool operator==(const iterator& I)const; //pour if(j==i)
		bool operator!=(const iterator& I)const; //pour if(j!=i)

		iterator operator++(int i);		//i++
		const iterator& operator++();	//++i

		iterator operator--(int i);		//i--
		const iterator& operator--();	//--i

		TYPE& operator*();				//accès à un élément
		const TYPE& operator*()const;	//idem, en mode const

	};
	list(void);				//constructeur
	~list(void);			//destructeur

	void clear();			//vide la liste
	bool empty()const;		//verifie si la liste est vide

	iterator begin()const;	//return premier
	iterator end()const;	//return NULL

	iterator insert(iterator I, const TYPE& e);	  //ajoute un element
	iterator erase(iterator I);					  //efface un element
	iterator erase(iterator first, iterator last);//efface plusieurs elements

	TYPE& operator[](iterator I);				  //pas ds la STL
	const TYPE& operator[](iterator I)const;

	//chercher dans la liste l’élément reçu en paramètre
	//et à partir de l’itérateur reçu pour la version 2
	//la méthode  retourne un iterator sur l’element trouvé
	//s’il est innexistant, l’iterator est à la fin 
	iterator operator()(const TYPE&)const;
	iterator operator()(const TYPE&, iterator)const;

	const list<TYPE>& operator=(const list<TYPE>& l); //operateur =
};

//***Class iterator***

//Constructeur
template <typename TYPE>
list<TYPE>::iterator::iterator() {
	_current = nullptr;
	_list = nullptr;
}

//Destructeur
template <typename TYPE>
list<TYPE>::iterator::~iterator(void) {
	_current = nullptr;
	_list = nullptr;
}
//pour j=i
template <typename TYPE>
const typename list<TYPE>::iterator& list<TYPE>::iterator::operator=(const iterator& I) {
	_current = I._current;
	_list = I._list;
	return *this;
}
//pour if(j==i)
template <typename TYPE>
bool list<TYPE>::iterator:: operator==(const iterator& I)const {

	assert(_list == I._list);
	return _current == I._current;
}
//pour if(j!=i)
template <typename TYPE>
bool list<TYPE>::iterator::operator!=(const iterator& I)const {
	return !operator==(I);
}
//i++
template <typename TYPE>
typename list<TYPE>::iterator list<TYPE>::iterator::operator++(int i) {
	list<TYPE>::iterator it(*this);

	operator++();

	return it;
}
//++i
template <typename TYPE>
const typename list<TYPE>::iterator& list<TYPE>::iterator::operator++() {
	assert(_current != nullptr);
	assert(!_list->empty());

	if (_current->next == _list->_first)
		_current = nullptr;
	else
		_current = _current->next;

	return *this;
}
//i--
template <typename TYPE>
typename list<TYPE>::iterator list<TYPE>::iterator::operator--(int i) {
	list<TYPE>::iterator it(*this);
	operator--();
	return it;
}
//--i
template <typename TYPE>
const typename list<TYPE>::iterator& list<TYPE>::iterator::operator--() {
	assert(!_list->empty());
	assert(_current != _list->_first);
	if (_current == nullptr)
		_current = _list->_first->previous;
	else
		_current = _current->previous;

	return *this;
}
//accès à un élément
template <typename TYPE>
TYPE& list<TYPE>::iterator::operator*() {
	assert(_current != nullptr);
	assert(!_list->empty());

	return _current->element;
}
//accès à un élément en mode const
template <typename TYPE>
const TYPE& list<TYPE>::iterator::operator*()const {
	assert(_current != nullptr);
	assert(!_list->empty());

	return _current->element;
}

////*******CLASS LIST********

//Constructeur
template<typename TYPE>
list<TYPE>::list(void) {
	_first = nullptr;
}
//Destructeur
template<typename TYPE>
list<TYPE>::~list(void) {
	clear();
}
//vide la liste
template<typename TYPE>
void list<TYPE>::clear() {
	list<TYPE>::iterator it = begin();

	while (!empty()) {//tant que la liste n'est pas vide on efface
		erase(it);
		it = begin();
	}

}
//verifie si la liste est vide
template<typename TYPE>
bool list<TYPE>::empty()const {
	return _first == nullptr;
}
//return premier
template <typename TYPE>
typename list<TYPE>::iterator list<TYPE>::begin()const {

	list<TYPE>::iterator it;	//Itérateur sur le début (1ere cellule)

	it._current = _first;
	it._list = this;

	return it;

}
//return NULL
template <typename TYPE>
typename list<TYPE>::iterator list<TYPE>::end()const {
	list<TYPE>::iterator it;	//Itérateur sur le début (1ere cellule)

	it._current = nullptr;
	it._list = this;

	return it;
}
//ajoute un element
template <typename TYPE>
typename list<TYPE>::iterator list<TYPE>::insert(iterator it, const TYPE& ele)
{
	assert(it._list == this);

	if (it._list->_first == nullptr) { //si on insere la premiere cellule
		it._current = _first->next = _first->previous = _first = new cellule<TYPE>(ele);
	}
	else if (it == end()) { //si on insere a la fin
		it._current = _first->previous = _first->previous->next = new cellule<TYPE>(ele, _first, _first->previous);
	}
	else {//les autres cas
		it._current->previous = it._current->previous->next = new cellule<TYPE>(ele, it._current, it._current->previous);

		if (it._current == _first) {//si l'element est egal au premier de la liste
			_first = _first->previous;
		}
		it--;
	}
	return it;
}
//efface un element
template <typename TYPE>
typename list<TYPE>::iterator list<TYPE>::erase(iterator I) {
	assert(!empty());
	assert(I._current != nullptr);
	assert(I._list == this);

	cellule<TYPE> *trash = I._current;

	if (trash == trash->next)
		I._current = _first = nullptr;
	else {//
		trash->next->previous = trash->previous;
		trash->previous->next = trash->next;
		if (I == begin())
			_first = _first->next;
		if (I == end()) {
			I--;
		}
		else
			I++;
	}
	delete trash;
	return I;
}
//efface plusieurs elements
template <typename TYPE>
typename list<TYPE>::iterator list<TYPE>::erase(iterator first, iterator last) {
	assert(!empty());
	list<TYPE>::iterator trash = first;
	list<TYPE>::iterator it = first;

	while (trash != last) {//Tant qu'on atteint pas le dernier iterateur, on efface
		it++;
		erase(trash);
		trash = it;
	}

	return last;
}
//pas ds la STL
template <typename TYPE>
TYPE& list<TYPE>::operator[](iterator I) {
	assert(I._list == this);
	return *I;

}
template <typename TYPE>
const TYPE& list<TYPE>::operator[](iterator I)const {
	assert(I._list == this);
	return *I;
}
//recherche un element a partir d'un iterateur
template <typename TYPE>
typename list<TYPE>::iterator list<TYPE>::operator()(const TYPE& ele, iterator it)const {
	assert(it._list == this);
	for (it; it != end(); it++)
		if (*it == ele)
			return it;
	return it;
}
//recherche un element
template <typename TYPE>
typename list<TYPE>::iterator list<TYPE>::operator()(const TYPE& ele)const {
	return operator()(ele, begin());
}
//operateur =
template <typename TYPE>
const list<TYPE>& list<TYPE>::operator=(const list<TYPE>& l) {
	if (this == &l)
		return *this;
	clear();

	if (!l.empty()) { //si la liste n'est pas vide
		list<TYPE>::iterator it = l.begin();
		list<TYPE>::iterator it2;
		//copie le premier element
		_first->previous = _first->next = _first = new cellule<TYPE>(l._first->element);
		it2 = begin();
		it2++;
		it++;

		while (it != l.end()) {//tant qu'on est pas a la fin, on insere les elements
			insert(it2, it._current->element);
			it++;
		}
	}

	return *this;
}