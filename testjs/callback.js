function each( arr, cb ) {
	for( let k in arr ) {
		let v = arr[k];
		cb( k, v );
	}
}


let arr = [{'name' : 'second'}, {'name' : 'test1'} ];

each(arr, function( k, v) {
	console.log( k );
	console.log( v );
});
