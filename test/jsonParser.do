class jsonParser {
	construct( a ) {
		this.str = a;
		this.i = 0;
		let res = this.parse();
		print(res);
	}

	print_tok() {
		//print("tok is : "+ (this.str[ this.i ])+"\n\n");
	}

	next() {
		this.i++;
	}

	tok() {
		return this.str[ this.i ];
	}

	parseObject() {
		//print("tok is : "+ (this.str[ this.i ])+"\n\n");

		this.next();
		let ret = [];
		while( this.tok() != '}' ) {


			let k = this.parse();

			print(k+"\n");

			//print( "ddd : "+this.tok()+"\n" )
			this.next();

			let v = this.parse();
			print( v+"\n" );
			ret[k] = v;

			//this.next();
			//this.parse();
			if( this.tok() == ',' ) {
				this.next();
			}

		}
		print( ret )

		return ret;
	}

	parseStr() {

		let ret = "";
		print("in ret1 : "+ret+" \n");
		this.next();	
		while( this.tok() != '"' ) {
			ret += this.tok();
			this.next();
		}
		this.next();

		print("in ret : "+ret+" \n");

		return ret;
	}

	parseArray() {

	}

	init() {

	}

	parse() {
		let i = 0;
		//this.next();
		let a = this.tok();
		//print( a );

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

	new jsonParser('{"test":"123"}');


}
