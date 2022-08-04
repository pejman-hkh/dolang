main() {

	var String = {};
	String.prototype = {};

	String.prototype.len = func( str ) {
		return str.length;
	}

	print( "test" );
}