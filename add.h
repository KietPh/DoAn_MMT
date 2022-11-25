#pragma once

#include <iostream>
#include <fstream>
using namespace std;

/*    Tim dau '/' o cuoi nhat   */
int last_slash(char str[]);
/*    Tim dau '.' o cuoi nhat   */
int last_dot(char str[]);
/*    Xoa di phan "https://" va dau '/' cuoi cung   */
void form(char str[]);
/*    Tai trang   */
bool is_webpage(char str[]);
/*    Tai 1 file cu the   */
bool is_file(char str[]);
/*    Tai folder chua nhieu file   */
bool is_folder(char str[]);
/*    Tim vi tri cua "<!doctype html>   */
int find_doc_html(char str[]);
/*    Tim vi tri cua "</html>   */
int find_close_html(char str[]);
/*    Content-Lengtg    */
bool checkConLen(char str[]);
/*    Transfer-Encoding: chunked    */
bool checkTEc(char str[]);
/*    Header size    */
int headerSz(char str[]);
/*    Delete header part    */
void eraseHeader(char str[]);