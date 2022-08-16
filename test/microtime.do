main() {

	var s = microtime()

	var j = 1000
	while( j-- ) {
		print(j+"#")
	}

	print("\n\n\n\n")

	print( microtime() - s  )
	
}