
class httpResult {
	write( a ) {
		print( a );
	}
}


class httpRequest {
	
}


class httpServer {
	
	listen( port ) {
		print("listen on port"+port+"\n");
	}

	route( a, cb ) {
		print("route for "+a+"\n");

		var res = new httpResult;
		var req = new httpRequest;

		cb( req, res );
	}
}

main() {
	

	var h = new httpServer;

	print(h);

	h.listen(8080);

	h.route('/', func( req, res ) {
		res.write("this is test");
	});
}