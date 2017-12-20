#include "BaseDonnees.h"

#include <cstring>

using namespace std;
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000
//selectUsager();
//
//char nom[] = { "Dufour1" };
//char prenom[] = { "Simon" };
//
//ajouteUsager(nom, prenom);
////suppUsager();
//
////Pause avant de fermer le programme
//cout << "\nPress any key to exit...";
//getchar();
void BaseDonnees::connexion()
{
	//Initialisations
	sqlConnHandle = NULL;
	sqlStmtHandle = NULL;


	try
	{
		//Allocations
		if (!((SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle)) || (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) || (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle)))) {
			//Affichage
			cout << "Tentative de connexion";
			cout << "\n";

			//Connexion au SQL Server
			switch (SQLDriverConnect(sqlConnHandle, NULL,
				(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=Connect4;UID=Connect4;PWD=abc123;", SQL_NTS, retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT)) {

			case SQL_SUCCESS:
				cout << "Connexion reussi";
				cout << "\n";


				break;
			case SQL_SUCCESS_WITH_INFO:
				cout << "Connexion reussi";
				cout << "\n";

				break;
			case SQL_INVALID_HANDLE:
				throw string("Erreur de connexion");

			case SQL_ERROR:
				throw string("Erreur de connexion");
			default:
				throw string("Erreur");
				break;
			}

			//Si la connexion est impossible
			if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
				throw string("Connexion impossible");
		}
	}
	catch (string const& e)
	{
		cout << e << "\n";
		deconnexion();

		cout << "\n Appuyer sur Retour pour fermer ...";
		getchar();
		exit(1);
	}

}

void BaseDonnees::deconnexion() {

	cout << "Deconnexion ..." << "\n";

	SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
	SQLDisconnect(sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
}

void BaseDonnees::selectUsager() {

	try
	{
		connexion();

		//S'il y a un problème avec la requête on quitte l'application sinon on affiche le résultat
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT username FROM tblJoueur", SQL_NTS)) {
			throw string("Erreur dans la requête");
		}
		else {
			//Déclarer les variables d'affichage

			SQLCHAR nom[SQL_RESULT_LEN];
			SQLLEN ptrnom;

			SQLCHAR prenom[SQL_RESULT_LEN];
			SQLLEN ptrprenom;

			while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, prenom, SQL_RESULT_LEN, &ptrprenom);
				SQLGetData(sqlStmtHandle, 2, SQL_CHAR, nom, SQL_RESULT_LEN, &ptrnom);

				//Afficher le résultat d'une requête			
				cout << nom << "  " << prenom << endl;
			}
		}
	}
	catch (string const& e)
	{
		cout << e << "\n";
		deconnexion();
	}

	deconnexion();
}



void BaseDonnees::ajouteUsager(char *nom, char *prenom) {

	try
	{
		connexion();

		SQLRETURN retcode;

		/*
		Paramètre SQLBindParameter:
		- Handler de la requête
		- No du paramètre (commence à 1)
		- Est-ce un paramètre de type Input ou Output
		- Quel est le type de la variable en C++
		- Quel est le type de la variable en SQL
		- Quelle est la taille de la colonne dans la BD
		- Nombre de décimal
		- Quelle variable ou données (pointeur)
		- Longueur du buffer
		- Pointeur du buffer
		*/
		retcode = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 50, 0, prenom, 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 50, 0, nom, 0, 0);

		retcode = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"INSERT INTO Usager (usagerprenom, usagernom) VALUES (?, ?)", SQL_NTS);

		retcode = SQLExecute(sqlStmtHandle);

		if (SQL_SUCCESS != retcode) {
			throw string("Erreur dans la requête");
		}
	}
	catch (string const& e)
	{
		cout << e << "\n";

		deconnexion();

		getchar();
		exit(1);
	}

	deconnexion();
}
void BaseDonnees::modifierPointage(int idjoueur, int win, int lose, int draw)
{
	try
	{
		connexion();

		SQLRETURN retcode;

		retcode = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &idjoueur, 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &win, 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &lose, 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &draw, 0, 0);

		retcode = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"EXEC modifierPointage(?, ?, ?, ?)", SQL_NTS);
		retcode = SQLExecute(sqlStmtHandle);

		if (SQL_SUCCESS != retcode)
		{
			throw string("Erreur dans la requête");
		}
	}
	catch (string const& e)
	{
		cout << e << endl;
		deconnexion();
	}

	deconnexion();
}

