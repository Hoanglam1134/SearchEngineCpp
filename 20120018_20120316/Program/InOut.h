#pragma once
#include <filesystem>
#include <fstream> 
#include <locale>
#include <codecvt> 
#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include "STL.h"

using namespace std;
namespace fs = std::filesystem;
std::wstring ReadFileUTF(string path);