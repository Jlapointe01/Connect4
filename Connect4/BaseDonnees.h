#pragma once
#include <iostream>
#include <string>

#include <windows.h>
#include "liste.hpp"
#include "vecteur.hpp"
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000
class BaseDonnees
{
private:
	SQLHANDLE sqlConnHandle;
	SQLHANDLE sqlStmtHandle;
	SQLHANDLE sqlEnvHandle;
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
public:
	void connexion();
	void deconnexion();
	void selectUsager();
	void ajouteUsager(char *nom, char *prenom);
	//void updatePointage();
	//int getPointage();
	//int getMeilleurPointage();
	void modifierPointage(string joueur, int win, int lose, int draw);
	void obtenirPointage(string joueur, int &win, int &lose, int &draw);
	void enregistrerGrille(string joueur1, string joueur2, vector<list<int>>  &grille);
	void obtenirGrille(string joueur1, string joueur2, vector<list<int>> &grille);
	void creerPartie(string joueur1, string joueur2);
	void supprimerPartie(string joueur1, string joueur2);
	string compacterColonne(vector<list<int>> &grille, int indexColonne);
	void decompacterColonne(vector<list<int>> &grille, int indexColonne, string &contenu);

};
