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

console.log( e )
console.log("\n")
var d = Base64.decode(e)
console.log( d )

