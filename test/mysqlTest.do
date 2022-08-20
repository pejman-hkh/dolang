import lib.mysql

main() {


	var db = new mysql("localhost", "root", "12c", "test")

	var stmt = db.prepare("select * from test1 where tid = 10 ").execute( /*[ 10 ]*/ )
	print("Test")

	var all = stmt.fetchAll();
	print(all);

/*	while( var fetch = stmt.next() ) {
		print( fetch )
	}
*/
	print("Test1")

	db.close()
}