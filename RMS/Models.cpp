#ifndef MODELS_CPP
#define MODELS_CPP
#include "Models.h"
Models::Models() {

	sqlConnHandle = NULL;
	sqlStmtHandle = NULL;
	cout << "Connecting   0%... Initialize." << endl;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle)) {
		cout << "Connecting  15%... SQLAllocHandle." << endl;
	}
	if (SQL_SUCCESS == SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) {
		cout << "Connecting  30%... SQLSetEnvAttr." << endl;
	}
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle)) {
		cout << "Connecting  45%... SQLAllocHandle." << endl;
	}
	switch (SQLDriverConnect(sqlConnHandle,
		NULL,
		(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost;DATABASE=qlNhaHang;Trusted=true;UID=sa;PWD=truongduc910",
		SQL_NTS,
		retconstring,
		1024,
		NULL,
		SQL_DRIVER_NOPROMPT))
	{
	case SQL_SUCCESS:
		cout << "Connecting  95%... Successfully connected to SQL Server." << endl;
		break;
	case SQL_SUCCESS_WITH_INFO:
		cout << "Connecting  95%... Successfully connected to SQL Server." << endl;
		break;
	default:
		cout << "Connecting fail... Could not connect to SQL Server." << endl;
		break;
	}
	cout << "Connecting 100%... Ready to query." << endl;
}

int Models::get(List<Menu>& menus) {
	cout << "SQL  [SELECT] ";
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
	SQLCHAR sqlVersion[SQL_RESULT_LEN];
	SQLLEN ptrSqlVersion;
	if (SQL_SUCCESS == SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT * FROM MENU", SQL_NTS)) {
		Menu temp;
		menus.empty();
		cout << "success";
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
			SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &temp.foodId, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 2, SQL_CHAR, &temp.name, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 3, SQL_DOUBLE, &temp.cost, SQL_RESULT_LEN, &ptrSqlVersion);
			menus.insert(temp);
		}
	}
	else {
		cout << "fail" << endl;
	}
	return true;
}
int Models::insert(const Menu& menu) {
	string command = "INSERT INTO MENU (FoodName, Cost) VALUES('";
	command += string(menu.name) + AND + to_string(menu.cost);
	command += "')";
	std::wstring stemp = s2ws(command);
	LPCWSTR result = stemp.c_str();
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
	SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)result, SQL_NTS);

	//GET ID OF THIS RECORD
	int id = 0;
	SQLCHAR sqlVersion[SQL_RESULT_LEN];
	SQLLEN ptrSqlVersion;
	command = "SELECT FoodID FROM MENU WHERE FoodName = '";
	command += string(menu.name);
	command += "'";
	stemp = s2ws(command);
	result = stemp.c_str();
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
	SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)result, SQL_NTS);
	if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
		SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &id, SQL_RESULT_LEN, &ptrSqlVersion);
	}
	return id;
}
int Models::findAndRemove(const Menu& menu) {
	cout << "SQL  [DELETE] ";
	string command = "DELETE MENU WHERE FoodId ='";
	command += to_string(menu.foodId);
	command += "'";
	std::wstring stemp = s2ws(command);
	LPCWSTR result = stemp.c_str();
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
	switch (SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)result, SQL_NTS)) {
	case SQL_SUCCESS:
		cout << "Success!" << endl;
		break;
	case SQL_SUCCESS_WITH_INFO:
		cout << "Success!" << endl;

		break;
	default:
		cout << "Fail!" << endl;
		break;
	}
	return true;
}
////void Models::menuUpdate(const Menu& menu) {
////	string command = "UPDATE MENU SET FoodName = '";
////	command += string(menu.foodName) + s;
////	command += ", Cost = '";
////	command += to_string(menu.cost) + s;
////	command += " WHERE FoodId = '";
////	command += to_string(menu.foodId) + s;
////	std::wstring stemp = s2ws(command);
////	LPCWSTR result = stemp.c_str();
////	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
////	switch (SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)result, SQL_NTS)) {
////	case SQL_SUCCESS:
////		cout << "[UPDATE] Successful!" << endl;
////		break;
////	case SQL_SUCCESS_WITH_INFO:
////		cout << "[UPDATE] Successful!" << endl;
////		break;
////	default:
////		cout << "[UPDATE] Fail!" << endl;
////		break;
////	}
////}
wstring Models::s2ws(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
}
#endif