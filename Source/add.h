#pragma once

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_PORT "80"
#define DEFAULT_BUFLEN 2000

#include <iostream>
#include <fstream>
#include <direct.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <windows.h>
using namespace std;


/*    Thay cac gia tri argv vao bien Argv*/
void fill_argv(int argc, char** argv);
/*    Lay cac gia tri trong Argv lap vao bien link */
void get_link();
/*    Tao mot socket moi   */
SOCKET createSocket();
/*    Tim vi tri 'href' dau tien   */
int find_href(char* Recvbuf);
/*    Tim so luong 'href      */
int find_href_amount(char Recvbuf[]);
/*    Tim dau '>'   */
int find_close(char* Recvbuf);
/*    Tim dau '/' o cuoi nhat   */
int last_slash();
/*    Tim dau '/' o cuoi thu 2   */
int second_last_slash();
/*    Tim dau '.' o cuoi nhat   */
int last_dot();
/*    Dem so dau '.' trong domain   */
int count_dot();
/*    Tim dau '/' dau tien    */
int first_slash();
/*    Tim dau '=' gan nhat    */
int find_equal();
/*    Xoa di phan "https://" va dau '/' cuoi cung   */
void form();
/*    Lay ten file    */
char* getFile();
/*    Tai folder chua nhieu file   */
bool is_folder();
/*    /Chunked      */
bool is_Chunked();
/*    Lay phan Host    */
char* getHost();
/*    Lay phan domain   */
char* getDomain();
/*    Lay phan GET    */
char* getRequest();
/*    Lay ten folder   */
char* getFolder();
/*    Tao phan sendbuf    */
char* getSendbuf();
/*    Tim vi tri cua "<!doctype html>   */
int find_doc_html(char Recvbuf[]);
/*    Tim vi tri cua "</html>   */
int find_close_html(char Recvbuf[]);
/*    Content-Lengtg    */
int checkConLen(char Recvbuf[]);
/*    Transfer-Encoding: chunked    */
int checkTEc(char Recvbuf[]);
/*    Header size    */
int headerSz(char Recvbuf[]);
/*    Find    *chunkbyte*\r\n      byte    */
int findHeadByte(char Recvbuf[]);
/*    Find   chunk   byte   */
int findChunkByte(char Recvbuf[]);
/*    Download with Content-Length    */
int conLenDown(char Recvbuf[], SOCKET ConSock, int Byterecv, int hSz, int Conlen);
/*    Download with Transfer-Encoding: chunked    */
int chunkDown(char Recvbuf[], SOCKET ConSock, int Byterecv, int hSz);
/*    Download specific file    */
int downFile(SOCKET ConSock);
/*    Thread function    */
DWORD WINAPI threadFunc(LPVOID lpParam);
/*    Tai het cac file trong folder    */
int downFolder(SOCKET ConSock);
