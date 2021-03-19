#include <stdio.h>
#include "../primf.h"
#include "../cache.h"
#include "../dyntypes.h"
#include "../functions.h"
#include "../lambda.h"

//BASE_FUNCTION(read_char) {
//	ELLIPSIS
//	if (n_ellipsis) {
//		dyntype_t port = ellipsis[0];
//		REQUIRE_SCHEME_PORT(port, 0);
//		char c;
//		fscanf(c_port, "%c", &c);
//		PUSH_LITERAL(scheme_new_char(c))
//	}
//	else {
//		current_input_port(0);
//		dyntype_t port = POP_LITERAL;
//		REQUIRE_SCHEME_PORT(port, 0);
//		char c;
//		fscanf(c_port, "%c", &c);
//		PUSH_LITERAL(scheme_new_char(c))
//		release_dyntype(port);
//	}
//	DESTROY_ELLIPSIS
//	return;
//}
//BASE_FUNCTION(peek_char) {
//	ELLIPSIS;
//	if (n_ellipsis) {
//		dyntype_t port = ellipsis[0];
//		REQUIRE_SCHEME_PORT(port, 0);
//		char c = fgetc(c_port);
//		ungetc(c, c_port);
//		PUSH_LITERAL(scheme_new_char(c))
//	}
//	else {
//		current_input_port(0);
//		dyntype_t port = POP_LITERAL;
//		REQUIRE_SCHEME_PORT(port, 0);
//		char c = fgetc(c_port);
//		ungetc(c, c_port);
//		PUSH_LITERAL(scheme_new_char(c))
//		release_dyntype(port);
//	}
//	DESTROY_ELLIPSIS
//}

dyntype_t i_read_line(dyntype_t* ellipsis, int n_ellipsis) {
	dyntype_t string;
	string.type = SCHEME_TYPE_STRING;
	string.data.string_val = REQUEST(scheme_string_t);
	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0);
		char* buf = REQUEST_ARRAY(char, 200);
		fgets(buf, 200, c_port);
		*string.data.string_val = buf;
	}
	else {
		scheme_port_t port = i_current_input_port();
		char* buf = REQUEST_ARRAY(char, 200);
		fgets(buf, 200, port);
		*string.data.string_val = buf;
	}
	return string;
}

SIGNATUR(read_line) {
	PUSH_LITERAL(i_read_line(current_activation->formal_parameters, current_activation->number_parameters))
	RETURN
}
//BASE_FUNCTION(read_string){
//	//TODO FIX
//	ELLIPSIS;
//	if (n_ellipsis) {
//		dyntype_t port = ellipsis[0];
//		REQUIRE_SCHEME_PORT(port, 0);
//		char* buf = REQUEST_ARRAY(char, 200);
//		fgets(buf, 200, c_port);
//		PUSH_LITERAL(scheme_new_string(buf))
//		RELEASE_ARRAY(char, 200, buf);
//	}
//	else {
//		current_input_port(0);
//		dyntype_t port = POP_LITERAL;
//		REQUIRE_SCHEME_PORT(port, 0);
//		char* buf = REQUEST_ARRAY(char, 200);
//		fgets(buf, 200, c_port);
//		PUSH_LITERAL(scheme_new_string(buf))
//		release_dyntype(port);
//		RELEASE_ARRAY(char, 200, buf);
//	}
//	DESTROY_ELLIPSIS
//}
//
//BASE_FUNCTION(newline) {
//	ELLIPSIS
//
//	if (n_ellipsis) {
//		dyntype_t port = ellipsis[0];
//		REQUIRE_SCHEME_PORT(port, 0)
//		fprintf(c_port, "\n");
//	}
//	else {
//		current_output_port(0);
//		dyntype_t port = POP_LITERAL;
//		REQUIRE_SCHEME_PORT(port, 0)
//		fprintf(c_port, "\n");
//		release_dyntype(port);
//	}
//
//	PUSH_UNSPECIFIED
//
//	DESTROY_ELLIPSIS
//}
//
//BASE_FUNCTION(write_char) {
//	PARAMETER(c)
//	ELLIPSIS
//	REQUIRE_SCHEME_CHAR(c, 0)
//
//	if (n_ellipsis) {
//		dyntype_t port = ellipsis[0];
//		REQUIRE_SCHEME_PORT(port, 0)
//		fprintf(c_port, "%c", c_c);
//	}
//	else {
//		current_output_port(0);
//		dyntype_t port = POP_LITERAL;
//		REQUIRE_SCHEME_PORT(port, 0)
//		fprintf(c_port, "%c", c_c);
//		release_dyntype(port);
//	}
//
//	PUSH_UNSPECIFIED
//
//	DESTROY_PARAM(c)
//	DESTROY_ELLIPSIS
//}

void i_write_string(dyntype_t string, dyntype_t *ellipsis, int n_ellipsis) {
	REQUIRE_SCHEME_STRING(string, 0)

	if (n_ellipsis) {
		dyntype_t port = ellipsis[0];
		REQUIRE_SCHEME_PORT(port, 0)
			fprintf(c_port, "%s", c_string);
	}
	else {
		scheme_port_t port = i_current_output_port();
		fprintf(port, "%s", c_string);
	}
}

SIGNATUR(write_string) {

	i_write_string(BOUND(0, 0), current_activation->formal_parameters + 1, current_activation->number_parameters - 1);
	
	PUSH_UNSPECIFIED
	RETURN
}
//BASE_FUNCTION(flush_output_port) {
//	ELLIPSIS
//	if (n_ellipsis) {
//		dyntype_t port = ellipsis[0];
//		REQUIRE_SCHEME_PORT(port, 0);
//		fflush(c_port);
//	}
//	else {
//		current_output_port(0);
//		dyntype_t port = POP_LITERAL;
//		REQUIRE_SCHEME_PORT(port, 0);
//		fflush(c_port);
//		release_dyntype(port);
//	}
//	PUSH_UNSPECIFIED
//	DESTROY_ELLIPSIS
//}

scheme_port_t i_current_output_port() {
	return (stdout);
}
scheme_port_t i_current_input_port() {
	return (stdin);
}

SIGNATUR(current_input_port){
	PUSH_LITERAL(scheme_literal_port(i_current_input_port));
	RETURN
}
SIGNATUR(current_output_port) {
	PUSH_LITERAL (scheme_literal_port(i_current_output_port()));
	RETURN
}
SIGNATUR(current_error_port) {
	PUSH_LITERAL(scheme_literal_port(stderr));
	RETURN
}