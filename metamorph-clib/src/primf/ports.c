#include <stdio.h>
#include "../primf.h"
#include "../cache.h"
#include "../dyntypes.h"
#include "../functions.h"

dyntype_t read_char(dyntype_t port) {
	REQUIRE_SCHEME_PORT(port, 0);
	char c;
	fscanf(c_port, "%c", &c);
	return scheme_new_char(c);
}
dyntype_t peek_char(dyntype_t port) {
	REQUIRE_SCHEME_PORT(port, 0);
	char c = fgetc(c_port);
	ungetc(c, c_port);
	return scheme_new_char(c);
}
dyntype_t read_line(dyntype_t port) {
	REQUIRE_SCHEME_PORT(port, 0);
	char* buf = REQUEST_ARRAY(char, 200);
	fgets(buf, 200, c_port);
	return scheme_new_string(buf);
}
dyntype_t read_string(dyntype_t number, dyntype_t port) {
	REQUIRE_SCHEME_PORT(port, 0);
	REQUIRE_SCHEME_EXACT_INTEGER(number, 0);
	//TODO FIX
	char* buf = REQUEST_ARRAY(char, 80);
	fgets(buf, 200, c_port);
	return scheme_new_string(buf);
}
dyntype_t newline(dyntype_t port) {
	REQUIRE_SCHEME_PORT(port, 0);
	fprintf(c_port,"\n");
	return (dyntype_t) { SCHEME_TYPE_UNSPECIFIED, 0, 0 };
}
dyntype_t write_char(dyntype_t c, dyntype_t port) {
	REQUIRE_SCHEME_PORT(port, 0);
	REQUIRE_SCHEME_CHAR(c, 0);
	fprintf(c_port, "%c", c_c);
	(dyntype_t) {
		SCHEME_TYPE_UNSPECIFIED, 0, 0
	};
}
dyntype_t write_string(dyntype_t string, dyntype_t port) {
	REQUIRE_SCHEME_PORT(port, 0);
	REQUIRE_SCHEME_STRING(string, 0)
	fprintf(c_port, "%s", c_string);
	(dyntype_t) {
		SCHEME_TYPE_UNSPECIFIED, 0, 0
	};
}
dyntype_t write_string_stack() {
	PARAMETER(string) 
	PARAMETER(port)
	REQUIRE_SCHEME_PORT(port, 0);
	REQUIRE_SCHEME_STRING(string, 0)
		fprintf(c_port, "%s", c_string);
	(dyntype_t) {
		SCHEME_TYPE_UNSPECIFIED, 0, 0
	};
}
dyntype_t flush_output_port(dyntype_t port) {
	REQUIRE_SCHEME_PORT(port, 0);
	fflush(c_port);
}

dyntype_t current_input_port() {
	return scheme_literal_port(stdin);
}
dyntype_t current_output_port() {
	return scheme_literal_port(stdout);
}
dyntype_t current_error_port() {
	return scheme_literal_port(stderr);
}