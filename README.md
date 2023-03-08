**Under Development**


Do lang is simple and tiny jit compiling programming language and it is very similar to js

There are many samples of language in test directory

Do lang now just supports x86 cpu but in the next time it will support another cpu structurs ...


Make it :

`make -B`

Usage :

`./dolang filename.do `


**Base64**
`
var  Base64 = {
    characters: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" ,

    encode: function( string )
    {
        var  characters = this.characters;
        var  result = '';

        var  i = 0;
        do {
            var  a = string.charCodeAt(i++);

            var  b = string.charCodeAt(i++);
            var  c = string.charCodeAt(i++);

            
            a = a ? a : 0;
            b = b ? b : 0;
            c = c ? c : 0;

   
            var  b1 = ( a >> 2 ) & 0x3F;
            var  b2 = ( ( a & 0x3 ) << 4 ) | ( ( b >> 4 ) & 0xF );
            var  b3 = ( ( b & 0xF ) << 2 ) | ( ( c >> 6 ) & 0x3 );
            var  b4 = c & 0x3F;


            if( ! b ) {
                b3 = b4 = 64;
            } else if( ! c ) {
                b4 = 64;
            }

            result = result + this.characters.charAt( b1 ) + this.characters.charAt( b2 ) + this.characters.charAt( b3 ) + this.characters.charAt( b4 );


        } while ( i < string.length );

        return result;
    } ,

    decode: function( string )
    {
        var  characters = this.characters;
        var  result     = '';

        var  i = 0;
        do {
      
            var  b1 = this.characters.indexOf( string.charAt(i++) );
            var  b2 = this.characters.indexOf( string.charAt(i++) );
            var  b3 = this.characters.indexOf( string.charAt(i++) );
            var  b4 = this.characters.indexOf( string.charAt(i++) );
      
            var  a = ( ( b1 & 0x3F ) << 2 ) | ( ( b2 >> 4 ) & 0x3 );
            var  b = ( ( b2 & 0xF  ) << 4 ) | ( ( b3 >> 2 ) & 0xF );
            var  c = ( ( b3 & 0x3  ) << 6 ) | ( b4 & 0x3F );

            result = result + (String.fromCharCode(a) + (b?String.fromCharCode(b):'') + (c?String.fromCharCode(c):'') );

        } while( i < string.length );

        return result;
    }
};
var e = Base64.encode('test')

print( e )
print("\n")
var d = Base64.decode(e)
print( d )
`


**JsonParser**

```
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


let a = new jsonParser('{ "test": "123" , "test1" : "321", "array" : [ "a", "b", "c", "d" ], "inner" : {"aa" : "11", "bb" : "22", "cc" : "33"} }')
print( a.res )
print( a.res.test )


```