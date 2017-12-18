#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "mysql/mysql.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

int main(void)
{
	MYSQL			mysql;	
	MYSQL_RES * 	res;
	MYSQL_ROW		row;	
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_real_connect(&mysql, "localhost", "root", "qwe123root", "ePrinter", 3306, NULL, 0);	


	string sql_insert_statementa = "insert into sys_filetime (timepoint, size) VALUES (";
	for(int i=1; i <= 576; i++)
	{
		sql_insert_statementa = "insert into sys_filetime (timepoint, size) VALUES (";
		sql_insert_statementa += to_string(i);
		sql_insert_statementa += ", 50);"
		mysql_query(&mysql, sql_insert_statementa.c_str());
	}
	return 0;	
}