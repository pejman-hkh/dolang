function main() {
	let Base64 = {
	    characters: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" ,

	    encode: function( string )
	    {

	        let characters = this.characters;

	        let result = '';

	        let i = 0;
	        do {
	            let a = string.charCodeAt(i++);
	            let b = string.charCodeAt(i++);
	            let c = string.charCodeAt(i++);


	            
	            a = a ? a : 0;
	            b = b ? b : 0;
	            c = c ? c : 0;

	            let b1 = ( a >> 2 ) & 0x3F;
	            let b2 = ( ( a & 0x3 ) << 4 ) | ( ( b >> 4 ) & 0xF );
	            let b3 = ( ( b & 0xF ) << 2 ) | ( ( c >> 6 ) & 0x3 );
	            let b4 = c & 0x3F;


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
	        let characters = this.characters;
	        let result     = '';

	        let i = 0;
	        do {
	      
	            let b1 = this.characters.indexOf( string.charAt(i++) );
	            let b2 = this.characters.indexOf( string.charAt(i++) );
	            let b3 = this.characters.indexOf( string.charAt(i++) );
	            let b4 = this.characters.indexOf( string.charAt(i++) );
	      
	            let a = ( ( b1 & 0x3F ) << 2 ) | ( ( b2 >> 4 ) & 0x3 );
	            let b = ( ( b2 & 0xF  ) << 4 ) | ( ( b3 >> 2 ) & 0xF );
	            let c = ( ( b3 & 0x3  ) << 6 ) | ( b4 & 0x3F );

	            result = result + (String.fromCharCode(a) + (b?String.fromCharCode(b):'') + (c?String.fromCharCode(c):'') );

	        } while( i < string.length );

	        return result;
	    }
	};

	let e = Base64.encode('test')
	print( e )
	print("\n")
	let d = Base64.decode(e)
	print( d )

}