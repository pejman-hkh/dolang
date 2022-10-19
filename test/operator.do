main() {
	
	var a = 0
	var b = 0
	var c = 1

	if( a || b || c ) {
		print("in if\n")
	}

	print("after || \n")


	var a1 = 1
	var a2 = 1

	if( a1 && a2 ) {
		print("test1 &&\n")
	}

	a1 = 1
	a2 = 0

	if( a1 && a2 ) {
		print("test2 &&\n")
	}

	print("after && \n")


	var s = 0

	if( !s ) {
		print("s is zero ")
	}

}