void BaseDonnees::obtenirPointage(int idjoueur, int &win, int &lose, int &draw)
{
	try
	{
		connexion();

		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT username, win, lose, draw FROM vue_pointage WHERE idjoueur = ?", SQL_NTS))
		{
			throw string("Erreur dans la requête");
		}
		else
		{
			SQLCHAR username[SQL_RESULT_LEN];
			SQLLEN ptrusername;
			//SQLINTEGER w;
			//SQLINTEGER l;
			//SQLINTEGER d;

			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, username, SQL_RESULT_LEN, &ptrusername);
			SQLGetData(sqlStmtHandle, 2, SQL_INTEGER, &win, 0, 0);
			SQLGetData(sqlStmtHandle, 3, SQL_INTEGER, &lose, 0, 0);
			SQLGetData(sqlStmtHandle, 4, SQL_INTEGER, &draw, 0, 0);
		}
	}
	catch (string const& e)
	{
		cout << e << endl;
		deconnexion();
	}

	deconnexion();
}

void BaseDonnees::enregistrerGrille(int idjoueur1, int idjoueur2, vector<list<int>>  &grille)
{
	try
	{
		connexion();

		SQLRETURN retcode;

		retcode = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 6, 0, &compacterColonne(grille, 0), 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 6, 0, &compacterColonne(grille, 1), 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 6, 0, &compacterColonne(grille, 2), 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 6, 0, &compacterColonne(grille, 3), 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 6, 0, &compacterColonne(grille, 4), 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 6, 0, &compacterColonne(grille, 5), 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 6, 0, &compacterColonne(grille, 6), 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &idjoueur1, 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &idjoueur2, 0, 0);
		retcode = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"UPDATE grilles SET colonne1 = ?, colonne2 = ?, colonne3 = ?, colonne4 = ?, colonne5 = ?, colonne6 = ?, colonne7 = ? WHERE idjoueur1 = ? AND idjoueur2 = ?",SQL_NTS);
		retcode = SQLExecute(sqlStmtHandle);

		if (SQL_SUCCESS != retcode)
		{
			throw string("Erreur dans la requête");
		}
	}
	catch (string const& e)
	{
		cout << e << endl;
		deconnexion();
	}

	deconnexion();
}

