

function test1() {
	console.log('in test1\n')
}

test1.prototype.a = function() {
	console.log('in a\n')
}

let a = new test1()
a.a()

test1()

console.log( a )