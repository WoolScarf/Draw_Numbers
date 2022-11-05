#include <iostream>
#include <windows.h>
#include <vector>
#include <math.h>
#include <array>
#include <utility>
#include <fstream>
#include <string>
#include <fileapi.h>
#include <sstream>


// TODO: maybe see why the calc time got up so bad...
void clearConsole() {
	system("cls");
	Sleep(50);
}
std::pair<int, int> calcCoords(int number) {	
	// using an interesting (Didn't prove it yet) property of numbers,
	// the 'median' factor(s) is usually the "best" pair as defined by me
	// for example, 72 has these factors
	// 1, 2, 3, 4, 6, 8 |SQRT CUTOFF| 9, 12, 24, 36, 72
	//                ^
	// Biggest factor, smaller than sqrt(72), is half of the "best" pair.
	// Again, this is an assumption, no proof

	int factor = 0;
	for (int i = (int)sqrt(number); i >= 1; i--) {
		if (number % i == 0) {
			factor = i;
			break;
		}
	}

	const int f1 = number / factor;
	const int f2 = factor;

	return std::make_pair(max(f1, f2), min(f1, f2));
}
std::pair<int, int> getCoordsFromFile(std::fstream &myfile) {
	std::vector<std::string> row;
	std::string line, numberAsString;

	std::getline(myfile, line);
	std::stringstream str(line);

	row.clear();
	int index = 0;
	while (getline(str, numberAsString, ',')) row.push_back(numberAsString);

	
	return std::make_pair(stoi(row[1]), std::stoi(row[2]));
}
void drawPixels(int number, int x, int y, int upperBound, char mode, int consoleHeight, int consoleWidth, HDC mydc) {
	// When drawing, skip numbers, when writing, don't skip :)
	if ((x > consoleWidth || y > consoleHeight) && mode != 'f') return;
	COLORREF COLOR = RGB(255, 255, 255);
	float quotient = (float) x / y;

	std::vector <float> quotientTest = {
		1.0f,		//0
		4.0f / 3,	//5
		3.0f / 2,	//6
		2.0f,		//7
		3.0f,		//8
		4.0f,		//9
		5.0f,		//10
		6.0f,		//11
	};
	switch (mode) {
		case 's':
			{
				float offset = 0.01f;
				for (int i = 0; i < quotientTest.size(); i++) {

					if (quotientTest[i] <= quotient && quotient <= quotientTest[i] + offset) {

						COLOR = RGB((64 * i) % 255, (64 + 64 * i) % 255, (128 + 64 * i) % 255);
						break;
					}
				}
				break;
			}
		case 'd':
			{
				for (int i = 0; i < quotientTest.size(); i++) {

					if (quotient <= quotientTest[i]) {
						COLOR = RGB((64 * i) % 255, (64 + 64 * i) % 255, (128 + 64 * i) % 255);
						break;

					}

				}
				break;
			}
	}

	SetPixel(
		mydc,
		x,
		consoleHeight - y,
		COLOR
	);
}
char getMode() {

	clearConsole();

	std::cout << "Press A for B&W\n"
		<< "Press S for colored borders\n"
		<< "Press D for colored sectors\n"
		<< "Press F to generate table of numbers and their factors (if on 1920x1080, 40MB)";

	char mode{};

	while (true) {
		if (GetKeyState(VK_ESCAPE) & 0x8000) exit(0);

		if (GetKeyState('A') & 0x8000) mode = 'a';
		if (GetKeyState('S') & 0x8000) mode = 's';
		if (GetKeyState('D') & 0x8000) mode = 'd';
		if (GetKeyState('F') & 0x8000) mode = 'f';
		if (mode) break;
		Sleep(50);
	}
	return mode;
}
int writeToFile(int upperBound, std::fstream& myfile) {
	clearConsole();
	std::cout << "Writing...\n";
	std::pair<int, int> coords;
	int number = 1;
	myfile << "          \n"; // 10 digits worth of space
	for (;number <= upperBound; number++){
		if (GetKeyState(VK_ESCAPE) & 0x8000)
			break;
		coords = calcCoords(number);
		myfile << number << ", " << coords.first << ", " << coords.second << "\n";
	}
	number -= 1;
	myfile.seekp(0);
	myfile << number;

	return number;
}
bool askUseFile(int lines) {
	clearConsole();
	std::cout << "Found a pre-generated file, with " << lines << " numbers calculated.\nUse to save calculation time ? (Y / N)";

	while (true) {
		if (GetKeyState(VK_ESCAPE) & 0x8000) exit(0);

		if (GetKeyState('Y') & 0x8000) return true;
		if (GetKeyState('N') & 0x8000) return false;
	}
	clearConsole();
}
/// <summary>
/// Stolen from https://stackoverflow.com/a/3471587
/// </summary>
void consoleSetup(HWND& consoleHWND, RECT &consoleWindowRect) {

	// Resize the console window, to fit the monitor (Fullscreen mode)
	DWORD dwStyle = GetWindowLong(consoleHWND, GWL_STYLE);
	SetWindowLong(consoleHWND, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
	SendMessage(consoleHWND, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

	// Disable "QuickEdit", disables mouse selections
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD previousMode = 0;
	GetConsoleMode(hInput, &previousMode);
	SetConsoleMode(hInput, previousMode & ENABLE_EXTENDED_FLAGS);

	// Remove cursor (The blinking boi)
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(hOutput, &curInfo);
	curInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hOutput, &curInfo);


	// Resize the console buffer, removes the scrollbar
	// get handle to the console window
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// retrieve screen buffer info
	CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
	GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);

	// current window size
	short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
	short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;

	// current screen buffer size
	short scrBufferWidth = scrBufferInfo.dwSize.X;
	short scrBufferHeight = scrBufferInfo.dwSize.Y;

	// to remove the scrollbar, make sure the window height matches the screen buffer height
	COORD newSize{};
	newSize.X = scrBufferWidth;
	newSize.Y = winHeight;

	// set the new screen buffer dimensions
	int Status = SetConsoleScreenBufferSize(hOut, newSize);

	// print the current screen buffer dimensions
	GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);

	GetWindowRect(consoleHWND, &consoleWindowRect);
	EnableWindow(consoleHWND, false);

}



