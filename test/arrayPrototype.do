
main() {


	Array.prototype.push = func( a ) {
		this.value[this.length()] = a;
	}

	Array.prototype.toString = func() {
		let ret = "";
		let pre = "";
		for( let k,v in this.value ) {
			ret = ret + pre + v;
			pre = ",";
		}

		return ret;
	}


	let b = [1,2,3,4];
	b.push('test2');
	b.push('test3');

	print(b.toString());
}