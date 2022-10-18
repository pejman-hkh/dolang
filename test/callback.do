each( arr, cb ) {
	for( var k,v in arr ) {
		cb( k, v );
	}
}


main() {
	
	var arr = [0 : {'name' : 'second'}, 1 : {'name' : 'test1'} ];

	each(arr, func( k, v) {
		print( k );
		print( v );
	});
}