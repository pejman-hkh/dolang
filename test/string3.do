main() {

	let paragraph = 'The quick brown fox jumps over the lazy dog. If the dog barked, was it really lazy?';

	let searchTerm = 'dog';
	let indexOfFirst = paragraph.indexOf(searchTerm);

	print( indexOfFirst );
	print("\n")

	let sentence = 'The quick brown fox jumps over the lazy dog.';

	let index = 4;

	print("The character at index "+index+" is "+sentence.charAt(index)+" ");
	print("\n")

	// expected output: "The character at index 4 is q"


	print( String.fromCharCode(65) );
	print("\n")

	let l = "test".length

	print(l)
	print("\n")

	String.prototype.test = func() {
		print( this )
	}

	let a = new String("test")

	print( a )
	print("\n")

	print( a.length() )
	print("\n")

	print(a.substr(1,2))
	print("\n")
	print(a.charCodeAt(1))
	print("\n")
	print(a.indexOf('e'))
	print("\n")
	a.test()


	let l = "test".length;

	print(l)

}