// stdafx.cpp : source file that includes just the standard includes
// web_client.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

bool is_fold = false;
char** Argv;
char* link;
int TotalByte = 0;
int link_pos = 1;

void fill_argv(int argc, char** argv) {
	Argv = (char**)malloc(argc * sizeof(char*));
	if (Argv != NULL) {
		for (int i = 0; i < argc; i++) Argv[i] = argv[i];
	}
}

void get_link() {
	link = Argv[link_pos];
	link_pos++;
}

SOCKET createSocket() {
	addrinfo hints, * ptr = NULL, * result = NULL;
	SOCKET ConnectSocket = INVALID_SOCKET;
	int iResult;


	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	// Resolve the server address and port
	iResult = getaddrinfo(getDomain(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return NULL;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return NULL;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return NULL;
	}
	return ConnectSocket;
}

int find_href(char* Recvbuf) {
	int len = strlen(Recvbuf);
	for (int i = 0; i < len; i++) {
		if (Recvbuf[i] == 'h' && Recvbuf[i+1] == 'r' && Recvbuf[i+2] == 'e' && Recvbuf[i+3] == 'f' && Recvbuf[i+6] != '/' && Recvbuf[i+6] != '?') return i;
	}
	return -1;
}

int find_href_amount(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	int count = 0;
	for (int i = 0; i < len; i++) {
		if (Recvbuf[i] == 'h' && Recvbuf[i + 1] == 'r' && Recvbuf[i + 2] == 'e' && Recvbuf[i + 3] == 'f' && Recvbuf[i + 6] != '/' && Recvbuf[i + 6] != '?') count++;
	}
	return count;
}

int find_close(char* Recvbuf) {
	int len = strlen(Recvbuf);
	for (int i = 0; i < len; i++) {
		if (Recvbuf[i] == '>') return i;
	}
	return -1;
}

int last_slash() {
	int len = strlen(link);
	int pos = 0;
	for (int i = 0; i < len; i++) {
		if (link[i] == '/') pos = i;
	}
	return pos;
}

int second_last_slash() {
	int last = last_slash();
	int len = strlen(link);
	int pos = 0;
	for (int i = 0; i < len; i++) {
		if (link[i] == '/' && i != last) pos = i;
	}
	return pos;
}

int last_dot() {
	int len = strlen(link);
	int pos = 0;
	for (int i = 0; i < len; i++) {
		if (link[i] == '.') pos = i;
	}
	return pos;
}

int first_slash() {
	int len = strlen(link);
	for (int i = 0; i < len; i++) {
		if (link[i] == '/') return i;
	}
	return 0;
}

int count_dot() {
	int pos = first_slash();
	int len = strlen(link);
	int count = 0;

	if (pos == 0) pos = len;
	for (int i = 0; i < pos; i++) {
		if (link[i] == '.') count++;
	}
	return count;
}

int find_equal() {
	int len = strlen(link);
	for (int i = 0; i < len; i++) {
		if (link[i] == '=') return i;
	}
	return 0;
}

void form() {
	char htt[] = "http://";
	int size = strlen(htt);
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (link[i] == htt[i]) count++;
	}
	if (count == size) {
		for (int i = 0; i < size; i++) {
			int len = strlen(link);
			for (int j = 0; j < len - 1; j++) {
				link[j] = link[j + 1];
			}
			link[len-1] = '\0';
		}
	}
	int len = strlen(link);
	if (second_last_slash() < last_dot() && last_slash() == len - 1) link[len - 1] = '\0';
}

char* getFile() {
	int pos = last_slash();
	if (pos == 0) return "index.html";


	int len = strlen(link);
	char* file = (char*)malloc(30 * sizeof(char));
	int k = 0;

	if (file != NULL) {
		if (pos != len - 1) {
			for (int i = pos + 1; i < len; i++, k++) file[k] = link[i];
			file[k] = '\0';
		}
		else if (pos = len - 1) {
			pos = second_last_slash();
			for (int i = pos + 1; i < len - 1; i++, k++) file[k] = link[i];
			file[k] = '\0';
		}
	}
	return file;
}

bool is_folder() {
	const char* chunk = "Chunked";
	int count = 0;
	char* file = getFile();
	if (second_last_slash() > last_dot()) {
		for (int i = 0; i < 7; i++) {
			if (file[i] == chunk[i]) count++;
		}
		if (count < 7) is_fold = true;
	}
	return is_fold;
}

bool is_Chunked() {
	const char* chunk = "Chunked";
	int count = 0;
	char* file = getFile();
	if (second_last_slash() > last_dot() || last_slash() > last_dot()) {
		for (int i = 0; i < 7; i++) {
			if (file[i] == chunk[i]) count++;
		}
		if (count == 7) return true;
	}
	return false;
}

