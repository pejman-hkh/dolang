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

do_mysql_bind( variable *ths, variable *stmt, variable *bd ) {
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
			bind[i]->is_null= is_null[i];
			bind[i]->length= length[i];
			bind[i]->error= error[i];
		}

		if( v->type == 1 ) {
			  bind[i]->buffer_type= MYSQL_TYPE_STRING;
			  bind[i]->buffer= (char *)v->val;
			  bind[i]->buffer_length= strlen( v->val );
			  bind[i]->is_null= is_null[i];
			  bind[i]->length= length[i];
			  bind[i]->error= error[i];
		}
	}
}