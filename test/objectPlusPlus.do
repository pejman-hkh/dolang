main() {
	let a = {};
	a.i = 0;
	//a.j = 2;
	let o = [];
	o.d = [ 0 : 1, 1 : 2];

	//print( a.i + a.j );

	
	print(o.d[a.i++])
	print("\n")
	print(o.d[a.i++])
	print("\n")

	let i = 0;

	i++;
	i++;
	print( i++ )
	print("\n")
	print( i++ )

	a.g = "test"
	a.g += "aaa"
	print(a.g)

}