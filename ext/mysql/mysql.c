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

	MYSQL_BIND  *bind = malloc( sizeof( MYSQL_BIND * ) * (bnd->length + 1) );
	int *is_null = malloc( bnd->length + 1);
	int *error = malloc( bnd->length + 1);
	unsigned long *length = malloc( bnd->length + 1);


	for( int i = 0; i < bnd->length; i++ ) {
		variable *v = bnd->value[i];
		if( v->type == 2 ) {
			bind[i].buffer_type = MYSQL_TYPE_LONG;
			bind[i].buffer = (char *)v->val;
			//bind[i]->buffer_length = sizeof(long);

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

do_mysql_stmt_fetch( variable *ths, variable *stmt ) {

	//stmt->val

	MYSQL_RES  *prepare_meta_result;

	prepare_meta_result = mysql_stmt_result_metadata(stmt->val);
	if (!prepare_meta_result)
	{
		printf(" mysql_stmt_result_metadata(), \
		returned no meta information\n");
		printf(" %s\n", mysql_stmt_error(stmt->val));
		exit(0);
	}

	int num_fields = mysql_num_fields(prepare_meta_result);
	MYSQL_FIELD *fields;
	fields = mysql_fetch_fields(prepare_meta_result);

/*	

	for(int i = 0; i < num_fields; i++)
	{
	   printf("Field %u is %s\n", i, fields[i].name);
	   printf("type %d\n", fields[i].type);
	}*/
	//exit(0);

	/* Get total columns in the query */
	//int num_fields = mysql_num_fields(prepare_meta_result);
	//printf(" total columns in SELECT statement: %d\n",
	//column_count);

	char str_data[50];
	int is_null[3];

	//unsigned long length[4];

	MYSQL_BIND * bind = malloc( sizeof( MYSQL_BIND ) * num_fields );
	int * int_data = malloc( sizeof(int) * num_fields );
	memset(bind, 0, sizeof (MYSQL_BIND) * num_fields);

	for (int i = 0; i < num_fields; ++i)

	{
		bind[i].buffer_type = fields[i].type; 
		bind[i].is_null = &is_null[i];

		switch (fields[i].type)
		{

		case MYSQL_TYPE_LONG:
			bind[i].buffer = (char *) &(int_data[i]); 
			bind[i].buffer_length = sizeof (int_data); 
			break;
		default:
			fprintf(stderr, "ERROR: unexpected type: %d.\n", fields[i].type); exit(1);

		}

	}


	if (mysql_stmt_bind_result(stmt->val, bind))
	{
		fprintf(stderr, " mysql_stmt_bind_result() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt->val));

		exit(0);
	}

	if (mysql_stmt_store_result(stmt->val))
	{
		fprintf(stderr, " mysql_stmt_store_result() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt->val));
		exit(0);
	}



	while (1)

	{

		int status = mysql_stmt_fetch(stmt->val);
		if (status == 1 || status == MYSQL_NO_DATA) {
			break;
		} 



		for (int i = 0; i < num_fields; ++i)

		{

		switch (bind[i].buffer_type)

		{
			case MYSQL_TYPE_LONG:
				if (*bind[i].is_null) 
					printf(" val[%d] = NULL;", i);
				else {
					printf("%s ", fields[i].name );
					printf(" val[%d] = %ld;", i, (long) *((int *) bind[i].buffer));
				}

			break;

			default:

			printf(" unexpected type (%d)\n", bind[i].buffer_type);

			}

		}

		printf("\n");

	}




/*	if (mysql_stmt_fetch(stmt->val))
	{
		fprintf(stderr, " mysql_stmt_fetch() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt->val));
		exit(0);
	}*/

/*	for( int i = 0; i < column_count; i++ ) {



		if (mysql_stmt_fetch(stmt->val ))
		{
			fprintf(stderr, " mysql_stmt_fetch(), failed\n");
			fprintf(stderr, " ssssssss %s\n", mysql_stmt_error(stmt->val));
			exit(0);
		}
	}
*/
	while ( ! mysql_stmt_fetch( stmt->val ) )
	{

		printf("dddd\n");
		for( int i = 0; i < num_fields; i++ ) {
			//printf("%d\n", real_length[i]);
		/*	if (real_length[i] > 0)
			{
				char * data = malloc(real_length);
				bind[i].buffer= data;
				bind[i].buffer_length= real_length;
				mysql_stmt_fetch_column(stmt, bind, 0, 0);
				printf("%s\n", data);
			}*/
		}


		//printf("%s\n", str_data );
	}

	//exit(0);

/*	mysql_stmt_fetch( stmt->val );
	int l = length[0];

	//printf("length is : %d\n", *(l) );
	printf("length is : %d\n", str_data );
	exit(0);*/

	dovar( ret, 1, DOTYPE_INT )
	return ret;
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
