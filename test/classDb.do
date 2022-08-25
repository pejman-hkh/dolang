class db {

	connect( host, db, user, pass ) {
		print("connect to db\n");
	}

	prepare( sql ) {
		print("prepare sql\n");
		return this;
	}


	execute() {
		print("execute sql\n");
	}

	next() {
		print("in next\n");
		return 0;
	}
}


main() {
	var a = new db;

	a.connect("localhost", "test", "root", "12c");


	a.prepare("select * from test ").execute();

	while( a.next() ) {

	}

}