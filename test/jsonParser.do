class jsonParser {
	construct( a ) {
		this.str = a
		this.i = 0
		this.res = this.parse()

	}

	skipSpace() {

		while( this.tok() == ' ' || this.tok() == "\n" || this.tok() == "\t" ) {
			this.i++
		}	

	}

	next() {
		this.skipSpace()
		this.i++
		this.skipSpace()
	}

	next1() {
		this.i++
	}

	tok() {
		return this.str[ this.i ]
	}

	parseObject() {
		this.next()
		
		let ret = {}
		while( this.tok() != '}' ) {
			let k = this.parse()

			
			this.next()
			
			let v = this.parse()
			//print(k);
			ret[k] = v
			

			if( this.tok() == ',' ) {
				this.next()
				
			}

		}

		this.next()
		
		return ret
	}

	parseStr() {

		let a = ""
		this.next()	
		
		while( this.tok() != '"' ) {
			a = a + this.tok()
			this.next1()
		}
		this.next()
		
		return a
	}

	parseArray() {
		this.next()
		
		let ret = []
		let i = 0
		while( this.tok() != ']' ) {
			let k = i++
			
			let v = this.parse()

			ret[k] = v

			if( this.tok() == ',' ) {
				this.next()
				
			}
		}
		this.next()
		
		return ret
	}

	parse() {
		let a = this.tok()
	
		if( a == "{" ) {
			return this.parseObject()
		} else if( a == '"' ) {
			return this.parseStr()
		} else if( a == '[' ) {
			return this.parseArray()
		}
	
	}
}

main() {

	if ( typeof(console) == 32 ) {
	    var console = { log: func( a ) {
	    	print( a );
	    }};
	}

	let a = new jsonParser('{ "test": "123" , "test1" : "321", "array" : [ "a", "b", "c", "d" ], "inner" : {"aa" : "11", "bb" : "22", "cc" : "33"} }')
	print( a.res )
	print( a.res.test )

}
