let str1 = "abc";
let str2 = "defghijklmnñopqrstuvwxyz";

let a = "test";
let b = " test1";
console.log( a + b );

console.log( a.length );

let str = "";
for(let i = 0; i < 30000; i++) {
  str += str1 + str2;
}

console.log( str )
console.log("\n")
console.log( str.length )
