main() {

	var o = {
		g : 'test', 
		a : func() { 
			this.b()
		},
		b : func() {
			print("in b")
			print( this.g )
		}
	}

	print(o);
	o.a();
}
