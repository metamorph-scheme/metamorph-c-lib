#include <stdio.h>
#include "../primf.h"
#include "../cache.h"
#include "../dyntypes.h"
#include "../functions.h"

BASE_FUNCTION(read_char) {
	ELLIPSIS
	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0);
		char c;
		fscanf(c_port, "%c", &c);
		PUSH_LITERAL(scheme_new_char(c))
	}
	else {
		current_input_port(0);
		dyntype_t port = POP_LITERAL;
		REQUIRE_SCHEME_PORT(port, 0);
		char c;
		fscanf(c_port, "%c", &c);
		PUSH_LITERAL(scheme_new_char(c))
		release_dyntype(port);
	}
	DESTROY_ELLIPSIS
	return;
}
BASE_FUNCTION(peek_char) {
	ELLIPSIS;
	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0);
		char c = fgetc(c_port);
		ungetc(c, c_port);
		PUSH_LITERAL(scheme_new_char(c))
	}
	else {
		current_input_port(0);
		dyntype_t port = POP_LITERAL;
		REQUIRE_SCHEME_PORT(port, 0);
		char c = fgetc(c_port);
		ungetc(c, c_port);
		PUSH_LITERAL(scheme_new_char(c))
		release_dyntype(port);
	}
	DESTROY_ELLIPSIS
}
BASE_FUNCTION(read_line) {
	ELLIPSIS;
	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0);
		char* buf = REQUEST_ARRAY(char, 200);
		fgets(buf, 200, c_port);
		PUSH_LITERAL(scheme_new_string(buf))
		RELEASE_ARRAY(char, 200, buf);
	}
	else {
		current_input_port(0);
		dyntype_t port = POP_LITERAL;
		REQUIRE_SCHEME_PORT(port, 0);
		char* buf = REQUEST_ARRAY(char, 200);
		fgets(buf, 200, c_port);
		PUSH_LITERAL(scheme_new_string(buf))
		RELEASE_ARRAY(char, 200, buf);
		release_dyntype(port);
	}
	DESTROY_ELLIPSIS
}
BASE_FUNCTION(read_string){
	//TODO FIX
	ELLIPSIS;
	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0);
		char* buf = REQUEST_ARRAY(char, 200);
		fgets(buf, 200, c_port);
		PUSH_LITERAL(scheme_new_string(buf))
		RELEASE_ARRAY(char, 200, buf);
	}
	else {
		current_input_port(0);
		dyntype_t port = POP_LITERAL;
		REQUIRE_SCHEME_PORT(port, 0);
		char* buf = REQUEST_ARRAY(char, 200);
		fgets(buf, 200, c_port);
		PUSH_LITERAL(scheme_new_string(buf))
		release_dyntype(port);
		RELEASE_ARRAY(char, 200, buf);
	}
	DESTROY_ELLIPSIS
}

BASE_FUNCTION(newline) {
	ELLIPSIS

	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0)
		fprintf(c_port, "\n");
	}
	else {
		current_output_port(0);
		dyntype_t port = POP_LITERAL;
		REQUIRE_SCHEME_PORT(port, 0)
		fprintf(c_port, "\n");
		release_dyntype(port);
	}

	PUSH_UNSPECIFIED

	DESTROY_ELLIPSIS
}

BASE_FUNCTION(write_char) {
	PARAMETER(c)
	ELLIPSIS
	REQUIRE_SCHEME_CHAR(c, 0)

	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0)
		fprintf(c_port, "%c", c_c);
	}
	else {
		current_output_port(0);
		dyntype_t port = POP_LITERAL;
		REQUIRE_SCHEME_PORT(port, 0)
		fprintf(c_port, "%c", c_c);
		release_dyntype(port);
	}

	PUSH_UNSPECIFIED

	DESTROY_PARAM(c)
	DESTROY_ELLIPSIS
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
		current_output_port(0);
		dyntype_t port = POP_LITERAL;
		REQUIRE_SCHEME_PORT(port, 0)
		fprintf(c_port, "%s", c_string);
	    release_dyntype(port);
	}
	
	PUSH_UNSPECIFIED

	DESTROY_PARAM(string)
	DESTROY_ELLIPSIS
}
BASE_FUNCTION(flush_output_port) {
	ELLIPSIS
	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0);
		fflush(c_port);
	}
	else {
		current_output_port(0);
		dyntype_t port = POP_LITERAL;
		REQUIRE_SCHEME_PORT(port, 0);
		fflush(c_port);
		release_dyntype(port);
	}
	PUSH_UNSPECIFIED
	DESTROY_ELLIPSIS
}

BASE_FUNCTION(current_input_port){
	PUSH_LITERAL(scheme_literal_port(stdin));
}
BASE_FUNCTION(current_output_port) {
	PUSH_LITERAL (scheme_literal_port(stdout));
}
BASE_FUNCTION(current_error_port) {
	PUSH_LITERAL(scheme_literal_port(stderr));
}