#include <mysql/mysql.h>
#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_mysql_connect( variable *ths, variable *host, variable *user, variable *pass, variable *db, variable *port ) {
	MYSQL *msql = NULL;
	msql = mysql_init(msql);

	int con = mysql_real_connect( msql, host->val, user->val, pass->val, db->val, port->val, NULL, CLIENT_FOUND_ROWS );
	dovar( ret, msql, DOTYPE_INT );
	return ret;
}

do_mysql_options( variable *ths, variable *msql, variable *opt, variable *opt_val ) {
	mysql_options( msql->val, (int)opt->val, (int)opt_val->val );
}

do_mysql_stmt( variable *ths, variable *msql, variable *sql ) {
	MYSQL_STMT    *stmt;
	stmt = mysql_stmt_init(msql->val);

	if (!stmt)
	{
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		exit(0);
	}

	printf("%s\n", sql->val);
	

	if ( mysql_stmt_prepare(stmt, sql->val, strlen(sql->val) ) )
	{
		fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}


	dovar( ret, stmt, DOTYPE_INT );
	return ret;	
}

do_mysql_stmt_bind( variable *ths, variable *stmt, variable *bd ) {
	array *bnd = bd->val;
	MYSQL_BIND  **bind = malloc( bnd->length + 1 );
	int *is_null = malloc( bnd->length + 1);
	int *error = malloc( bnd->length + 1);
	unsigned long *length = malloc( bnd->length + 1);


	for( int i = 0; i < bnd->length; i++ ) {
		variable *v = bnd->value[i];
		if( v->type == 2 ) {
			bind[i]->buffer_type = MYSQL_TYPE_LONG;
			bind[i]->buffer = v->val;
			bind[i]->buffer_length = sizeof(long);

		}

		if( v->type == 1 ) {
			  bind[i]->buffer_type = MYSQL_TYPE_STRING;
			  bind[i]->buffer = (char *)v->val;
			  bind[i]->buffer_length = strlen( v->val );
		}
	}

	if( mysql_stmt_bind_param( stmt->val, bind) ) {
		fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt->val));
		exit(0);		
	}

	if (mysql_stmt_execute(stmt->val ))
	{
		fprintf(stderr, " mysql_stmt_execute(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt->val));
		exit(0);
	}


}

/*do_mysql_stmt_exec( variable *ths, variable *stmt ) {
		
}*/

do_mysql_stmt_fetch( variable *ths, variable *stmt ) {
	MYSQL_RES  *prepare_meta_result;
	printf("jjjjjjjjjj %d\n", stmt->val);
	prepare_meta_result = mysql_stmt_result_metadata(stmt->val);
	if (!prepare_meta_result)
	{
		printf(" mysql_stmt_result_metadata(), \
		returned no meta information\n");
		printf(" %s\n", mysql_stmt_error(stmt->val));
		exit(0);
	}

	/* Get total columns in the query */
	int column_count= mysql_num_fields(prepare_meta_result);
	printf(" total columns in SELECT statement: %d\n",
	column_count);

	if (column_count != 4) /* validate column count */
	{
		printf(" invalid column count returned by MySQL\n");
		exit(0);
	}
	exit(0);
}

do_mysql_close( variable *ths, variable *msql ) {

}

extern load() {
    array *arr = malloc( sizeof( array *) );
    array_init( arr );
    array_set1( arr, "mysql_connect", &do_mysql_connect);
    array_set1( arr, "mysql_stmt", &do_mysql_stmt);
    array_set1( arr, "mysql_stmt_bind", &do_mysql_stmt_bind);
    //array_set1( arr, "mysql_stmt_exec", &do_mysql_stmt_exec);
    array_set1( arr, "mysql_stmt_fetch", &do_mysql_stmt_fetch);
    array_set1( arr, "mysql_close", &do_mysql_close);


    return arr;

}
