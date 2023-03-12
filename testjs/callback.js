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



var s = "test s";
var l = function( a ) {

	console.log("in callback\n");
	console.log( a );
};

l("test\n");

var add = function( a , b ) {
	return a+b;
};


console.log( "sum of 2 and 4 is : "+add( 2, 4)+"\n" );