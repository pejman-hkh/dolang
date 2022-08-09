import fs

main() {

	f = new fs

	var a = f.open("test.txt").read()

	print( a )
}