// stdafx.cpp : source file that includes just the standard includes
// web_client.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

int find_href(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	for (int i = 0; i < len; i++) {
		if (Recvbuf[i] == 'h' && Recvbuf[i+1] == 'r' && Recvbuf[i+2] == 'e' && Recvbuf[i+3] == 'f') return i;
	}
	return -1;
}

int find_href_amount(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	int count = 0;
	for (int i = 0; i < len; i++) {
		if (Recvbuf[i] == 'h' && Recvbuf[i + 1] == 'r' && Recvbuf[i + 2] == 'e' && Recvbuf[i + 3] == 'f') count++;
	}
	return count;
}

int find_close(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	for (int i = 0; i < len; i++) {
		if (Recvbuf[i] == '>') return i;
	}
	return -1;
}

int last_slash(char argv1[]) {
	int len = strlen(argv1);
	int pos = 0;
	for (int i = 0; i < len; i++) {
		if (argv1[i] == '/') pos = i;
	}
	return pos;
}

int last_dot(char argv1[]) {
	int len = strlen(argv1);
	int pos = 0;
	for (int i = 0; i < len; i++) {
		if (argv1[i] == '.') pos = i;
	}
	return pos;
}

int count_dot(char argv1[]) {
	int pos = first_slash(argv1);
	int len = strlen(argv1);
	int count = 0;

	if (pos == 0) pos = len;
	for (int i = 0; i < pos; i++) {
		if (argv1[i] == '.') count++;
	}
	return count;
}

int first_slash(char argv1[]) {
	int len = strlen(argv1);
	for (int i = 0; i < len; i++) {
		if (argv1[i] == '/') return i;
	}
	return 0;
}

void form(char argv1[]) {
	char htt[] = "http://";
	int size = strlen(htt);
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (argv1[i] == htt[i]) count++;
	}
	if (count == size) {
		for (int i = 0; i < size; i++) {
			int len = strlen(argv1);
			for (int j = 0; j < len - 1; j++) {
				argv1[j] = argv1[j + 1];
			}
			argv1[len-1] = '\0';
		}
	}
	int len = strlen(argv1);
	if (argv1[len - 1] == '/') argv1[len - 1] = '\0';
}

char* getHost(char argv1[]) {
	int pos = first_slash(argv1);
	int len = strlen(argv1);
	if (pos == 0) pos = len;
	char* host = (char*)malloc(50 * sizeof(char));
	int k = 0;
	
	if (host != NULL) {
		if (count_dot(argv1) < 2) {
			strcpy(host, "www.");
			k = 4;
		}
		for (int i = 0; i < pos; i++) host[i + k] = argv1[i];
		host[pos + k] = '\0';
	}
	return host;
}

char* getDomain(char argv1[]) {
	int pos = first_slash(argv1);
	int len = strlen(argv1);
	if (pos == 0) pos = len;
	char* dom = (char*)malloc(50 * sizeof(char));

	if (dom != NULL) {
		for (int i = 0; i < pos; i++) dom[i] = argv1[i];
		dom[pos] = '\0';
	}
	return dom;
}

char* getRequest(char argv1[]) {
	int pos = first_slash(argv1);
	if (pos == 0) return "";

	int len = strlen(argv1);
	char* req = (char*)malloc(100 * sizeof(char));
	int k = 0;

	if (req != NULL) {
		for (int i = pos + 1; i < len; i++, k++) req[k] = argv1[i];
		req[k] = '\0';
	}

	return req;
}

char* getFile(char argv1[]) {
	int pos = last_slash(argv1);
	if (pos == 0) return "index.html";

	int len = strlen(argv1);
	char* file = (char*)malloc(30 * sizeof(char));
	int k = 0;

	if (file != NULL) {
		for (int i = pos + 1; i < len; i++, k++) file[k] = argv1[i];
		file[k] = '\0';
	}
	return file;
}

