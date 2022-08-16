main() {

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
}