char* getHost() {
	int pos = first_slash();
	int len = strlen(link);
	if (pos == 0) pos = len;
	char* host = (char*)malloc(50 * sizeof(char));
	int k = 0;
	
	if (host != NULL) {
		if (count_dot() < 2) {
			strcpy(host, "www.");
			k = 4;
			for (int i = 0; i < pos; i++) host[i + k] = link[i];
			host[pos + k] = '\0';
		}
		for (int i = 0; i < pos; i++) host[i + k] = link[i];
		host[pos + k] = '\0';
	}
	return host;
}

char* getDomain() {
	int pos = first_slash();
	int len = strlen(link);
	if (pos == 0) pos = len;
	char* dom = (char*)malloc(50 * sizeof(char));

	if (dom != NULL) {
		for (int i = 0; i < pos; i++) dom[i] = link[i];
		dom[pos] = '\0';
	}
	return dom;
}

char* getRequest() {
	int pos = first_slash();
	if (pos == 0) return "";

	int len = strlen(link);
	char* req = (char*)malloc(100 * sizeof(char));
	int k = 0;

	if (req != NULL) {
		for (int i = pos + 1; i < len; i++, k++) req[k] = link[i];
		req[k] = '\0';
	}

	return req;
}

char* getFolder() {
	int pos_1 = second_last_slash();
	int pos_2 = last_slash();
	char* fold = (char*)malloc(pos_2 - pos_1);

	int k = 0;
	if (fold != NULL) {
		for (int i = pos_1 + 1; i < pos_2; i++, k++) fold[k] = link[i];
		fold[k] = '\0';
	}
	return fold;
}

