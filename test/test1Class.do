class test1 {
	b() {
		print("in b\n");
	}
	
	a() {
		this.b();
		print("in a\n");
	}
}