main() {


	String.prototype.test = func() {
		print( this )
	}

	let a = new String("test")

	print( a )

	print( a.length() )

	print(a.substr(1,2))
	print("\n")
	print(a.charCodeAt(1))
	print("\n")
	print(a.indexOf('e'))
	print("\n")
	a.test()


	let l = "test".length();

	print(l)

}