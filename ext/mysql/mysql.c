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
	stmt = mysql_stmt_init(msql);
	mysql_stmt_prepare(stmt, sql->val, strlen(sql->val) );

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
			bind[i]->buffer_type= MYSQL_TYPE_LONG;
			bind[i]->buffer= (char *)bnd->value[i];
			bind[i]->is_null= 0;
			bind[i]->length= 0;
		}

		if( v->type == 1 ) {
			  bind[i]->buffer_type= MYSQL_TYPE_STRING;
			  bind[i]->buffer= (char *)v->val;
			  bind[i]->buffer_length= strlen( v->val );
			  bind[i]->is_null= 0;
			  bind[i]->length= 0;
		}
	}

	mysql_stmt_bind_param( stmt->val, bind);
}

do_mysql_stmt_exec( variable *ths, variable *stmt ) {
	mysql_stmt_execute( stmt->val );	
}

do_mysql_stmt_fetch( variable *ths, variable *stmt ) {
	int column_count= mysql_num_fields(prepare_meta_result);
	printf("%d\n", column_count );

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
    array_set1( arr, "mysql_stmt_exec", &do_mysql_stmt_exec);
    array_set1( arr, "mysql_stmt_fetch", &do_mysql_stmt_fetch);
    array_set1( arr, "mysql_close", &do_mysql_close);


    return arr;

}
