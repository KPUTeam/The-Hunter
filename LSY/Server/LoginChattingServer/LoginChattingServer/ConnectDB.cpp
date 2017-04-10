#include "ConnectDB.h"
#include "NetworkManager.h"

DEFINITION_SINGLE(CConnectDB)

CConnectDB::CConnectDB()
	: sql_result(NULL),
	  sql_row(NULL),
	  query_stat(0),
	  iRowNum(0),
	  iFieldNum(0)
{
}


CConnectDB::~CConnectDB()
{
	mysql_close(connection);
}

bool CConnectDB::DBconnect()
{
	// �ʱ�ȭ
	mysql_init(&conn);

	// DB ����
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
	if (connection == NULL)
	{
		cout << stderr << "Mysql connection error : " << mysql_error(&conn) << endl;
		return false;
	}

	// �ѱۻ���� ���� �߰�
	mysql_query(connection, "set session character_set_connection=euckr;");
	mysql_query(connection, "set session character_set_results=euckr;");
	mysql_query(connection, "set session character_set_client=euckr;");

	return true;
}

bool CConnectDB::InputQuery(char* query)
{
//	query = "insert into user_info values('�̻�123', '12345')";
	query_stat = mysql_query(connection, query);

	if (query_stat != 0)
	{
		cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
		return false;
	}

	return true;
}

bool CConnectDB::ShowResult()
{
	sql_result = mysql_store_result(connection);
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		iRowNum = mysql_num_rows(sql_result); // �� �����ΰ�?
		iFieldNum = mysql_num_fields(sql_result); // �ʵ尡 ��ΰ�??

		for (int i = 0; i < iFieldNum; ++i)
		{
			if (sql_row[i] == NULL)	// NULL���� ���ִ� �����ʹ� �׳� �ǳ� ��
				continue;

			cout << sql_row[i] << " ";
		}
		cout << endl;
	}

	mysql_free_result(sql_result);

	return true;
}
