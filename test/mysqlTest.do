import lib.mysql


main() {


	let db = new mysql("localhost", "root", "12c", "test")

	for( let i = 0; i < 10; i++ ) {
		let a = [i,"a test name"]
		let s = db.prepare("insert into test1(tid, name) values(?,?) ").execute(a)
		s.close()
	}


	//print(b)

	let stmt = db.prepare("select * from test1 where tid = ? ").execute( [ 5 ] )
	while( let fetch = stmt.next() ) {
		print(fetch)


		let stmt1 = db.prepare("select * from test2 where test1id = ? ").execute( [ fetch.id ] )

		let fetch1 = stmt1.next()
		if( fetch1 ) {
			print(fetch1)
		}
		
		stmt1.close()


	}
	stmt.close()

	let su = db.prepare("update test1 set name = ? where id = ?").execute([ 'test for update', 1])
	su.close()
	

	db.close()
	
	let cn = mysql_connect("localhost", "root", "12c", "test");

	for( var i = 0; i < 10; i++ ) {
		let a = [ i, "this is test1"];
		mysql_query(cn, "insert into test1(tid, name) values(?,?)", a )
	}


}