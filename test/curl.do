class curl {

	construct() {
		print("test");
		this.init();

	}

	init() {

		this.curl = curl_init();
		
		return this;
	}

	setopt( opt, optVal ) {
		curl_setopt( this.curl, opt, optVal );
		return this;
	}

	perform() {


	}


	exec() {
		var ret = curl_exec( this.curl );
		return ret;
	}

	close() {
		curl_close( this.curl );
	}
}