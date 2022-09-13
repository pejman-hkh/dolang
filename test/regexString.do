main() {

	String.prototype.replace = func( a, b ) {
		if( typeof( a ) == 1 ) {
			let r = new RegExp(a);
			while( let m = r.exec(this.value) ) {
				print(this.value.substr(0,r.index)+"\n\n" );
				print(this.value.substr(0,r.lastIndex)+"\n\n" );
			}

		}
		return a;
	}

	let p = 'The quick brown fox jumps over the lazy dog. If the dog reacted, was it really lazy?';
	print(p.replace('dog', 'monkey'));

}