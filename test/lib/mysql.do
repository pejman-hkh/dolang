
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
		let ret = [];
		let i = 0;
		while( let fetch = this.next() ) {
			ret[i++] = fetch;
		}

		return ret;
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