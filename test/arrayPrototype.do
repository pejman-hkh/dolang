
main() {


	Array.prototype.push = func( a ) {
		this[this.length()] = a;
	}

	let b = [1,2,3,4];
	b.push('test2');
	b.push('test3');


	print(b);
}