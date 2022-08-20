import lib.mysql

main() {


	var db = new mysql("localhost", "root", "12c", "test")

	var stmt = db.prepare("select * from test1 where tid = ? ").execute( [ 10 ] )
	var all = stmt.fetchAll();
	print(all);

/*	while( var fetch = stmt.next() ) {
		print( fetch )
	}
*/

	db.close()
}