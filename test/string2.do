class Test {
	a() {
		print("in a\n")
	}
}

main() {

	let t = "test";
	print( t.charCodeAt(2) );

	print("\n")

	let y = {}
	y.o = func() {
		print("y o \n")
	}

	y.o();

	print( String )
	print( Array )
	//print( Object )

	String.prototype.test = func() {
		return 2;
	}

	String.prototype.test1 = func() {
		return "test1";
	}

	let a = "Hello World"

	print( a.substr(1,4)+"\n" )

	print( a.length() )

	print("\n")

	print( a.test() )
	print("\n")
	print( a.test1() )
	print("\n")


	let g = new Test
	Test.prototype.b = func() {
		print("in b\n");
	}

	print( g );

	g.a();
	g.b();
}