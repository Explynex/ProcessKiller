#include "tray.h"
#include <iostream>
#include <fstream>
#include <Tlhelp32.h>
#define STATUSCOUNT 4
struct processlist {
	std::vector <std::string> list;
	int counter=0;
};
std::string Status[4] = { "OFF","OFF","OFF","OFF" };
std::string systemService[10] = { "dllhost.exe","ServiceHub.","conhost.exe","svchhost.exe","ekrn.exe","System.exe","NVDisplay.Container.exe","Taskmgr.exe","explorer.exe","crss.exe"};
std::thread* core = nullptr;
bool ExitFlag = false, showLog = false,mainSize = false;
int timeCheck = 4;

void threadCore(std::string str = "");
void winPause() {
	system("pause > nul");
}

bool remove_line(std::string filename, size_t index){
	std::vector<std::string> vec;
	std::ifstream file(filename);
	if (file.is_open())
	{
		std::string str;
		while (std::getline(file, str))
			vec.push_back(str);
		file.close();
		if (vec.size() < index-1)
			return false;
		vec.erase(vec.begin() + index-1);
		std::ofstream outfile(filename);
		if (outfile.is_open())
		{
			std::copy(vec.begin(), vec.end(),
				std::ostream_iterator<std::string>(outfile, "\n"));
			outfile.close();
			return true;
		}
		return false;
	}
	return false;
}

