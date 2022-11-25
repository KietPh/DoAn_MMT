// stdafx.cpp : source file that includes just the standard includes
// web_client.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


int last_slash(char str[]) {
	int len = strlen(str);
	int pos = 0;
	for (int i = 0; i < len; i++) {
		if (str[i] == '/') pos = i;
	}
	return pos;
}

int last_dot(char str[]) {
	int len = strlen(str);
	int pos = 0;
	for (int i = 0; i < len; i++) {
		if (str[i] == '.') pos = i;
	}
	return pos;
}

void form(char str[]) {
	char htt[] = "https://";
	int size = strlen(htt);
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (str[i] == htt[i]) count++;
	}
	if (count == size) {
		for (int i = 0; i < size; i++) {
			int len = strlen(str);
			for (int j = 0; j < len - 1; j++) {
				str[j] = str[j + 1];
			}
			str[len-1] = '\0';
		}
	}
	int len = strlen(str);
	if (str[len - 1] == '/') str[len - 1] = '\0';
}

bool is_webpage(char str[]) {
	if (last_slash(str) == 0) return true;
	return false;
}

bool is_file(char str[]) {
	if (last_dot(str) > last_slash(str)) return true;
	return false;
}

bool is_folder(char str[]) {
	if (last_slash(str) > last_dot(str)) return true;
	return false;
}

int find_doc_html(char str[]) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[len] == '<' && str[len + 1] == '!') return i;
	}
}

int find_close_html(char str[]) {
	int len = strlen(str);
	char c_html[] = "</html>";
	int size = strlen(c_html);
	int count = 0, k = 0;

	for (int i = 0; i < len; i++) {
		for (int j = i; j < i + size; j++, k++) {
			if (str[j] == c_html[k]) count++;
		}
		if (count == size) return i;
		else {
			count = 0;
			k = 0;
		}
	}
}

bool checkConLen(char str[]) {
	int len = strlen(str);
	char ct[] = "Content-Length";
	int ct_len = strlen(ct);
	int count = 0, k = 0;
	for (int i = 0; i < len; i++) {
		for (int j = i; j < i + ct_len; j++, k++) {
			if (str[j] == ct[k]) count++;
		}
		if (count == ct_len) return true;
		else {
			count = 0;
			k = 0;
		}
	}
	return false;
}

bool checkTEc(char str[]) {
	int len = strlen(str);
	char ct[] = "Transfer-Encoding: chunked";
	int TEc_len = strlen(ct);
	int count = 0, k = 0;
	for (int i = 0; i < len; i++) {
		for (int j = i; j < i + TEc_len; j++, k++) {
			if (str[j] == ct[k]) count++;
		}
		if (count == TEc_len) return true;
		else {
			count = 0;
			k = 0;
		}
	}
	return false;
}

int headerSz(char str[]) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[i] == '\r' && str[i + 1] == '\n' && str[i + 2] == '\r' && str[i + 3] == '\n') return i + 4;
	}
}

void eraseHeader(char str[]) {
	int len = strlen(str);
	int times = headerSz(str);
	for (int i = 0; i < times; i++) {
		for (int j = 0; j < len - 1; j++) {
			str[j] = str[j + 1];
		}
		len--;
	}
	str[len] = '\0';
}