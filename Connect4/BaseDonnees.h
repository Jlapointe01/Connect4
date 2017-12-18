#pragma once
#include <iostream>
#include <string>

#include <windows.h>
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

};
