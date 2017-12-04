#pragma once
#include <iostream>
#include <list>   //Changer pour list.hpp
#include <vector> //changer pour vector.hpp

using namespace std;

class map {

private:
	vector<list<int>> _grille[7];
	list<int> _colonnes;
public:
	map();
	~map();
	
};
