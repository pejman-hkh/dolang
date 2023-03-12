var a = {};
a.p = "this is test";

print( a );


var o = { a : func( a ) { print(a); } };
o.a('test');


var o = {
	g : 'test\n', 
	a : func() {
		this.b()
	},
	b : func() {
		print( this.g )
		print("in b")
	}
}

print(o);
o.a();