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

BASE_FUNCTION(write_string) {
	PARAMETER(string) 
	ELLIPSIS

	REQUIRE_SCHEME_STRING(string, 0)

	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0)
		fprintf(c_port, "%s", c_string);
	}
	else {
		current_output_port();
		dyntype_t port = POP_LITERAL;
		REQUIRE_SCHEME_PORT(port, 0)
		fprintf(c_port, "%s", c_string);
	    release_dyntype(port);
	}
	
	PUSH_LITERAL((dyntype_t) {SCHEME_TYPE_UNSPECIFIED})

	DESTROY_PARAM(string) 
	DESTROY_ELLIPSIS
}
dyntype_t flush_output_port(dyntype_t port) {
	REQUIRE_SCHEME_PORT(port, 0);
	fflush(c_port);
}

dyntype_t current_input_port() {
	return scheme_literal_port(stdin);
}
dyntype_t current_output_port() {
	PUSH_LITERAL (scheme_literal_port(stdout));
}
dyntype_t current_error_port() {
	return scheme_literal_port(stderr);
}