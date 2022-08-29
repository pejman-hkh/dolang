/*var i;
test() {
	return i--;
}
*/
main() {
/*	i = 10;

	while( var a = test() ) {
		print( a +"\n" );
	}
*/

	let i = []
	i.i = 0
	let o = []
	o.str = '{"test":"123"}'
	
	i.i++

	while( o.str[i.i] != '}' ) {
		print( "str : "+o.str[i.i]+" ###\n" )
		i.i++
	}
}