#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#define _HAS_STD_BYTE 0
#define MIN_RATE 100
#define MAX_RATE 1000
#define MIN_WORD 50

#include "stringFunction.h"
#include "InOut.h"

wstring idVowels = L"aAeEiIoOuUyYdD";
wstring vowels[14] = {
L"àáạảãâầấậẩẫăằắặẳẵ", L"ÀÁẠẢÃÂẦẤẬẨẪĂẰẮẶẲẴ",
L"èéẹẻẽêềếệểễ", L"ÈÉẸẺẼÊỀẾỆỂỄ",
L"ìíịỉĩ", L"ÌÍỊỈĨ",
L"òóọỏõôồốộổỗơờớợởỡ", L"ÒÓỌỎÕÔỒỐỘỔỖƠỜỚỢỞỠ",
L"ùúụủũưừứựửữ", L"ÙÚỤỦŨƯỪỨỰỬỮ",
L"ỳýỵỷỹ", L"ỲÝỴỶỸ",
L"đ", L"Đ" };



VectorStr stopWords;

void delStopWords(){
	stopWords.del();
}
string extractFileName(string path) {
	string res;
	for (int i = path.size() - 1; i >= 0 && path[i] != '\\'; i--) 
		res += path[i];
	reverse(res.begin(), res.end());
	return res;
}

void lowerCase(string& str) {
	for (int i = 0; i < str.size(); i++) 
		str[i] = tolower(str[i]);
}
void fixWord(string& str) {
	for (int i = 0; i < str.size(); i++) 
		if (!((str[i] >= 'a' && str[i] <= 'z') ||
			(str[i] >= 'A' && str[i] <= 'Z') ||
			(str[i] >= '0' && str[i] <= '9') || str[i] == '\'')) 
			str[i] = ' ';
}
void extractWord(string content, VectorStr& words, const char* tok) {
	words.cap = 1;
	for (int i = 0; i < content.size(); i++)
		if (content[i] == tok[0])
			words.cap++;
	words.init(words.cap);
	char* token = strtok((char*)content.c_str(), tok);
	while (token != NULL){
		words.pushBack(token);
		token = strtok(NULL, tok);
	}
}
void deleteStopWord(string& content) {
	VectorStr childWords, momWords;
	extractWord(content, momWords);
	content = "";
	for (int i = 0; i < momWords.size; i++) {
		int fr = stopWords.lowerBound(momWords.s[i]);
		bool isStopWord = false;
		for (int j = fr; j < stopWords.size; j++) {
			bool isDif = false;
			extractWord(stopWords.s[j], childWords);
			if (momWords.s[i] != childWords.s[0]) 
				break;
			if (i + childWords.size > momWords.size) 
				continue;
			for (int t = 1; t < childWords.size; t++) 
				if (momWords.s[i + t] != childWords.s[t]) {
					isDif = true;
					break;
				}
			if (isDif == false) {
				isStopWord = true;
				i += childWords.size - 1;
				break;
			}
		}
		if (isStopWord == false) 
			content += momWords.s[i] + ' ';
	}
	childWords.del(); momWords.del();
}
string XoaDau(std::wstring& w_txt) {
	for (int i = 0; i < w_txt.size(); i++) 
		for (int id = 0; id < 14; id++) 
			for (int j = 0; j < vowels[id].size(); j++) 
				if (w_txt[i] == vowels[id][j]) 
					w_txt[i] = idVowels[id];
	string txt(w_txt.begin(), w_txt.end());
	return txt;
}
void createStopWord() {
	wstring w_content = ReadFileUTF("source\\vietnamese-stopwords.txt");
	string content = XoaDau(w_content);
	extractWord(content, stopWords, "\n");
	stopWords.quickSort(0, stopWords.size - 1);
}
void countAppearance(VectorStr& words, VectorStr& grams, VectorInt& rate, int type) {
	grams.init(words.size - type);
	for (int i = 0; i < words.size - type; i++) {
		string token;
		for (int j = i; j <= i + type; j++) {
			token += words.s[j] + ' ';
		}
		token.pop_back();
		grams.pushBack(token);
	}
	grams.quickSort(0, grams.size - 1);
	grams.unique();
	rate.init(grams.size);
	rate.size = grams.size;
	VectorStr gramToken;
	for (int i = 0; i < words.size - type; i++) {
		int fr = grams.lowerBound(words.s[i]);
		for (int j = fr; j < grams.size; j++) {
			extractWord(grams.s[j], gramToken);
			if (words.s[i] != gramToken.s[0])
				break;
			bool isSame = true;
			for (int t = 1; t < gramToken.size; t++) {
				if (words.s[i + t] != gramToken.s[t]) {
					isSame = false;
					break;
				}
			}
			if (isSame == true)
				rate.a[j]++;
		}
	}
	gramToken.del();
	for (int i = 0; i < rate.size; i++) 
		rate.a[i] = (float)rate.a[i] / (words.size - type) * 100 * 100; // count rate 
	int cnt = 0;
	for (int i = 0; i < rate.size; i++)
		if (words.size <= MIN_WORD || (rate.a[i] >= MIN_RATE && rate.a[i] <= MAX_RATE)) cnt++;
	VectorStr tmpGrams;
	tmpGrams.init(cnt);
	VectorInt tmpRate;
	tmpRate.init(cnt);
	for (int i = 0; i < rate.size; i++)
		if (words.size <= MIN_WORD || (rate.a[i] >= MIN_RATE && rate.a[i] <= MAX_RATE)) {
			tmpGrams.pushBack(grams.s[i]);
			tmpRate.pushBack(rate.a[i]);
		}
	grams.del();
	rate.del();
	grams = tmpGrams;
	rate = tmpRate;
}
void extractKeyWord(string path, VectorStr grams[3], VectorInt rate[3]) {
	wstring w_content = ReadFileUTF(path);
	string content = XoaDau(w_content);
	lowerCase(content);
	fixWord(content);
	deleteStopWord(content);
	VectorStr words;
	extractWord(content, words);
	for (int i = 0; i < min(3, words.size); i++) 
		countAppearance(words, grams[i], rate[i], i);
	words.del();
}