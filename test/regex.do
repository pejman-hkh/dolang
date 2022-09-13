main() {

	let regex1 = new RegExp('foo*', 'gmi');
	let str1 = 'table football, foosball';

	print( str1 +"\n\n");
	
	while( let array1 = regex1.exec(str1) ) {
		print("Found "+array1[0]+" Next starts at "+regex1.lastIndex+" ");
	}
}