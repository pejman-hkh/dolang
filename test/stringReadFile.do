import lib.fs

main() {

	var f = new fs("test.txt")

	print( f.read() )
}