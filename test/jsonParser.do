class jsonParser {
	construct( a ) {
		this.str = a;
		this.i = 0;
		this.res = this.parse();
	}

	next() {
		this.i++;
	}

	tok() {
		return this.str[ this.i ];
	}

	parseObject() {

		this.next();
		let ret = [];
		while( this.tok() != '}' ) {


			let k = this.parse();

			this.next();

			let v = this.parse();

			ret[k] = v;

			if( this.tok() == ',' ) {
				this.next();
			}

		}


		return ret;
	}

	parseStr() {

		let a = "";

		this.next();	
		while( this.tok() != '"' ) {
			a = a + this.tok();
			this.next();
		}
		this.next();

		return a;
	}

	parseArray() {

	}

	init() {

	}

	parse() {
		let i = 0;
		let a = this.tok();
		if( a == "{" ) {
			return this.parseObject();
		} else if( a == '"' ) {
			return this.parseStr();
		} else if( a == '[' ) {
			return this.parseArray();
		}
	
	}
}

main() {

	let a = new jsonParser('{"test":"123","test1":"321"}');
	print( a.res );
	print( a.res.test );

}
