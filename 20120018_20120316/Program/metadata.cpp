#include"metadata.h"

const float weight[3] = { 0.75, 1.25, 1.5 };

std::size_t countFiles(std::filesystem::path path) {
	using std::filesystem::directory_iterator;
	return std::distance(directory_iterator(path), directory_iterator{});
}
void getTopic(Data& metadata, string trainPath) {
	metadata.numTopic = (int)countFiles(trainPath) + 1;
	metadata.topics = new Topic[metadata.numTopic];
	int cnt = 0;
	for (auto& file : fs::directory_iterator(trainPath)) {
		fs::path p = file.path();
		metadata.topics[cnt++].name = extractFileName(p.string());
	}
	metadata.topics[cnt].files = new File[SIZE_OTHER];
	metadata.topics[cnt++].name = "Other";
}
void getIndex(Data& metadata, string trainPath) {
	getTopic(metadata, trainPath);
	for (int i = 0; i < metadata.numTopic - 1; i++) {
		metadata.topics[i].numFiles = countFiles(trainPath + "\\" + metadata.topics[i].name);
		metadata.topics[i].files = new File[metadata.topics[i].numFiles];
		int cnt = 0;
		for (auto& file : fs::directory_iterator(trainPath + "\\" + metadata.topics[i].name)) {
			fs::path p = file.path();
			metadata.topics[i].files[cnt++].name = extractFileName(p.string());
		}
	}
}
void TrainFile(File& file, string trainPath, bool addQuery = false) {
	if(addQuery == false)
		file.path = trainPath + "\\" + file.name;
	extractKeyWord(file.path, file.grams, file.rate);
}
void TrainTopic(Topic& topic, string trainPath, int x = 42, int y = 0) {
	topic.path = trainPath + "\\" + topic.name;
	for (int i = 0; i < topic.numFiles; i++) {
		TrainFile(topic.files[i], topic.path);
		float percent = 1.0 * (i + 1) / topic.numFiles * 100;
		goToXy(x, y); cout << fixed << setprecision(2) << percent << "%   ";
	}
}
void TrainData(Data& metadata, string trainPath, int x, int y) {
	metadata.del();
	metadata.path = trainPath;
	getIndex(metadata, trainPath);
	for (int i = 0; i < metadata.numTopic - 1; i++) {
		string output = "Training \"" + metadata.topics[i].name + "\"";
		output += "(" + to_string(i + 1) + '/' + to_string(metadata.numTopic - 1) + ")";
		while (output.size() < 42) output += ".";
		goToXy(x, y); cout << output;
		TrainTopic(metadata.topics[i], trainPath, x + 42, y);
	}
}
void ReadFile(File& file, ifstream& fin, string& trainPath) {
	getline(fin, file.name);
	file.path = trainPath + "\\" + file.name;
	for (int i = 0, cap = 0; i < 3; i++) {
		fin >> cap;
		file.grams[i].init(cap);
		file.rate[i].init(cap);
		file.rate[i].size = file.grams[i].size = cap;
		for (int j = 0; j < file.grams[i].size; j++) {
			string token;
			for (int w = 0; w <= i; w++) {
				fin >> token;
				file.grams[i].s[j] += token + ' ';
			}
			file.grams[i].s[j].pop_back();
			fin >> file.rate[i].a[j];
		}
	}
}
void ReadTopic(Topic& topic, ifstream& fin, string& trainPath, int x = 0, int y = 0) {
	getline(fin, topic.name);
	topic.path = trainPath + "\\" + topic.name;
	fin >> topic.numFiles;
	if (topic.name == "Other")
		topic.files = new File[SIZE_OTHER - topic.numFiles];
	else
		topic.files = new File[topic.numFiles];
	for (int i = 0; i < topic.numFiles; i++) {
		fin.ignore();
		ReadFile(topic.files[i], fin, topic.path);
		float percent = 1.0 * (i + 1) / topic.numFiles * 100;
		cout << fixed << setprecision(2) << percent << "%   ";
		goToXy(40 + x, 0 + y);
	}
}
void ReadData(Data& metadata, string dataPath, int x, int y) {
	ifstream fin(dataPath);
	getline(fin, metadata.path);
	fin >> metadata.numTopic;
	metadata.topics = new Topic[metadata.numTopic];
	for (int i = 0; i < metadata.numTopic; i++) {
		string output = "Loading app ";
		output += "(" + to_string(i + 1) + '/' + to_string(metadata.numTopic) + ")";
		while (output.size() < 40) output += ".";
		goToXy(x, y); cout << output;
		fin.ignore();
		ReadTopic(metadata.topics[i], fin, metadata.path, x, y);
		goToXy(x, y);
	}
	fin.close();
}
void PrintFile(File& file, ofstream& fout) {
	if (file.isDel == true)
		return;
	fout << file.name << '\n';
	for (int i = 0; i < 3; i++) {
		fout << file.grams[i].size << '\n';
		for (int j = 0; j < file.grams[i].size; j++)
			fout << file.grams[i].s[j] << ' ' << file.rate[i].a[j] << '\n';
	}
}
void PrintTopic(Topic& topic, ofstream& fout) {
	fout << topic.name << '\n';
	fout << topic.numFiles - topic.cntDel << '\n';
	for (int i = 0; i < topic.numFiles; i++)
		PrintFile(topic.files[i], fout);
}
void PrintData(Data& metadata, string dataPath) {
	ofstream fout(dataPath);
	fout << metadata.path << '\n';
	fout << metadata.numTopic << '\n';
	for (int i = 0; i < metadata.numTopic; i++){
		PrintTopic(metadata.topics[i], fout);
	}
	fout.close();
}
int findFile(Topic& topic, string file) {
	if (topic.name == "Other") {
		for (int i = 0; i < topic.numFiles; i++)
			if (topic.files[i].name == file)
				return i;
		return -1;
	}
	int l = 0, r = topic.numFiles - 1;
	while (l <= r) {
		int mid = l + (r - l) / 2;
		if (topic.files[mid].name == file)
			if (topic.files[mid].isDel == true)
				return -1;
			else
				return mid;
		if (topic.files[mid].name < file)
			l = mid + 1;
		else
			r = mid - 1;
	}
	return -1;
}
string deleteFile(Data& metadata, string Fpath) {
	string Fname = extractFileName(Fpath);
	for (int i = 0; i < metadata.numTopic; i++) {
		int pos = findFile(metadata.topics[i], Fname);
		if (pos != -1) {
			if (metadata.topics[i].files[pos].isDel) 
				return "This file is not in data";
			metadata.topics[i].cntDel++;
			metadata.topics[i].files[pos].isDel = true;
			return "Delete done              ";
		}
	}
	return "This file is not in data";
}
string addFile(Data& metadata, string Fpath) {
	ifstream f(Fpath);
	if (f.good() == false) {
		f.close();
		return "Can not open file           ";
	}
	f.close();
	string Fname = extractFileName(Fpath);
	for (int i = 0; i < metadata.numTopic; i++) {
		int pos = findFile(metadata.topics[i], Fname);
		if (pos != -1) {
			if (metadata.topics[i].files[pos].isDel == true) {
				metadata.topics[i].files[pos].isDel = false;
				metadata.topics[i].cntDel--;
				return "Add done              ";
			}
			else return "This file has been existed in data";

		}
	}
	File tmp;
	tmp.name = Fname;
	tmp.path = Fpath;
	TrainFile(tmp, Fpath, true);
	int Back = metadata.topics[metadata.numTopic - 1].numFiles;
	metadata.topics[metadata.numTopic - 1].files[Back] = tmp;
	metadata.topics[metadata.numTopic - 1].numFiles++;
	return "Add done              ";
}

