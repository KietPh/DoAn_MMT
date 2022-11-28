#pragma once

#include <iostream>
#include <fstream>
#include <direct.h>
#include <ws2tcpip.h>
using namespace std;


/*    Tim vi tri 'href' dau tien   */
int find_href(char Recvbuf[]);
/*    Tim so luong 'href      */
int find_href_amount(char Recvbuf[]);
/*    Tim dau '>'   */
int find_close(char Recvbuf[]);
/*    Tim dau '/' o cuoi nhat   */
int last_slash(char argv1[]);
/*    Tim dau '.' o cuoi nhat   */
int last_dot(char argv1[]);
/*    Dem so dau '.' trong domain   */
int count_dot(char argv1[]);
/*    Tim dau '/' dau tien    */
int first_slash(char argv1[]);
/*    Xoa di phan "https://" va dau '/' cuoi cung   */
void form(char argv1[]);
/*    Lay phan Host    */
char* getHost(char argv1[]);
/*    Lay phan domain   */
char* getDomain(char argv1[]);
/*    Lay phan GET    */
char* getRequest(char argv1[]);
/*    Lay ten file    */
char* getFile(char argv1[]);
/*    Tao phan sendbuf    */
char* getSendbuf(char argv1[]);
/*    Tai folder chua nhieu file   */
bool is_folder(char argv1[]);
/*    Tim vi tri cua "<!doctype html>   */
int find_doc_html(char Recvbuf[]);
/*    Tim vi tri cua "</html>   */
int find_close_html(char Recvbuf[]);
/*    Content-Lengtg    */
int checkConLen(char Recvbuf[]);
/*    Transfer-Encoding: chunked    */
bool checkTEc(char Recvbuf[]);
/*    Header size    */
int headerSz(char Recvbuf[]);
/*    Find    *chunkbyte*\r\n      byte    */
int findHeadByte(char Recvbuf[]);
/*    Find   chunk   byte   */
int findChunkByte(char Recvbuf[]);
/*    Download with Content-Length    */
int conLenDown(char Recvbuf[], int Buflen, SOCKET ConSock, int ConLen, int hSz);
/*    Download with Transfer-Encoding: chunked    */
int chunkDown(char Recvbuf[], int Buflen, SOCKET ConSock, int Byterecv, int hSz, char argv1[]);
/*    Download specific file    */
int downFile(char argv1[], char Recvbuf[], int Buflen, SOCKET ConSock);
