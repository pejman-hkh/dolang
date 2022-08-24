class mysqlStmt {
	construct( sql, con ) {
		this.res = 0;
		this.con = con;
		this.stmt = mysql_stmt( con, sql );

		return this;
	}

	bind( b ) {
		mysql_stmt_bind( this.stmt, b);
		mysql_stmt_exec( this.stmt );

		return this;
	}

	fetchAll() {
		let ret = [];
		let i = 0;
		while( let fetch = this.next() ) {
			ret[i++] = fetch;
		}

		return ret;
	}

	next() {

		if( ! this.res ) {
			this.res = mysql_stmt_prepare_result( this.stmt );
		}

		return mysql_stmt_fetch( this.stmt, this.res );
	}

	close() {

		if( this.res ) {
			mysql_free_result( this.res );
		}

		mysql_stmt_close( this.stmt );
	}
}

class mysql {
	construct( host, user, pass, db ) {
		this.connect( host, user, pass, db );
		return this;
	}

	connect( host, user, pass, db ) {

		this.con = mysql_connect(host, user, pass, db, 3306);
	}

	query( sql, bind ) {
		mysql_query( this.con, sql, bind );
	}

	prepare( sql ) {

		this.sql = sql;
		return this;
	}

	execute( b ) {

		let stmt = new mysqlStmt( this.sql, this.con );
		stmt.bind( b );
		return stmt;
	}

	close() {
		mysql_close( this.con );
	}
}