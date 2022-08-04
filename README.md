**Under Development**


Do lang is simple jit language and is very similar to js

in test directory there are many samples of language

Do lang now just supports x86 cpu but in the next time it will support another cpu structurs ...


Make it :

`make -B`

Usage :

`./dolang filename.do `


# Samples

```


class Array {
	push() {

	}
}

main() {
	
}
```

```

var a;

test() {
	print(a);
}

main() {
	a = [];
	a['t'] = 'test1';
	a['2'] = 'test2';
	a['3'] = 'test3';
	test();
}
```

```
each( arr, cb ) {
	for( var k,v in arr ) {
		cb( k, v );
	}
}


main() {
	
	var arr = [0 : ['name' : 'second'], 1 : ['name' : 'test1'] ];

	each(arr, func( k, v) {
		print( k );
		print( v );
	});
}
```

```
main() {


	var s = "test s";
	var l = func( a ) {
	
		print("in callback\n");
		print( a );
	}

	l("test\n");

	var add = func( a , b ) {
		return a+b;
	}


	print( "sum of 2 and 4 is : "+add( 2, 4)+"\n" );

}
```

```
test( callback ) {
	callback(1,2);
}

main() {
	test(func( a, b ) {
		print(a+b);
	});
}
```

```
class test {
	a(r, cb) {
		var l = {};
		l.f = func( d ) {
			print(d);
		}

		cb(l);
		print("here1\n");
	}
}

main() {
	
	var a = new test;
	a.a('/', func( b ) {
		b.f("is test\n");

	});

	print("here\n");
}
```

```
main() {

	var o = { 'a' : { 'b' : func(a) { print(a); } } };
	print(o);

	o.a.b("in function in object");
}
```

```
main() {


	var o = {};
	o.a = func(a) {
		print(a);
	}
	
	print( o );

	o.a("in function in object1\n");

	o.a = func( a ) {
		print(a + "+++++\n");
	}

	o.a("in function in object1");
}
```

```
test() {
	return func( a ) {
		print(a);
	};
}


main() {
	var l = test();	
	l("test\n");
}
```

```
class test {
	var a;

	b() {
		this.a = "this is test\n";
	}

	c() {
		print( this.a );
	}

}

main() {
	
	var a = new test;
	a.b();
	a.a = "test1\n";
	a.c();	


	var b = new test;
	b.b();

	b.c();

	print( a );
}
```

```
class test {
	a() {
		this.a = "this is a";
		print( this.c );
		return this;
	}

	b() {
		this.b = "this is b";
		print( this.c );
	}
}

main() {
	
	var a = new test;
	a.c = "this is c\n";

	a.a().b();
	print( a );

}
```

```
class test {
	var a;

	b() {
		this.a = "this is test\n";
	}

	c() {
		print( this.a );
	}

}

main() {
	
	var n = new test;
	n.b();
	n.a = "test1\n";
	n.c();
}
```

```
class test {
	c() {
		this.s = "ddd";
	}

	b() {
		print("test ...");
		this.d = 123;
	}

	a() {
		this.b();
		return this;
	}

}

main() {
	var a = new test;
	a.a().c();
	print( a );
}
```

```
class test {
	a() {
		//this.d = "ddd";

		print("in a\n");
		return this;
	}		

	b() {
		print("in b\n");
	}	
}


test(d) {
	d.a();
}

main() {
	
	var a = new test;
	print( a );
	a.a();

	test( a );

	a.b();
}
```

```
class test {
	construct( a ) {
		print( a );
		
		return this;
	}

	a() {
		print("in a\n");
	}
}

main() {
	new test("test");
}
```

```
class test {
	a() {
		this.b();
	}

	b() {
		print("in b\n");
	}
}

main() {
	var a = new test;
	a.a();
}
```

```
class test {
	construct( a ) {
		print("in construct " + a +"\n" );
		print("here\n")
	}

	a() {
		print("in a\n");
		return this;
	}

	b() {
		print("in b\n");
	}
}


main() {
	var a = new test("test");
	a.a().b();


	print( a );
}
```

```
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
	var db = new db;

	db.connect("localhost", "test", "root", "12c");


	db.prepare("select * from test ").execute();

	while( db.next() ) {

	}

}
```

```
test_a( this ) {
	this.a = "test_a";
}

test_b( this ) {
	this.b = "test_b";
}

main() {
	
	var a = {};
	test_a( a );
	test_b( a );
	print( a );	



	var b = {};
	test_a( b );
	b.a = "test_a2";
	test_b( b );
	b.b = "test_b2";
	print( b );
	
	print( a );

}
```

