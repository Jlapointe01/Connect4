#include "BaseDonnees.h"

using namespace std;

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

		//S'il y a un probl�me avec la requ�te on quitte l'application sinon on affiche le r�sultat
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT usagerprenom, usagernom FROM usager", SQL_NTS)) {
			throw string("Erreur dans la requ�te");
		}
		else {
			//D�clarer les variables d'affichage

			SQLCHAR nom[SQL_RESULT_LEN];
			SQLLEN ptrnom;

			SQLCHAR prenom[SQL_RESULT_LEN];
			SQLLEN ptrprenom;

			while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, prenom, SQL_RESULT_LEN, &ptrprenom);
				SQLGetData(sqlStmtHandle, 2, SQL_CHAR, nom, SQL_RESULT_LEN, &ptrnom);

				//Afficher le r�sultat d'une requ�te			
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
		Param�tre SQLBindParameter:
		- Handler de la requ�te
		- No du param�tre (commence � 1)
		- Est-ce un param�tre de type Input ou Output
		- Quel est le type de la variable en C++
		- Quel est le type de la variable en SQL
		- Quelle est la taille de la colonne dans la BD
		- Nombre de d�cimal
		- Quelle variable ou donn�es (pointeur)
		- Longueur du buffer
		- Pointeur du buffer
		*/
		retcode = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 50, 0, prenom, 0, 0);
		retcode = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 50, 0, nom, 0, 0);

		retcode = SQLPrepare(sqlStmtHandle, (SQLWCHAR*)L"INSERT INTO Usager (usagerprenom, usagernom) VALUES (?, ?)", SQL_NTS);

		retcode = SQLExecute(sqlStmtHandle);

		if (SQL_SUCCESS != retcode) {
			throw string("Erreur dans la requ�te");
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