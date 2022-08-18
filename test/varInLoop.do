var i;
test() {
	return i--;
}

main() {
	i = 10;

	while( var a = test() ) {
		print( a +"\n" );
	}

}