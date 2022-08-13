import lib.curl

main() {
	var c = new curl();
	c.setopt( 10002, 'www.example.com' );
	var res = c.exec();
	print( res );
	c.close();

}