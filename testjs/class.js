class test {
	construct( a ) {
		print(a+"\n")
	}

	a() {
		print("in a\n")
		return this
	}

	b() {
		print("in b\n");
	}
}

let a = new test('test')
a.a().b()
print("here")