char* getSendbuf(char argv1[]) {
	const char* get = "GET /";
	const char* slash = getRequest(argv1);
	const char* req = " HTTP/1.1\r\nHost: ";
	const char* host = getHost(argv1);
	const char* con = " \r\nConnection: close\r\n\r\n";

	int len = strlen(get) + strlen(slash) + strlen(req) + strlen(host) + strlen(con);
	char* des = (char*)malloc(len + 1);
	if (des != NULL) {
		strcpy(des, get);
		strcat(des, slash); strcat(des, req); strcat(des, host); strcat(des, con);
	}
	return des;
}

bool is_folder(char argv1[]) {
	const char* chunk = "Chunked";
	if ( (last_slash(argv1) > last_dot(argv1)) || (getFile(argv1) == chunk) ) return true;
	return false;
}

int find_doc_html(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	for (int i = 0; i < len; i++) {
		if (Recvbuf[len] == '<' && Recvbuf[len + 1] == '!') return i;
	}
}

int find_close_html(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	char c_html[] = "</html>";
	int size = strlen(c_html);
	int count = 0, k = 0;

	for (int i = 0; i < len; i++) {
		for (int j = i; j < i + size; j++, k++) {
			if (Recvbuf[j] == c_html[k]) count++;
		}
		if (count == size) return i;
		else {
			count = 0;
			k = 0;
		}
	}
}

int checkConLen(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	char ct[] = "Content-Length";
	int ct_len = strlen(ct);
	int count = 0, k = 0;
	int ConLen = 0;

	for (int i = 0; i < len; i++) {
		for (int j = i; j < i + ct_len; j++) {
			if (Recvbuf[j] == ct[k]) count++; k++;
		}
		if (count == ct_len) {
			int bytes, c = 0;
			for (int j = i + count + 2; Recvbuf[j] != '\r'; j++) bytes = j - i - count - 2;
			for (int j = i + count + 2; j <= i + count + 2 + bytes; j++) {
				ConLen += (Recvbuf[j] - 48) * pow(10, bytes - c);
				c++;
			}
			return ConLen;
		}
		else {
			count = 0;
			k = 0;
		}
	}
	return 0;
}

bool checkTEc(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	char ct[] = "Transfer-Encoding: chunked";
	int TEc_len = strlen(ct);
	int count = 0, k = 0;

	for (int i = 0; i < len; i++) {
		for (int j = i; j < i + TEc_len; j++) {
			if (Recvbuf[j] == ct[k]) count++; k++;
		}
		if (count == TEc_len) return true;
		else {
			count = 0;
			k = 0;
		}
	}
	return false;
}

int headerSz(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	for (int i = 0; i < len; i++) {
		if (Recvbuf[i] == '\r' && Recvbuf[i + 1] == '\n' && Recvbuf[i + 2] == '\r' && Recvbuf[i + 3] == '\n') return i + 4;
	}
}

int findHeadByte(char Recvbuf[]) {
	int i = 0;
	for (i ; i < strlen(Recvbuf); i++) {
		if (Recvbuf[i] == '\r' && Recvbuf[i + 1] == '\n') return i + 2;
	}
}

int findChunkByte(char Recvbuf[]) {
	int HeadLen = findHeadByte(Recvbuf) - 2;
	int Size = 0;
	for (int j = 0; j < HeadLen; j++) {
		if (Recvbuf[j] == 'a') Size += 10 * pow(16, HeadLen - 1 - j);
		else if (Recvbuf[j] == 'b') Size += 11 * pow(16, HeadLen - 1 - j);
		else if (Recvbuf[j] == 'c') Size += 12 * pow(16, HeadLen - 1 - j);
		else if (Recvbuf[j] == 'd') Size += 13 * pow(16, HeadLen - 1 - j);
		else if (Recvbuf[j] == 'e') Size += 14 * pow(16, HeadLen - 1 - j);
		else if (Recvbuf[j] == 'f') Size += 15 * pow(16, HeadLen - 1 - j);
		else Size += ( Recvbuf[j] - 48 ) * (int)pow(16, HeadLen - 1 - j);
	}
	return Size;
}