int processChecker(std::string filename){
	int status = 0;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes){
		if (strcmp(pEntry.szExeFile, filename.c_str()) == 0){
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			status = 1;
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	if (status == 0) return 0;
	else return 1;
}

int fileProcessListChecker(std::string nameProcess) {
	std::ifstream fin;
	std::string str;
	fin.open("processlist.md");
	while (fin >> str) {
		if (str == nameProcess) {
			fin.close();
			return 1;
		}
	}
	fin.close();
	return 0;
}

void generateMenuUpDown(int x, int y, int stringcount, int pointer, std::string Menu[]) {
	setColor(LIGHTGRAY, BLACK);
	GotoXY(x, y, "╭━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╮\n");
	for (int i = 0; i < stringcount; ++i){
		if (i == pointer){
			GotoXY(x, i + y + 1, "┃");
			GotoXY(x + 35, i + y + 1, "┃");
			setColor(LIGHTGREEN, BLACK);
			GotoXY(x + 1, i + y + 1, "");
			std::cout << " >>" << Menu[i];
			GotoXY(92, i + y + 15, "");
		}
		else{
			setColor(LIGHTGRAY, BLACK);
			GotoXY(x, i + y + 1, "");
			std::cout << "┃" << Menu[i];
			GotoXY(x + 35, i + y + 1, "┃");
		}
		setColor(LIGHTGRAY, BLACK);
		GotoXY(x, stringcount + y + 1, "╰━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╯\n");
	}
}

void showProcessList() {
	setConsoleSize(76, 250);
	COORD cursPos;
	std::string name, command;
	std::ofstream fin;
	int status = 0,exit;
	while (true) {
		system("cls");
		setColor(LIGHTGREEN, BLACK);
		system("tasklist");
		setColor(LIGHTGRAY, BLACK);
		std::cout << "\n Name of the process:> ";
		cursPos = getConsoleCursorPosition();
		do {
		s:	exit = 0;
			GotoXY(cursPos.X, cursPos.Y, "ESC to exit.          ", DARKGRAY, BLACK);
			Sleep(800);
			cleaning(cursPos.X, cursPos.Y, 12, 1);
			setColor(LIGHTGRAY, BLACK);
			name = printFilter(cursPos.X, cursPos.Y, 30, "str", " ", "z");
			if (name == "0") {
				mainSize = 0;
				return;
			}
			status = processChecker(name);
			if (status == 0) {
				GotoXY(cursPos.X, cursPos.Y, "Process not found!", LIGHTRED, BLACK);
				Sleep(800);
			}
			if (Status[2] == "OFF") {
				for (int i = 0; i < 10; i++){
					if(name == systemService[i]) {
						exit = 1;
						GotoXY(cursPos.X, cursPos.Y, "Its system process!", LIGHTRED, BLACK);
						Sleep(800);
						goto s;
					}
				}
			}
		} while (status != 1);
		command = "taskkill /F /IM " + name + " /T";
		GotoXY(cursPos.X, cursPos.Y);
		setColor(LIGHTGREEN, BLACK);
		system(command.c_str());
		setColor(LIGHTGRAY, BLACK);
		std::cout << "\nKill process automatically on startup? y\\n:> ";
		char key = _getch();
		if (key == 'Y' || key == 'y') {
			if (fileProcessListChecker(name) == 1) {
				setColor(LIGHTRED, BLACK);
				std::cout << "This process is already added!";
			}
			else {
				fin.open("processlist.md", std::ios::app);
				fin << name;
				fin.close();
				setColor(LIGHTGREEN, BLACK);
				std::cout << "Yes";
			}
		}
		else {
			setColor(LIGHTRED, BLACK);
			std::cout << "No";
		}
		Sleep(1500);
		break;
	}
	mainSize = 0;
}

void showBlockedList() {
	system("cls");
	setConsoleSize(49, 20);
	std::ifstream fin;
	COORD cord;
	std::string s;
	int i = 0;
	std::cout << "  ╭━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╮" << std::endl;
	std::cout << "  ┃  List of automatically blocked processes  ┃" << std::endl;
	std::cout << "  ╰━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╯" << std::endl << std::endl;
	setColor(LIGHTGREEN, BLACK);
	fin.open("processlist.md");
	while (fin >> s) {
		std::cout << "  " << i + 1 << ". " << s << std::endl;
		i++;
	}
	fin.close();
	if (i == 0) {
		setColor(LIGHTRED, BLACK);
		std::cout << "\n\n  Empty... ";
		winPause();
		return;
	}
	setColor(LIGHTGRAY, BLACK);
	std::cout << "\n\n  Enter number to delete: ";
	cord = getConsoleCursorPosition();
	i = stoi(printFilter(cord.X, cord.Y,2, "digit", "0", "9", 1, i));
	if (i == 0) {
		mainSize = 0;
		return;
	}
	remove_line("processlist.md", i);
	mainSize = 0;
	winPause();
}

processlist processKillerCoreChecker() {
	processlist pList{};
	std::string temp;
	std::ifstream fin;
	fin.open("processlist.md");
	if (fin.is_open()) {
		for (pList.counter = 0; fin >> temp; pList.counter++) {
			pList.list.push_back(temp);
		}
		fin.close();
		return pList;
	}
	else return pList;
}

void processKillerCore() {
	std::string command;
	while (true && ExitFlag == 0) {
		processlist pList = processKillerCoreChecker();
		if (pList.counter != 0) {
			for (int i = 0; i < pList.counter; i++) {
				int k = processChecker(pList.list[i]);
				if (k != 0) {
					if(showLog==true)
						command = "taskkill /F /IM " + pList.list[i] + " /T";
					else
						command = "taskkill /F /IM " + pList.list[i] + " /T > nul";
					system(command.c_str());
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(timeCheck));
	}
}

void statusOptionChecker(std::string str  = "") {
	std::string Menu[STATUSCOUNT] = { "ProcessKiller core: ", "Quick process check: ","Kill system services: ","Minimize to tray: "};
	std::ofstream fout;
	std::ifstream fin;
	std::string temp;
	fin.open("propertions.ini");
	if (!fin.is_open()|| str == "save") {
	s:	fin.close();
		fout.open("propertions.ini");
		for (int i = 0; i < STATUSCOUNT; i++) {
			fout << Menu[i] + Status[i] << "\n";
		}
		fout.close();
	}
	else {
		for (int i = 0, j = 0; i == STATUSCOUNT, fin >> temp; j++) {
			if (j > 4 && i == 0) goto s;
			if (temp == "ON" || temp == "OFF") {
				Status[i] = temp;
				i++;
			}
		}
		fin.close();
	}
}

void showOptions() {
	std::string Menu[5] = { " Process Killer core ", " Quick process check "," Kill system services "," Minimize to tray "," Back   "};
	int pointer = 0;
	system("cls");
	while (true)
	{
		if (statusCheck() != 0) winPause();
		setColor(GREEN, BLACK);
		GotoXY(3, 0, "╭━━━┳━━━┳━━━━┳━━┳━━━┳━╮ ╭┳━━━╮");
		GotoXY(3, 1, "┃╭━╮┃╭━╮┃╭╮╭╮┣┫┣┫╭━╮┃┃╰╮┃┃╭━╮┃");
		GotoXY(3, 2, "┃┃ ┃┃╰━╯┣╯┃┃╰╯┃┃┃┃ ┃┃╭╮╰╯┃╰━━╮");
		GotoXY(3, 3, "┃┃ ┃┃╭━━╯ ┃┃  ┃┃┃┃ ┃┃┃╰╮┃┣━━╮┃");
		GotoXY(3, 4, "┃╰━╯┃┃    ┃┃ ╭┫┣┫╰━╯┃┃ ┃┃┃╰━╯┃");
		GotoXY(3, 5, "╰━━━┻╯    ╰╯ ╰━━┻━━━┻╯ ╰━┻━━━╯");
		for (int i = 0; i < STATUSCOUNT; i++) {
			if(Status[i]=="OFF")
				GotoXY(30, i+7, Status[i], RED, BLACK);
			else
				GotoXY(30, i + 7, Status[i]+" ", GREEN, BLACK);
		}
		setColor(LIGHTGRAY, BLACK);
		GotoXY(2, 6, "╭━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━╮\n");
		for (int i = 0; i < 5; ++i) {
			if (i == pointer) {
				GotoXY(2, i + 6 + 1, "┃");
				GotoXY(2+26, i + 6 + 1, "┃");
				GotoXY(2 + 32, i + 6 + 1, "┃");
				setColor(LIGHTGREEN, BLACK);
				GotoXY(2 +1, i + 6 + 1);
				std::cout  << Menu[i];
			}
			else {
				setColor(LIGHTGRAY, BLACK);
				GotoXY(2, i + 6 + 1, "");
				std::cout << "┃" << Menu[i];
				GotoXY(2 + 32, i + 6 + 1, "┃");
				GotoXY(2 + 26, i + 6 + 1, "┃");
			}
			setColor(LIGHTGRAY, BLACK);
			GotoXY(2, 5 + 6 + 1, "╰━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━╯\n");
		}
		while (true && statusCheck() == 0)
		{

			Sleep(100);
			if (GetKeyState(0x57) < 0 || GetKeyState(VK_UP) < 0) {
				pointer -= 1;
				if (pointer == -1)
					pointer = STATUSCOUNT;
				break;
			}
			if (GetKeyState(0x53) < 0 || GetKeyState(VK_DOWN) < 0) {
				pointer += 1;
				if (pointer == STATUSCOUNT+1)
					pointer = 0;
				break;
			}
			else if (GetKeyState(VK_RETURN) < 0) {
				switch (pointer) {
				std::cin.ignore(32767, '\n');
				case 0:
					if (Status[0] == "OFF") {
						Status[0] = "ON";
						threadCore();
						ExitFlag = 0;
					}
					else if (Status[0] == "ON") {
						threadCore("disable");
						Status[0] = "OFF";
					}
					statusOptionChecker("save");
					goto m;
				case 1:
					if (Status[1] == "OFF") {
						Status[1] = "ON";
						timeCheck = 1;
					}
					else {
						Status[1] = "OFF";
						timeCheck = 4;
					}
					statusOptionChecker("save");
					goto m;
				case 2:
					if (Status[2] == "OFF") {
						Status[2] = "ON";
					}
					else Status[2] = "OFF";
					statusOptionChecker("save");
					goto m;
				case 3:
					if (Status[3] == "OFF") {
						Status[3] = "ON";
					}
					else Status[3] = "OFF";
					statusOptionChecker("save");
					goto m;
				case 4: return;
				}
			}
			else if (GetKeyState(VK_ESCAPE) < 0) {
				Sleep(150);
				return;
			}
		}
	m:;
	}
}

void printRuntimeLog(){
	system("cls");
	setConsoleSize(76, 1000);
	std::cout << "\t\t  ╭━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╮" << std::endl;
	std::cout << "\t\t  ┃  Process Killer kernel runtime log  ┃" << std::endl;
	std::cout << "\t\t  ╰━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╯" << std::endl << std::endl;
	if (Status[0] == "OFF")
		GotoXY(3, 5, "Kernel is disabled!", LIGHTRED, BLACK);
	else {
		showLog = true;
		setColor(GREEN, BLACK);
	}
	mainSize = 0;
	winPause();
	showLog = false;
}

void mainMenu() {
	std::string Menu[5] = { " Show process list   \n", " Show blocked processes   \n"," Show kernel log     "," Options   "," Exit   "};
	int pointer = 0;
	while (true)
	{
		if (mainSize == 0) {
			setConsoleSize(40, 14);
			mainSize = 1;
		}
		if (statusCheck() != 0) winPause();
		setColor(GREEN, BLACK);
		GotoXY(2, 0, "╭━━━╮                ╭╮╭━╮╭╮╭╮");
		GotoXY(2, 1, "┃╭━╮┃                ┃┃┃╭╯┃┃┃┃");
		GotoXY(2, 2, "┃╰━╯┣━┳━━┳━━┳━━┳━━┳━━┫╰╯╯╭┫┃┃┃╭━━┳━╮");
		GotoXY(2, 3, "┃╭━━┫╭┫╭╮┃╭━┫┃━┫━━┫━━┫╭╮┃┣┫┃┃┃┃┃━┫╭╯");
		GotoXY(2, 4, "┃┃  ┃┃┃╰╯┃╰━┫┃━╋━━┣━━┃┃┃╰┫┃╰┫╰┫┃━┫┃");
		GotoXY(2, 5, "╰╯  ╰╯╰━━┻━━┻━━┻━━┻━━┻╯╰━┻┻━┻━┻━━┻╯");
		setColor(LIGHTGRAY, BLACK);
		generateMenuUpDown(2, 6, 5, pointer, Menu);
		while (true && statusCheck()==0)
		{
			Sleep(100);
			if (GetKeyState(0x57) < 0 || GetKeyState(VK_UP) < 0) {
				pointer -= 1;
				if (pointer == -1)
					pointer = 4;
				break;
			}
			if (GetKeyState(0x53) < 0 || GetKeyState(VK_DOWN) < 0) {
				pointer += 1;
				if (pointer == 5)
					pointer = 0;
				break;
			}
			else if (GetKeyState(VK_RETURN) < 0) {
				switch (pointer) {
				case 0:
					std::cin.ignore(32767, '\n');
					showProcessList();
					system("cls");
					goto m;
				case 1:
					std::cin.ignore(32767, '\n');
					showBlockedList();
					system("cls");
					goto m;
				case 2:
					std::cin.ignore(32767, '\n');
					printRuntimeLog();
					system("cls");
					goto m;
				case 3:
					std::cin.ignore(32767, '\n');
					showOptions();
					system("cls");
					goto m;
				case 4:
					return;

				}
			}
			else if (GetKeyState(VK_ESCAPE) < 0) {
				Sleep(50);
				return;
			}
			
		}
	m:;
	}
}

void threadCore(std::string str) {
	if (Status[0] == "ON") {
		core = new std::thread(processKillerCore);
	}
	if (Status[0] == "ON" && str == "disable") {
		ExitFlag = 1;
	}
}

int main() {
	utf.setUtfLocale();
	statusOptionChecker();
	if (Status[1] == "ON")
		timeCheck = 1;
	threadCore();
	if (Status[3] == "ON")
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	std::thread tray(Tray);
	SetConsoleTitleA("Process Killer");
	mainMenu();
	core->detach();
	tray.detach();
	return 0;
}