void compareFile(File& file, string& gram, int& type) {
	if (file.isDel == true)
		return;
	int pos = file.grams[type].lowerBound(gram);
	if (pos < file.grams[type].size && file.grams[type].s[pos] == gram)
		file.appear[type] += file.rate[type].a[pos];
}
void computeFile(File& file, VectorPSI& res) {
	if (file.isDel == true)
		return;
	float sum = 0.0;
	for (int i = 0; i < 3; i++){
		sum += weight[i] * file.appear[i];
		file.appear[i] = 0;
	}
	if (sum > 0) {
		PairSI tmp = { file.path, 1 * sum };
		res.pushBack(tmp);
	}
}
void compareData(Data& metadata, string& gram, int& type) {
	for (int i = 0; i < metadata.numTopic; i++)
		for (int j = 0; j < metadata.topics[i].numFiles; j++)
			compareFile(metadata.topics[i].files[j], gram, type);
}
void computeData(Data& metadata, VectorPSI& res) {
	for (int i = 0; i < metadata.numTopic; i++)
		for (int j = 0; j < metadata.topics[i].numFiles; j++)
			computeFile(metadata.topics[i].files[j], res);
}
void SearchData(Data& metadata, string input, VectorPSI& res) {
	fixWord(input);
	lowerCase(input);
	string tmp = input;
	deleteStopWord(tmp);
	if (tmp.size() == 0)
		return;
	VectorStr words;
	extractWord(input, words);
	for (int type = 0; type < min(3, words.size); type++) {
		VectorStr gram;
		for (int i = 0; i < words.size - type; i++) {
			string token;
			for (int j = i; j <= i + type; j++)
				token += words.s[j] + ' ';
			token.pop_back();
			compareData(metadata, token, type);
		}
		gram.del();
	}
	computeData(metadata, res);
	res.quickSort(0, res.size - 1);
	res.size = min(res.size, 25);
	words.del();
}