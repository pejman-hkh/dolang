function each( arr, cb ) {
	for( let k,v in arr ) {
		cb( k, v );
	}
}


let arr = [{'name' : 'second'}, {'name' : 'test1'} ];

each(arr, function( k, v) {
	console.log( k );
	console.log( v );
});
