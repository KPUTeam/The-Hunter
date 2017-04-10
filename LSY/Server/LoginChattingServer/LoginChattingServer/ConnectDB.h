#pragma once

#include "Global.h"

class CConnectDB
{
	DECLARE_SINGLE(CConnectDB)

private:
	MYSQL*		connection = NULL, conn;
	MYSQL_RES*	sql_result;
	MYSQL_ROW	sql_row;
	int query_stat;

	my_ulonglong	iRowNum;	// 레코드셋의 개수
	unsigned int	iFieldNum;	// 필드의 개수


public:
	bool DBconnect();
	bool InputQuery(char* query);
	bool ShowResult();
};

