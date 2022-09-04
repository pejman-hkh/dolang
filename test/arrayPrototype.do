main() {

	Array.prototype.push = func( a ) {
		
		print( a );
		print( this.length() );

	}

	let a = ['test1', 'test2'];
	//print( a.length() );

	a.push('test3');
}