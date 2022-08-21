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

	//printf("%s\n", sql->val);
	

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

	MYSQL_BIND  *bind = malloc( sizeof( MYSQL_BIND ) * (bnd->length ) );
	memset(bind, 0, sizeof (bind));
	//int *is_null = malloc( bnd->length + 1);
	//int *error = malloc( bnd->length + 1);
	//unsigned long *length = malloc( bnd->length + 1);

	memset(bind, 0, sizeof (bind));

	for( int i = 0; i < bnd->length; i++ ) {
		variable *v = bnd->value[i];

		if( v->type == 2 ) {

			bind[i].buffer_type = MYSQL_TYPE_LONG; 
			bind[i].buffer = (char *) &(v->val); 
	
		} else if( v->type == 1 ) {
			  bind[i].buffer_type = MYSQL_TYPE_STRING;
			  bind[i].buffer = (char *)v->val;
			  bind[i].buffer_length = strlen( v->val );
		}
	}

	if( mysql_stmt_bind_param( stmt->val, bind) ) {
		fprintf(stderr, " mysql_stmt_bind_param(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt->val));
		exit(0);		
	}
}

do_mysql_stmt_exec( variable *ths, variable *stmt ) {

	if (mysql_stmt_execute(stmt->val ))
	{
		fprintf(stderr, " mysql_stmt_execute(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt->val));
		exit(0);
	}
		
}

do_mysql_stmt_prepare_result( variable *ths, variable *stmt ) {

	MYSQL_RES  *prepare_meta_result;

	prepare_meta_result = mysql_stmt_result_metadata(stmt->val);
	if (!prepare_meta_result)
	{
		printf(" mysql_stmt_result_metadata(), \
		returned no meta information\n");
		printf(" %s\n", mysql_stmt_error(stmt->val));
		exit(0);
	}
	
	if (mysql_stmt_store_result(stmt->val))
	{
		fprintf(stderr, " mysql_stmt_store_result() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt->val));
		exit(0);
	}

	dovar( ret, prepare_meta_result, DOTYPE_INT );
	return ret;
}

do_mysql_stmt_fetch( variable *ths, variable *stmt, variable *res ) {

	MYSQL_RES  *prepare_meta_result = res->val;

	int num_fields = mysql_num_fields(prepare_meta_result);
	MYSQL_FIELD *fields = mysql_fetch_fields(prepare_meta_result);

	MYSQL_BIND * bind = malloc( sizeof( MYSQL_BIND ) * num_fields );
	int * real_length = malloc( sizeof(int) * num_fields );
	memset(bind, 0, sizeof (MYSQL_BIND) * num_fields);

	for (int i = 0; i < num_fields; ++i)

	{
		bind[i].buffer = 0; 
		bind[i].buffer_length = 0; 
		bind[i].length = &real_length[i]; 
	}

	if (mysql_stmt_bind_result(stmt->val, bind))
	{
		fprintf(stderr, " mysql_stmt_bind_result() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt->val));

		exit(0);
	}

	int status = mysql_stmt_fetch(stmt->val);
	if (status == 1 || status == MYSQL_NO_DATA) {
		dovar(r1, 0, DOTYPE_INT);
		return r1;
	} 

	array *arr1 = safe_alloc_new( &alloc, sizeof( array ) );
	array_init( arr1 );
	dovar( arr, arr1, DOTYPE_ARRAY );

	for (int i = 0; i < num_fields; ++i) {
		if (real_length[i] > 0) {
			int len = real_length[i];

			void *data = malloc(len);
			bind[i].buffer = data;
			bind[i].buffer_length = &real_length[i];
			mysql_stmt_fetch_column(stmt->val, &bind[i], i, 0);
			
			//printf("%d\n", fields[i].type );
			int type = DOTYPE_STRING;
			switch( fields[i].type ) {
				case 3 :
					type = DOTYPE_INT;
					data = atoi(data);
				break;
				case 253 : 
					type = DOTYPE_STRING;
				break;
			}
			dovar( fn, fields[i].name, DOTYPE_STRING );
			dovar( fv, data, type );

			array_set( arr, fn, fv );

		}
	}

	return arr;
}

do_mysql_stmt_close( variable *ths, variable *stmt, variable *res ) {
	MYSQL_RES  *prepare_meta_result = res->val;

	mysql_free_result(prepare_meta_result);
	if (mysql_stmt_close(stmt->val))
	{
	  fprintf(stderr, " failed while closing the statement\n");
	  //fprintf(stderr, " %s\n", mysql_error(mysql));
	  exit(0);
	}	
}

do_mysql_close( variable *ths, variable *msql ) {
	mysql_close(msql->val);
}

extern load() {
    array *arr = malloc( sizeof( array *) );
    array_init( arr );
    array_set1( arr, "mysql_connect", &do_mysql_connect);
    array_set1( arr, "mysql_stmt", &do_mysql_stmt);
    array_set1( arr, "mysql_stmt_bind", &do_mysql_stmt_bind);
    array_set1( arr, "mysql_stmt_exec", &do_mysql_stmt_exec);
    array_set1( arr, "mysql_stmt_prepare_result", &do_mysql_stmt_prepare_result);
    array_set1( arr, "mysql_stmt_fetch", &do_mysql_stmt_fetch);
    array_set1( arr, "mysql_stmt_close", &do_mysql_stmt_close);
    array_set1( arr, "mysql_close", &do_mysql_close);


    return arr;

}
