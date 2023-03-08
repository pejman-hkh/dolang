import lib.curl

let c = new curl();
c.setopt( 10002, 'www.example.com' );
let res = c.exec();
print( res );
c.close();

