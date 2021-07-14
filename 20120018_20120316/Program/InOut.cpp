#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS_
#define _HAS_STD_BYTE 0

#include "InOut.h"

std::wstring ReadFileUTF8(string path) {
	std::wstring line, res;
	std::wifstream fin(path);
	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);  // UTF-8
	fin.imbue(loc);
	while (getline(fin, line))
		res += line + L"\n";
	fin.close();
	return res;
}

std::wstring ReadFileUTF16(string path) {
	std::wstring line, res;
	std::wifstream fin(path);
	fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
	while (getline(fin, line))
		res += line + L"\n";
	fin.close();
	return res;
}
std::wstring ReadFileUTF(string path) {
	ifstream f(path, std::ios::binary);
	int ch1 = f.get();
	int ch2 = f.get();
	f.close();
	if ((ch1 == 0xff && ch2 == 0xfe) || (ch1 == 0xfe && ch2 == 0xff))
		return ReadFileUTF16(path);
	return ReadFileUTF8(path);
}
