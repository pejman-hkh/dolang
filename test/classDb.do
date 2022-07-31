class db {
	connect() {

	}

	prepare() {

	}


	execute() {
	
	}
}


main() {
	var db = new db;

	db.connect();


	db.prepare("select * from test ").execute();

	while( db.next() ) {

	}

}