```
test_a( this ) {
	this.a = "test_a";
}

main() {
	
	var a = {};
	test_a( a );
	print( a );	
}
```

```

class a {
	a() {
		print("in a\n");
	}
}

class b extends a {
	b() {

	}
}


main() {
	var b = new b;
	b.a();

	print( b );
}
```

```
main() {
	class test {
		b() {
			print("in b\n");
		}
	}


	var a = new test;
	a.b();

	print("test\n");
}
```

```
class test {
	a() {
		print("in a\n");
		return this;
	}

	b() {

		print("in b\n");
		return this;
	}
}

test() {
	var a = new test;
	return a;	
}

main() {
	test().a().b();
}
```

```
class test {
	b() {
		print("in b ... \n");
	}

	a() {
		this.b();
		print( this );
	}
}


main() {
	
	var a = new test;
	a.c = "Test";

	a.a = func() {
		this.b();
		print("here\n");
	}

	a.a();
}
```

```
class test {
	b() {
		print("in b ... \n");
	}

	a() {
		this.b();
		print( this );
	}
}


main() {
	
	var a = new test;
	a.c = "Test";

	a.a();
}
```

```
test() {
	for( var j = 0; j < 10; j++ ) {
		print("j : "+j+" / ");
		for( var s = 0; s < 10; s++ ) {
			print("s:"+s+",");
		}

		print("\n");
	}	
}


main() {


	test();

}
```

```
test() {

	var arr = [0 : ['name' : 'second'], 1 : ['name' : 'test1'] ];

	for( var k,v in arr ) {
		print(k);
		print(" : ");
		print(v);
		print("\n");

	}
}


main() {
	test();
}
```

```
add( a, b ) {
	print( a + b );
}


main() {
	add( 3, 5 );
}
```

```
test() {
	
}

main() {
	test();
}
```

```

main() {
	test();
}

test() {
	print("here");
}

```

```
main() {

	func test( a ) {
		print( a );
	}

	test("str\n");	
}
```

```

class httpResult {
	write( a ) {
		print( a );
	}
}


class httpRequest {
	
}


class httpServer {
	
	listen( port ) {
		print("listen on port"+port+"\n");
	}

	route( a, cb ) {
		print("route for "+a+"\n");

		var res = new httpResult;
		var req = new httpRequest;

		cb( req, res );
	}
}

main() {
	

	var h = new httpServer;

	print(h);

	h.listen(8080);

	h.route('/', func( req, res ) {
		res.write("this is test");
	});
}
```

```
import testClass;
import test1Class;

test() {
	print("in test function\n");
}

main() {
	var a = new test;
	a.a();

	var a = new test1;
	a.a();

	test();


	var n = new n();
	n.a();
}
```

```



main() {
	var a = [];
	a.p = "this is test";

	print( a );
}
```

```
main() {
	
	var o = { a : func( a ) { print(a); } };
	o.a('test');
}
```

```
test() {
	var o = { 'a' : { 'b' : func(a) { print(a); } } };
	return o;
}

main() {
	var b = test();

	b.a.b("test\n");
}
```

```
main() {
	
	var a = "test";
	var b = 123;

	if( a || b ) {
		print("here")
	}
}
```

```
main() {
	print("test");

	var o = { 'a' : '123', 'b' : 321 };

	print( o );
}
```

```

test( this ) {
	this.d = 321;
}

main() {
	var this = {};
	test( this );
	print( this );
}
```

```

```

```
main() {

	var String = {};
	String.prototype = {};

	String.prototype.len = func( str ) {
		return str.length;
	}

	print( "test" );
}
```

```
class test1 {
	b() {
		print("in b\n");
	}
	
	a() {
		this.b();
		print("in a\n");
	}
}


class n {
	construct() {
		print("in construct n\n")
	}

	a() {
		print("in n in a\n");
	}
}
```

```
class test {
	a() {
		print("here\n");
	}
}
```

```

var i;
test_a() {
	for( i = 0; i < 10; i++ ) {
		print(i+"\n");
	}
}

main() {
	test_a();
	print(i);
}
```

```
var test_var;

test_b() {
	test_var = "bbb\n";	
}

test_a() {
	test_var = "aaa\n";
}

main() {
	test_var = "test";

	test_a();
	print(test_var);

	test_var = "test1";
	test_b();
	print(test_var);

	var a = "test";
}
```

```
var a;

main() {
	
	a = "test1";
	print( a );
}
