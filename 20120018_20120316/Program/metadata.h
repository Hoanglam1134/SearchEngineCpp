#pragma once
#define _HAS_STD_BYTE 0
#define SIZE_OTHER 100

#include <iomanip>
#include "STL.h"
#include "InOut.h"
#include "WindowFunctions.h"
#include "stringFunction.h"

struct File {
	string name;
	string path;
	bool isDel = false;
	int appear[3] = { 0 };
	VectorStr grams[3];
	VectorInt rate[3];
	void del() {
		for (int i = 0; i < 3; i++) {
			grams[i].del();
			rate[i].del();
		}
	}
};
struct Topic {
	int numFiles = 0;
	int cntDel = 0;
	string path;
	string name;
	File* files;
	void del(){
		for(int i = 0 ; i < numFiles; i++)
			files[i].del();
		if(files) delete[]files;
	}
};
struct Data {
	int numTopic;
	Topic* topics = nullptr;
	string path;
	void del(){
		for(int i = 0; i < numTopic; i++)
			topics[i].del();
		if(topics) delete[]topics;
	}
};

void TrainData(Data& metadata, string trainPath, int x = 0, int y = 0);
void ReadData(Data& metadata, string dataPath, int x = 0, int y = 0);
void PrintData(Data& metadata, string dataPath);
string deleteFile(Data& metadata, string Fpath);
string addFile(Data& metadata, string Fpath);
void SearchData(Data& metadata, string input, VectorPSI& res);
