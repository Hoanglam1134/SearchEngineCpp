#define _HAS_STD_BYTE 0

#include "metadata.h"
#include "UI.h"
#include "WindowFunctions.h"

Data metadata; 
int main(){
	ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	Console();
	createStopWord();
	ReadData(metadata, "source\\metadata.txt");
	UI(metadata);
	DisableCtrButton(1, 0, 0);
	PrintData(metadata, "source\\metadata.txt");
	metadata.del();
	delStopWords();
	return 0;
}