
main() {
  let str1 = "abc";
  let str2 = "defghijklmnñopqrstuvwxyz";

  let a = "test";
  let b = " test1";
  print( a + b );

  print( a.length );

  let str = "";
  for(let i = 0; i < 30000; i++) {
    str += str1 + str2;
  }
  
  print( str )
  print("\n")
  print( str.length )

}