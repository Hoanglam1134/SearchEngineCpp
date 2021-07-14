#define _HAS_STD_BYTE 0

#include "WindowFunctions.h"
#include "metadata.h"
#include "UI.h"
#include <fcntl.h> //_O_WTEXT
#include <io.h>    //_setmode()
using namespace std;

void drawRect(int x, int y, int a, int b) {
    goToXy(x, y);
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            if (i == 0 || (i == a - 1 && j != 0 && j != b - 1))
                cout << char(220);
            else if (j == 0 || j == b - 1) {
                cout << char(219);
            }
            else cout << ' ';
        }
        goToXy(x, y + i + 1);
    }
}
bool chkIn(char c, char s[32]) {
    for (int i = 0; i < 32; i++)
        if (c == s[i])
            return true;
    return false;
}
char deli[] = { ',','.','-','\'','+','*','/','\"','!','@','#','$','%','^','&','(',')','=','_','[',']','{','}','\\','|',';',':','<','>','?','`','~' };
string input(int x, int y, int len) {
    int n;
    char c;
    string query="";
    goToXy(x, y);
    while (true)
    {
        c = _getch();
        if (chkIn(c, deli) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == '\b' || c == 32 || (c <= '9' && c >= '0') || (int)c == -32) {
            if ((int)c == -32) {
                char tmp = _getch();
                if (tmp == 72 || tmp == 75 || tmp == 77 || tmp == 80)
                    continue;
            }
            query += c;
        }
        if (c == '\r')
            break;
        else if (c == '\b'){
            if (query.size() > 1)
                query.pop_back();
            query.pop_back();
        }
        goToXy(x, y); //search bar
        for (int i = 0; i < len; i++) cout << ' ';
        goToXy(x, y);
        if (query.size() <= len)
            cout << query;
        else
            for (int i = query.size() - len; i < query.size(); i++)
                cout << query[i];
    }
    return query;
}

bool isValidChoice(const string& str)
{
    if (str.size() == 1 && str[0] >= '0' && str[0] <= '6') return true;
    if (str.size() == 2 && str[0] == '-' && str[1] == '1') return true;
    return false;
}
void searchGuide(){
    int x = 80, y = 7;
    goToXy(x, y); cout << "Enter 1~5 to see detail.";
    goToXy(x, y+1); cout << "Enter \"0\" to go to previous page.";
    goToXy(x, y+2); cout << "Enter \"6\" to go to next page.";
    goToXy(x, y+3); cout << "Enter \"-1\" to back to menu: ";
}
void homeScreen() {  //giao dien trang chu
    goToXy(0, 0);
    cout << "Input \"*1\"~\"*5\":                                           \n"
        "      1. Search      |      2. Add file      |      3. Detele file      |      4. Train folder      |      5. Exit";
    cout << "\n\n\n\n\n\t\t\t\t\tSEARCH ENGINE\n";
    drawRect(20, 7, 3, 55);
    goToXy(60, 8); cout << "| COMMAND: ";
    
    int x = 90, y = 25;
    goToXy(x, y); cout << "Author:";
    goToXy(x, y+1); cout << "   -   Tran Kieu Minh Lam";
    goToXy(x, y+2); cout << "   -   Nguyen Hoang Lam";
    goToXy(x, y+3); cout << "   -   HCMUS - K20 - CTT1TN";
}
void printPage(VectorPSI &res, int page) {
    for(int j = 0; j < 5; j++){
        goToXy(20, 10 + j);
        cout << "                                                      ";
    }
    int tmp = min(page*5 + 5, res.size);
    for (int j = page*5; j < tmp ; j++) {
        goToXy(20, 10 + j % 5); cout << extractFileName(res.p[j].s);
        goToXy(62, 10 + j % 5); cout << res.p[j].a;
    }
    goToXy(69, 20); cout << "Page " << page + 1;
}
void SearchQuery(VectorPSI &res) { //do query
    int numPage = res.size / 5 + (res.size % 5 != 0);
    int choice;
    int page = 0;
    string query;
    while (true) {
        printPage(res, page);
        searchGuide(); // print the instruction of query
        do {
            goToXy(70, 8); cout << "   "; goToXy(70, 8); 
            query = input(70, 8, 3);
        } while (!isValidChoice(query));
        choice = stoi(query);
        // check if input is valid
        if (choice == -1) 
            break;
        if (choice == 0) {
            if (page == 0) {
                goToXy(20, 22);
                cout << "This is the fisrt page.                 ";
            }
            else {
                page -= 1;
                goToXy(20, 22);
                cout << "                                              ";
            }
        }
        else if (choice == 6) {
            if (page < numPage - 1) {
                page++;
                goToXy(20, 22); cout << "                               ";
            }
            else {
                goToXy(20, 22); cout << "This is the end of query.";
            }
        }
        else {
            if (choice > res.size - (5 * page)) continue;
            system("cls");
            cout << "                    ***This is the content of the " << extractFileName(res.p[page * 5 + choice - 1].s) << "***\n\n";
            wstring wcontent = ReadFileUTF(res.p[page * 5 + choice - 1].s);
	        _setmode(_fileno(stdout), _O_WTEXT); 
            wcout << wcontent << '\n';
	        _setmode(_fileno(stdout), _O_TEXT);
            cout << "This is the end of " << extractFileName(res.p[page * 5 + choice - 1].s);
            cout << "\nPress any key to back to menu"; _getch();
            system("cls");
            homeScreen();
        }
    }
}
void UI(Data& metadata) {
    string query="";
    SetColor(0, 14);
    VectorPSI res;
    res.init(2e4);
    do {
        homeScreen();
        do {
            goToXy(70, 8); cout<< "   ";
            query = input(70, 8, 3); 
            goToXy(20, 10); cout << "Wrong command";
        } while (query.size() != 2 || query[0] != '*' || query[1] > '5' || query[1] <'0');
        goToXy(20, 10); cout << "               ";
        if (query == "*1") {
            res.size = 0;
            SearchData(metadata,input(22, 8, 35), res);
            if (res.size == 0) {
                goToXy(20, 10);
                cout << "Sorry :( we can not find any result.";
                goToXy(20, 11);
                cout << "Press any key to back to menu"; _getch();
            }
            else
                SearchQuery(res);
        }
        if (query == "*2" || query == "*3") {
            goToXy(20, 10); cout << "Enter file name: ...";
            goToXy(22, 8); //search bar
            string path = input(22, 8, 35);
            if (query == "*2") {//add file
                goToXy(20, 10); cout << addFile(metadata,path);
                goToXy(20, 11); cout << "Press any key to back to menu"; _getch();
            }
            else { //delete file
                goToXy(20, 10); cout << deleteFile(metadata,path);
                goToXy(20, 11); cout << "Press any key to back to menu"; _getch();
            }
        }
        if (query == "*4") {
            goToXy(20, 10); cout << "Enter folder path to train ...";
            goToXy(22, 8); //search bar
            string path = input(22, 8, 35);
            if (!DirectoryExists(path.c_str())){ 
                goToXy(20, 10); cout << "The folder is not exist.         ";
                goToXy(20, 11); cout << "Press any key to back to menu.";
                _getch();
            }
            else {
                TrainData(metadata,path,20,10);
                goToXy(20, 10); cout << "Completed. Press any key to back to menu. "; 
                _getch();
            }
        }
        if (query == "*5") {
            goToXy(20, 10);
            cout << "Saving data and closing ..."<<'\n';
            goToXy(20, 11);
            cout << "Do not close the console until end." << '\n';
            return;
        }
        system("cls");
    } while (true);
    res.del();
}