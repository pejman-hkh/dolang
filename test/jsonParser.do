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
		this.i++
	}

	tok() {
		return this.str[ this.i ]
	}

	parseObject() {
		this.next()
		this.skipSpace()
		let ret = {}
		while( this.tok() != '}' ) {
			let k = this.parse()

			this.skipSpace()
			this.next()
			this.skipSpace()
			let v = this.parse()
			//print(k);
			ret[k] = v
			this.skipSpace()

			if( this.tok() == ',' ) {
				this.next()
				this.skipSpace()
			}

		}

		this.next()
		this.skipSpace()
		return ret
	}

	parseStr() {

		let a = ""
		this.next()	
		this.skipSpace()
		while( this.tok() != '"' ) {
			a = a + this.tok()
			this.next()
		}
		this.next()
		this.skipSpace()
		return a
	}

	parseArray() {
		this.next()
		this.skipSpace()
		let ret = []
		let i = 0
		while( this.tok() != ']' ) {
			let k = i++
			
			let v = this.parse()

			ret[k] = v

			if( this.tok() == ',' ) {
				this.next()
				this.skipSpace()
			}
		}
		this.next()
		this.skipSpace()
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

	let a = new jsonParser('{ "test": "123" , "test1" : "321", "array" : [ "a", "b", "c", "d" ], "inner" : {"aa" : "11", "bb" : "22", "cc" : "33"} }')
	print( a.res )
	print( a.res.test )


}
