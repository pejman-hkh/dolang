import lib.mysql

main() {


	let db = new mysql("localhost", "root", "12c", "test")
	var a = 10

	let si = db.prepare("insert into test1(tid, name) values(?,?) ").execute([5,"a test name"]);
	si.close();

	let stmt = db.prepare("select * from test1 where tid = ? ").execute( [ a ] )
	while( let fetch = stmt.next() ) {
		print(fetch)


		let stmt1 = db.prepare("select * from test2 where test1id = ? ").execute( [ fetch.id ] )

		let fetch1 = stmt1.next()
		if( fetch1 ) {
			print(fetch1)
		}
		
		stmt1.close()


	}
/*
	let su = db.prepare("update test1 set name = ? where id = ?").execute([ 'test for update', 1]);
	su.close();
*/
	stmt.close()

	db.close()
}