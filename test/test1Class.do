class test1 {
	b() {
		print("in b\n");
	}
	
	a() {
		this.b();
		print("in a\n");
	}
}


class n {
	construct() {
		print("in construct n\n")
	}

	a() {
		print("in n in a\n");
	}
}