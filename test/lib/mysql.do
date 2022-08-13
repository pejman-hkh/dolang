
class mysqlStmt {
	stmt() {
		this.stmt = mysql_stmt( this.con, "select * from test where tid > ? " );
	}

	bind( b ) {
		mysql_bind( this.stmt, b);
	}
}

class mysql {
	connect() {
		this.con = mysql_connect("localhost", "root", "123", "test");
	}

	stmt() {
		this.stmt = new mysqlStmt();
	}

	bind( b ) {
		this.stmt.bind(b);
	}
}