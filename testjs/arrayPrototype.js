
Array.prototype.push = function( a ) {
	this.value[this.length] = a;
}

Array.prototype.toString = function() {
	let ret = "";
	let pre = "";
	for( let k in this.value ) {
		let v = this.value[k];
		ret = ret + pre + v;
		pre = ",";
	}

	return ret;
}


let b = [1,2,3,4];
b.push('test2');
b.push('test3');

console.log(b.toString());