char* getSendbuf() {
	const char* get = "GET /";
	char* slash = getRequest();
	const char* req = " HTTP/1.1\r\nHost: ";
	char* host = getHost();
	const char* con = " \r\nConnection: close\r\n\r\n";

	int len = strlen(get) + strlen(slash) + strlen(req) + strlen(host) + strlen(con);
	char* des = (char*)malloc(len + 1);
	if (des != NULL) {
		strcpy(des, get);
		strcat(des, slash); strcat(des, req); strcat(des, host); strcat(des, con);
	}
	return des;
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

int checkTEc(char Recvbuf[]) {
	int len = strlen(Recvbuf);
	char ct[] = "Transfer-Encoding: chunked";
	int TEc_len = strlen(ct);
	int count = 0, k = 0;

	for (int i = 0; i < len; i++) {
		for (int j = i; j < i + TEc_len; j++) {
			if (Recvbuf[j] == ct[k]) count++; k++;
		}
		if (count == TEc_len) return 1;
		else {
			count = 0;
			k = 0;
		}
	}
	return 0;
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

int conLenDown(char Recvbuf[], SOCKET ConSock, int Byterecv, int hSz, int ConLen) {
	int Buflen = DEFAULT_BUFLEN;
	int bytes = 0;
	char* name;
	ofstream Of;


	if (is_fold) {
		char* dom = getDomain();
		char* fold = getFolder();
		int len = strlen(getFile()) + strlen(dom) + strlen(fold);
		name = (char*)malloc(len + 3);
		if (name != NULL) {
			strcpy(name, dom);
			strcat(name, "_");
			strcat(name, fold);
			strcat(name, "/");
			strcat(name, getFile());
		}
	}
	else {
		
		int len = strlen(getHost()) + strlen(getRequest()) + 5;
		name = (char*)malloc(len + 2);
		if (name != NULL) {
			strcpy(name, getDomain());
			strcat(name, "_");
			strcat(name, getFile());
		}
	}

	Of.open(name, ios::binary | ios::app);
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
	return bytes;
}

int chunkDown(char Recvbuf[], SOCKET ConSock, int Byterecv, int hSz) {
	int Buflen = DEFAULT_BUFLEN;
	int chunkContentByte = 0, bytes = 0, chunkByte = 0, TotalByte = 0, headByte;
	bool enoughChunkByte = 0, keep = 1;
	int k = Byterecv;
	char* name;
	ofstream Of;

	
	if (is_fold) {
		char* dom = getDomain();
		char* fold = getFolder();
		int len = strlen(getFile()) + strlen(dom) + strlen(fold);
		name = (char*)malloc(len + 3);
		if (name != NULL) {
			strcpy(name, dom);
			strcat(name, "_");
			strcat(name, fold);
			strcat(name, "/");
			strcat(name, getFile());
		}
	}
	else {
		if (is_Chunked()) {
			int len = strlen(getHost()) + strlen(getRequest()) + 5;
			name = (char*)malloc(len + 2);
			if (name != NULL) {
				strcpy(name, getDomain());
				strcat(name, "_");
				strcat(name, getFile());
				strcat(name, ".html");
			}
		}
		else {
			int len = strlen(getHost()) + strlen(getRequest()) + 5;
			name = (char*)malloc(len + 2);
			if (name != NULL) {
				strcpy(name, getDomain());
				strcat(name, "_");
				strcat(name, getFile());
			}
		}
	}

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

int downFile(SOCKET ConSock) {
	int fileSz = 0;
	int Byterecv, hSz, Buflen = DEFAULT_BUFLEN;
	char Recvbuf[DEFAULT_BUFLEN];

	memset(Recvbuf, 0, Buflen);
	Byterecv = recv(ConSock, Recvbuf, Buflen, 0);
	hSz = headerSz(Recvbuf);
	Byterecv -= hSz;

	if (checkConLen(Recvbuf)) fileSz = conLenDown(Recvbuf, ConSock, Byterecv, hSz, checkConLen(Recvbuf));
	else if (checkTEc(Recvbuf)) fileSz = chunkDown(Recvbuf, ConSock, Byterecv, hSz);

	return fileSz;
}

DWORD WINAPI threadFunc(LPVOID lpParam) {
	SOCKET ConSock = *(SOCKET*) lpParam;

	char* Sendbuf = getSendbuf();
	send(ConSock, Sendbuf, (int)strlen(Sendbuf), 0);
	int fileByte = downFile(ConSock);
	cout << fileByte << "  ";
	
	TotalByte += fileByte;

	return 0;
}

int downFolder(SOCKET ConSock) {
	int Byterecv, close_pos, temp = 0, href_amount = 0;
	bool seperate = false;
	link = (char*)malloc(100);
	int argv1_len;
	if (link != NULL) {
		strcpy(link, Argv[1]);
		argv1_len = strlen(link);
	}
	int Buflen = DEFAULT_BUFLEN;
	char Recvbuf[DEFAULT_BUFLEN];


	char* dom = getDomain();
	char* fold = getFolder();
	int len = strlen(dom) + strlen(fold);
	char* name = (char*)malloc(len + 2);
	if (name != NULL) {
		strcpy(name, dom);
		strcat(name, "_");
		strcat(name, fold);
	}
	mkdir(name);


	char* sendbuf = getSendbuf();
	send(ConSock, sendbuf, (int)strlen(sendbuf), 0);


	int k = 0;
	len = 0;

	HANDLE hThread;
	DWORD threadID;

	char* Recvbuf_temp, *recvb;;
	
	do {
		memset(Recvbuf, 0, 2000);
		Byterecv = recv(ConSock, Recvbuf, 1999, 0);
		Recvbuf[1999] = '\0';
		if (Byterecv == 0) break;
		len += strlen(Recvbuf);

		if (k == 0) {
			Recvbuf_temp = (char*)malloc(len + 1);
			strcpy(Recvbuf_temp, Recvbuf);
		}
		else {
			recvb = (char*)malloc(len + 1);
			strcpy(recvb, Recvbuf_temp);
			strcat(recvb, Recvbuf);
			Recvbuf_temp = (char*)malloc(len + 1);
			strcpy(Recvbuf_temp, recvb);
		}

		k++;
	} while (Byterecv > 0);

	k = 0;

	
	href_amount = find_href_amount(Recvbuf_temp);
	HANDLE* Array_Of_Thread_Handles = (HANDLE*)malloc(href_amount * sizeof(HANDLE));


	cout << "File's size: ";

	if (find_href(Recvbuf_temp) >= 0) {
		
		int pos = find_href(Recvbuf_temp);
		do {
			int pos_close = find_close(Recvbuf_temp + pos + 6);

			strncat(link, Recvbuf_temp + pos + 6, pos_close - 1);

			SOCKET ConSock_temp = createSocket();
			hThread = CreateThread(NULL, 0, threadFunc, &ConSock_temp, 0, &threadID);
			Array_Of_Thread_Handles[k] = hThread;
			k++;
			Sleep(500);
				
			link[argv1_len] = '\0';
			pos += find_href(Recvbuf_temp + pos + 1) + 1;
			
			temp++;
		} while (temp < href_amount);
		temp = 0;
	}
	
	
	WaitForMultipleObjects(href_amount, Array_Of_Thread_Handles, TRUE, INFINITE);

	return TotalByte;
}