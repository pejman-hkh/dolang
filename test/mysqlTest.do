import lib.mysql

main() {


	var db = new mysql("localhost", "root", "12c", "test")

	var stmt = db.prepare("select * from test where tid = ? ").execute( [ 10 ] );

	while( var fetch = stmt.next() ) {
		print( fetch )
	}


	db.close();
}