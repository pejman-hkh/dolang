
set_extensions() {
	array_set1( &ext, "print", &do_print);
	array_set1( &ext, "typeof", &do_typeof);
	array_set1( &ext, "array_init", &array_init);
	array_set1( &ext, "array_key", &array_key);
	array_set1( &ext, "array_value", &array_value);
	array_set1( &ext, "array_len", &array_len);
	array_set1( &ext, "array_set", &array_set);
	array_set1( &ext, "array_get", &array_get);
	array_set1( &ext, "set_val", &set_val);
	array_set1( &ext, "strcat", &mstrcat);
	array_set1( &ext, "microtime", &do_microtime);
}