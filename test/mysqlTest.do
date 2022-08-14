import lib.mysql

main() {
	
	var db = new mysql

	db.connect("localhost", "root", "12c", "test")

	db.prepare("select from test where tid = ? ").execute([ 10 ])

	while( var fetch = db.next() ) {
		print( fetch );
	}
}