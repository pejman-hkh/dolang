
class mysqlStmt {
	stmt() {
		this.stmt = mysql_stmt( this.con, "select * from test where tid > ? " );
	}

	bind( b ) {
		mysql_stmt_bind( this.stmt, b);
		mysql_stmt_exec( this.stmt );
	}

	next() {
		return mysql_stmt_fetch( this.stmt );
	}
}

class mysql {
	connect( host, user, pass, db ) {
		this.con = mysql_connect("localhost", "root", "123", "test");
	}

	prepare() {
		this.stmt = new mysqlStmt();
	}

	exec( b ) {
		this.stmt.bind(b);
	}
}