void BaseDonnees::obtenirGrille(int idjoueur1, int idjoueur2, vector<list<int>> &grille)
{
	try
	{
		BaseDonnees::connexion();

		SQLRETURN retcode;

		retcode = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &idjoueur1, 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &idjoueur2, 0, 0);
		retcode = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"SELECT colonne1, colonne2, colonne3, colonne4, colonne5, colonne6, colonne7 FROM grilles WHERE idjoueur1 = ? AND idjoueur2 = ?", SQL_NTS);
		retcode = SQLExecute(sqlStmtHandle);

		if (SQL_SUCCESS != retcode)
		{
			throw string("Erreur dans la requête");
		}
		else
		{
			SQLCHAR colonne1[SQL_RESULT_LEN];
			SQLLEN ptrcolonne1;
			SQLCHAR colonne2[SQL_RESULT_LEN];
			SQLLEN ptrcolonne2;
			SQLCHAR colonne3[SQL_RESULT_LEN];
			SQLLEN ptrcolonne3;
			SQLCHAR colonne4[SQL_RESULT_LEN];
			SQLLEN ptrcolonne4;
			SQLCHAR colonne5[SQL_RESULT_LEN];
			SQLLEN ptrcolonne5;
			SQLCHAR colonne6[SQL_RESULT_LEN];
			SQLLEN ptrcolonne6;
			SQLCHAR colonne7[SQL_RESULT_LEN];
			SQLLEN ptrcolonne7;

			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, colonne1, SQL_RESULT_LEN, &ptrcolonne1);
			SQLGetData(sqlStmtHandle, 2, SQL_CHAR, colonne2, SQL_RESULT_LEN, &ptrcolonne2);
			SQLGetData(sqlStmtHandle, 3, SQL_CHAR, colonne3, SQL_RESULT_LEN, &ptrcolonne3);
			SQLGetData(sqlStmtHandle, 4, SQL_CHAR, colonne4, SQL_RESULT_LEN, &ptrcolonne4);
			SQLGetData(sqlStmtHandle, 5, SQL_CHAR, colonne5, SQL_RESULT_LEN, &ptrcolonne5);
			SQLGetData(sqlStmtHandle, 6, SQL_CHAR, colonne6, SQL_RESULT_LEN, &ptrcolonne6);
			SQLGetData(sqlStmtHandle, 7, SQL_CHAR, colonne7, SQL_RESULT_LEN, &ptrcolonne7);
			string col1((const char*)colonne1);
			string col2((const char*)colonne2);
			string col3((const char*)colonne3);
			string col4((const char*)colonne4);
			string col5((const char*)colonne5);
			string col6((const char*)colonne6);
			string col7((const char*)colonne7);
			decompacterColonne(grille, 0, col1);
			decompacterColonne(grille, 1, col2);
			decompacterColonne(grille, 2, col3);
			decompacterColonne(grille, 3, col4);
			decompacterColonne(grille, 4, col5);
			decompacterColonne(grille, 5, col6);
			decompacterColonne(grille, 6, col7);
		}
	}
	catch (string const& e)
	{
		cout << e << endl;
		deconnexion();
	}

	deconnexion();
}

void BaseDonnees::creerPartie(string joueur1, string joueur2)
{
	try
	{
		connexion();

		SQLRETURN retcode;

		retcode = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &joueur1, 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &joueur2, 0, 0);
		retcode = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"EXEC creerPartie(?, ?)",SQL_NTS);
		retcode = SQLExecute(sqlStmtHandle);

		if (SQL_SUCCESS != retcode)
		{
			throw string("Erreur dans la requête");
		}
	}
	catch (string const& e)
	{
		cout << e << endl;
		deconnexion();
	}

	deconnexion();
}

void BaseDonnees::supprimerPartie(string joueur1, string joueur2)
{
	try
	{
		connexion();

		SQLRETURN retcode;

		retcode = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &joueur1, 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &joueur2, 0, 0);
		retcode = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"EXEC supprimerPartie(?, ?)", SQL_NTS);
		retcode = SQLExecute(sqlStmtHandle);

		if (SQL_SUCCESS != retcode)
		{
			throw string("Erreur dans la requête");
		}
	}
	catch (string const& e)
	{
		cout << e << endl;
		deconnexion();
	}

	deconnexion();
}

string BaseDonnees::compacterColonne(vector<list<int>> &grille, int indexColonne)
{
	string colonne = "";
	list<int>::iterator it;
	it = grille[indexColonne].begin();
	for (int i = 0; i < 6; i++)
	{
		colonne.append(to_string(grille[indexColonne][it]));
		it++;
	}
	return colonne;
}

void BaseDonnees::decompacterColonne(vector<list<int>> &grille, int indexColonne, string &contenu)
{
	list<int>::iterator it;
	it = grille[indexColonne].end();
	for (int i = 0; i < 6; i++)
	{
		grille[indexColonne].insert(it,contenu[i]);
	}
}
