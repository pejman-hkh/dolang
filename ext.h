#include "fs.h"

set_extensions() {
	array_set1( &ext, "print", &do_print);
	array_set1( &ext, "typeof", &do_typeof);
	array_set1( &ext, "array_init", &array_init);
	array_set1( &ext, "array_key", &array_key);
	array_set1( &ext, "array_value", &array_value);
	array_set1( &ext, "array_len", &array_len);
	array_set1( &ext, "array_set", &array_set);
	array_set1( &ext, "array_get", &array_get);
	array_set1( &ext, "array_set_val", &array_set_val);
	array_set1( &ext, "strcat", &mstrcat);
	array_set1( &ext, "microtime", &do_microtime);
	array_set1( &ext, "fopen", &do_fopen);
	array_set1( &ext, "feof", &do_feof);
	array_set1( &ext, "fgetc", &do_fgetc);
}