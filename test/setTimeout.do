//event driven programming ...
var cbs;
var i;
setTimeout( cb, time ) {
	cbs[i++] = [ cb, time ];
}

main() {
	i = 0;
	cbs = [];

	setTimeout(func() {
		print("here");
	}, 1000 );


	//run cbs after a time ...
	while( 1 ) {

	}
}