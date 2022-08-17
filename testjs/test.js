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

var o = {
	g : 'test\n', 
	a : func() {
		this.b()
	},
	b : func() {
		print( this.g )
		print("in b")
	}
}

print(o);
o.a();