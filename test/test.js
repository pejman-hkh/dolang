var a = "Test\n"

print(a)

for( var i = 0; i < 10; i++ ) {
	print(i)
}

print("\n")

func test() {
	print("Test\n")
}

test()

class test {
	a() {
		print("in a\n")
	}
}

var d = new test
d.a()