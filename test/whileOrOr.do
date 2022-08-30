test() {
	let str = "  
this is test
and test and test";

	let i = 0

	while( str[i] ) {
		//print( str[i] +"\n" )
		//print( i +"\n" )
		if( str[i] == ' ' || str[i] == '\n' ) {
			print('empty\n')
		}
		i++
	}	
}

main() {
	test();
}