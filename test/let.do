main() {
	let s = microtime();

	let a = "test\n"
	print( a )


	let a = { [ id : 1, name : 'test'], [ id : 2, name : 'test1'] }
	
	for( let k,v in a ) {
		print(k)
		print(v)
	}

	print( microtime() - s )
}