import lib.mysql


class model {
	construct( db ) {
		this.db = db;
	}

	save() {
		let b = {};
		let i = 0;
		for( let k,v in this ) {
			let t = typeof(v)

			if( t != 4 && t != 7 && t != 3 && k != "class" ) {
				b[k] = v;
			}
		}

		print( b )
	}

	sql( sql ) {
		this.sql = 'select * from '+this.class+' '+sql;
		return this;
	}

	findFirst( bind ) {
		let ret = this.find( bind );
		if( ret[0] ) {
			return ret[0];
		}
		else {
			return [];
		}
	}

	find( bind ) {

		let stmt = this.db.prepare( this.sql ).execute( bind );
		return stmt.fetchAll();
	}
}


class test1 extends model {

}

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
	

	//db.close()
	


	/*
	Manually connect to mysql and insert and fetch some results
	*/
/*	let cn = mysql_connect("localhost", "root", "12c", "test", 3306);
	for( var i = 0; i < 10; i++ ) {
		let a = [ i, "this is test1"];
		mysql_query(cn, "insert into test1(tid, name) values(?,?)", a )
	}

	let stmt = mysql_stmt( cn, "select * from test1 where id = ? ");
	mysql_stmt_bind(stmt, [1]);
	mysql_stmt_exec(stmt);
	let res = mysql_stmt_prepare_result(stmt);
	let fetch = mysql_stmt_fetch( stmt, res );
	print( fetch )

	mysql_free_result(res);
	mysql_stmt_close( stmt );
	mysql_close( cn );
*/


	let m = new test1(db);
	m.tid = 1500;
	m.name = "test from model";
	m.save();

	let fetch = m.sql("where name = ?").findFirst(['test from model']);
	print( fetch );

}