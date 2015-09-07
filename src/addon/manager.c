/*
 * c_config.c
 *
 *  Created on: 2015年8月11日
 *      Author: Stur
 */

#include "manager.h"

void main_init() {
	context = setup_context();
}

void main_free() {
	char data[30];
	sprintf(data, "%d", context->msg_index);
	mark(data);
	free_context(context);
}

void set_domain(char* domain) {
	copy_nstr(context->domain, domain);
}

void set_server(const char* server[], int len) {

	int i;

	if (len > 4)
		len = 4;

	for (i = 0; i < len; i++) {
		strncpy(context->serv->server[i], server[i], strlen(server[i]));
	}

	context->serv->len = len;
}

void set_debug_level(int level) {
	debug_level = level;
}

void next_message_id(char** buffer) {
	char* buf = *buffer;
	char foo[30];
	c_long x, timestamp;
	x = get_tv_usec();

#ifdef _WIN32
	timestamp = x / 3600000000LL;
#else
	timestamp = x / 3600000000L;
#endif

	context->msg_index++;

	strcpy(buf, context->domain);
	strcat(buf, SPLIT);
	strncat(buf, context->local_ip_hex, 8);
	strcat(buf, SPLIT);

#ifdef _WIN32
	sprintf(foo, "%lld", timestamp);
#else
	sprintf(foo, "%ld", timestamp);
#endif

	strcat(buf, foo);
	strcat(buf, SPLIT);

	sprintf(foo, "%d", context->msg_index);
	strcat(buf, foo);
}

void add_message(struct message* msg) {
	char* ptr = small_buf;
	next_message_id(&ptr);
	copy_string(context->msg_id, small_buf, CHAR_BUFFER_SIZE);
	context->msg = msg;
}