int conLenDown(char Recvbuf[], int Buflen, SOCKET ConSock, int ConLen, int hSz) {
	int bytes = 0;
	int Byterecv;
	ofstream Of;
	Of.open("1.html", ios::binary | ios::app);
	Of.write(Recvbuf + hSz, Buflen - hSz);
	bytes += Buflen - hSz;

	do {
		if (hSz != 0) hSz = 0;

		memset(Recvbuf, 0, Buflen);
		Byterecv = recv(ConSock, Recvbuf, Buflen, 0);
		if (Byterecv == 0) break;

		Of.write(Recvbuf, Byterecv);
		bytes += Byterecv;
	} while (bytes < ConLen);
	return ConLen;
}

int chunkDown(char Recvbuf[], int Buflen, SOCKET ConSock, int Byterecv, int hSz, char argv1[]) {
	int chunkContentByte = 0, bytes = 0, chunkByte = 0, TotalByte = 0, headByte;
	bool enoughChunkByte = 0, keep = 1;
	int k = Byterecv;
	
	int len = strlen(getHost(argv1)) + strlen(getRequest(argv1));
	char* name = (char*)malloc(len + 2);
	if (name != NULL) {
		strcpy(name, getDomain(argv1));
		strcat(name, "_");
		strcat(name, getFile(argv1));
	}

	ofstream Of;
	Of.open(name, ios::binary | ios::app);

	do {
		do {
			if (chunkContentByte == chunkByte) {
				chunkContentByte = 0;
				TotalByte += chunkByte;
				if (enoughChunkByte) {
					bytes += 2;
					Byterecv -= 2;
					enoughChunkByte = 0;
				}

				headByte = findHeadByte(Recvbuf + hSz + bytes);
				chunkByte = findChunkByte(Recvbuf + hSz + bytes);
				if (chunkByte == 0) {
					keep = 0;
					break;
				}
				Byterecv -= headByte;
			}

			if (chunkContentByte + Byterecv < chunkByte) {
				bytes += headByte;
				Of.write(Recvbuf + hSz + bytes, Byterecv);
				chunkContentByte += Byterecv;
				bytes += Byterecv;
			}
			else if (chunkContentByte + Byterecv == chunkByte) {
				bytes += headByte;
				Of.write(Recvbuf + hSz + bytes, Byterecv);
				chunkContentByte = chunkByte;
				bytes += Byterecv;
				enoughChunkByte = 1;
			}
			else if (chunkContentByte + Byterecv > chunkByte) {
				bytes += headByte;
				Of.write(Recvbuf + hSz + bytes, chunkByte - chunkContentByte);
				bytes += chunkByte - chunkContentByte + 2;
				Byterecv = Byterecv - (chunkByte - chunkContentByte) - 2;
				chunkContentByte = chunkByte;
			}
			
		} while (bytes < k);


		bytes = 0;
		headByte = 0;
		if (hSz != 0) hSz = 0;
		memset(Recvbuf, 0, Buflen);
		Byterecv = recv(ConSock, Recvbuf, Buflen, 0);
		k = Byterecv;
		

	} while (keep);
	Of.close();
	
	return TotalByte;
}

int downFile(char argv1[], char Recvbuf[], int Buflen, SOCKET ConSock) {
	int Byterecv, hSz, fileSz;

	memset(Recvbuf, 0, Buflen);
	Byterecv = recv(ConSock, Recvbuf, Buflen, 0);

	hSz = headerSz(Recvbuf);
	Byterecv -= hSz;
	if (checkConLen(Recvbuf)) fileSz = conLenDown(Recvbuf, Buflen, ConSock, checkConLen(Recvbuf), hSz);
	else if (checkTEc(Recvbuf)) fileSz = chunkDown(Recvbuf, Buflen, ConSock, Byterecv, hSz, argv1);
	return fileSz;
}