int main() {


	// Important variable declaration and Console setup
	// Turns console into borderless, scrollbar-less, unselectable, cursorless window
	HWND consoleHWND = GetConsoleWindow();
	RECT consoleWindowRect{0};
	HDC mydc = GetDC(consoleHWND);
	COLORREF COLOR = RGB(255, 255, 255);
	consoleSetup(consoleHWND, consoleWindowRect);
	int consoleHeight = consoleWindowRect.bottom - consoleWindowRect.top;
	int consoleWidth = consoleWindowRect.right - consoleWindowRect.left;

	#pragma region Step 1: Entry
	// Get user input for mode
	// a -> Black and white
	// b -> Colored lines
	// c -> Colored sectors
	// d -> Generate and write list of numbers and their factors generated
	char mode = getMode();
	#pragma endregion

	// Step 2: Check file

	// Declare filestream and necessary variables, and fileExists check.
	std::fstream myfile;
	std::vector<std::string> row;
	std::string line, numberAsString;
	int fileLines = 0;
	std::pair<int, int> factorPair;
	constexpr int upperBound = 2'100'000;

	myfile.open("factorTable.csv", std::ios::in);
	bool fileExists = myfile.is_open();

	if (fileExists) {
		std::getline(myfile, line);
		if (line != "          " && line != "") {
			fileLines = stoi(line);
		}
	}
	myfile.close();

	// Step 3: Maybe writing to file
	//		if 'f' && no file -> step 3fn1: create file
	//						 	 step 3fn2: write to file
	//							 step 3fn3: write stopping point as file header
	//		if 'f' &&	 file -> step 3fy1: get stopping point as file header
	//							 step 3fy2: ask to use file
	//							 step 3fy3: read from file until stopping point.
	//
	// Step 4: Maybe drawing
	//		if userPermitsFile -> step 4.05: Loop through file first
	//							   step 4.1: Loop the rest with calculations

	if (mode == 'f') {
		// Writing to file
		if (!fileExists) {
			myfile.open("factorTable.csv", std::ios::out);
			writeToFile(upperBound, myfile);
			myfile.close();
		} else {
			clearConsole();
			std::cout << "factorTable.csv already exists with " << fileLines << " digits saved. Overwrite ? (Y / N) ESCAPE to exit.";

			while (true) {
				if (GetKeyState(VK_ESCAPE) & 0x8000)
					exit(0);
				if (GetKeyState('Y') & 0x8000) {
					myfile.open("factorTable.csv", std::ios::out);
					writeToFile(upperBound, myfile);
					myfile.close();
					clearConsole();
					break;
				}
				if (GetKeyState('N') & 0x8000) {
					clearConsole();
					std::cout << "Exiting...";
					Sleep(1000);
					exit(0);
				}
				Sleep(50);
			}
		}
	} else {
		clearConsole();
		// Drawing, potentially using file.
		bool userPermitsFile = false;
		std::pair<int, int> coords;

		if (fileLines) {
			userPermitsFile = askUseFile(fileLines);
		}

		int number = 1;

		if (userPermitsFile) {
			std::cout << "Reading from file...";
			{
				myfile.open("factorTable.csv", std::ios::in);
				std::string incrementLine;
				std::getline(myfile, incrementLine);
			}

			for (; number <= fileLines; number++) {
				coords = getCoordsFromFile(myfile);

				drawPixels(number, coords.first, coords.second, upperBound, mode, consoleHeight, consoleWidth, mydc);
				if (GetKeyState(VK_ESCAPE) & 0x8000)
					break;
			}
		}
		
		std::cout << '\r' << "Calculating numbers...";
		for (; number <= upperBound; number++) {
			coords = calcCoords(number);
			drawPixels(number, coords.first, coords.second, upperBound, mode, consoleHeight, consoleWidth, mydc);
			if (GetKeyState(VK_ESCAPE) & 0x8000)
				break;
		}
	}
		
	std::cout << '\r' << "Done! Press Enter or Escape to exit.";
	Sleep(400);
	while (true) {
		if (GetKeyState(VK_ESCAPE) & 0x8000)
			exit(0);
		if (GetKeyState(VK_RETURN) & 0x8000)
			exit(0);
		Sleep(33);
	}

	// We're done drawing
	ReleaseDC(consoleHWND, mydc);
	return 0;
}
