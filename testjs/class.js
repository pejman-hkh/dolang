class test {
	construct( a ) {
		console.log(a+"\n")
	}

	a() {
		console.log("in a\n")
		return this
	}

	b() {
		console.log("in b\n");
	}
}

let a = new test('test')
a.a().b()
console.log("here")
