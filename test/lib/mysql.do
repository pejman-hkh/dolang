
class mysqlStmt {
	construct( sql, con ) {

		this.stmt = mysql_stmt( con, sql );
		return this;
	}

	bind( b ) {
		//print( this )

		mysql_stmt_bind( this.stmt, b);
		mysql_stmt_exec( this.stmt );
		
		this.res = mysql_stmt_prepare_result( this.stmt );

		return this;
	}

	fetchAll() {
		return mysql_stmt_fetch_all( this.stmt, this.res );
		//return [ { id : 1, name : 'test'} ];
	}

	next() {
		return mysql_stmt_fetch( this.stmt, this.res );
		//return [ { id : 1, name : 'test'} ];
	}

	close() {
		mysql_stmt_close( this.stmt, this.res );
	}
}

class mysql {
	construct( host, user, pass, db ) {
		this.connect( host, user, pass, db );
		return this;
	}

	connect( host, user, pass, db ) {
		this.con = mysql_connect(host, user, pass, db);
	}

	prepare( sql ) {
		
		this.stmt = new mysqlStmt( sql, this.con );

		return this;
	}

	execute( b ) {

		return this.stmt.bind( b );
	}

	close() {

	}
}