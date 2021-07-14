#pragma once
#include<string>
#include"STL.h"
using namespace std;
void delStopWords();
string extractFileName(string path);
void lowerCase(string& str);
void fixWord(string& str);
void extractWord(string content, VectorStr& words, const char* tok=" ");
void deleteStopWord(string& content);
string XoaDau(std::wstring& w_txt);
void createStopWord();
void countAppearance(VectorStr& words, VectorStr& grams, VectorInt& rate, int type);
void extractKeyWord(string path, VectorStr grams[3], VectorInt rate[3]);