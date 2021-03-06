#pragma once

#include <fstream>
#include <iostream>
#include <string>
#ifdef _WIN32
	#include "windows.h"
#endif
#ifdef TARGET_OS_MAC
	#include <unistd.h>
#endif
#define START_FILE_STRING "START.txt"
#define END_FILE_STRING "End.txt"
#define SLEEP_DURATION 20 // typewriter effect duration

using namespace std;

#define MAX_LINES_OF_DESCRIPTION 200

void LocalSleep(int duration)
{
#ifdef _WIN32
	Sleep(duration);
#endif

#ifdef TARGET_OS_MAC
	usleep(duration);
#endif
}

string readFile(string filename) { // prints out description and returns exit options 
	cout << '\n';

	string currentLine = "";
	ifstream RoomFile(filename);
	char myChars[2] = { RoomFile.get() };
	bool sleepIsOn = false;

	if (filename == END_FILE_STRING) {
		return "";
	}
	
	// look for a bracket in the first 200 lines
	for (int i = 0; i <= MAX_LINES_OF_DESCRIPTION; ++i)
	{
		getline(RoomFile, currentLine);
		if (currentLine[0] == '[') {
			RoomFile.clear();
			RoomFile.seekg(0); 
			break;
		}
		if (i == MAX_LINES_OF_DESCRIPTION)
			throw(filename);
	}

	while ((myChars[0] = RoomFile.get()) != '[') {
		if (myChars[0] == '\\')// && (myChars[0] = RoomFile.get()) == '%') // the loop just passed over a '\%'
		{
			myChars[0] = RoomFile.get();
			switch (myChars[0])
			{
			case '%':
				cin.get();
				continue;
			case 't':
				sleepIsOn = !sleepIsOn; // change status of typewriter effect being on or off
				continue;
			default:
				break;
			}
		}
		myChars[1] = myChars[0];
		cout << myChars[0];
		if (sleepIsOn) LocalSleep(SLEEP_DURATION);

	}

	RoomFile.close();
	return currentLine; // currentLine holds the exits as a string
}

/* Cycle through exits string, look for one less comma than user's input*/

string getExit(string exits, int input) { /* get exit from string of exits */
	if (input <= 0)
		throw (input);
	string selectedExit = "";
	for (int i = 1; exits[i] != ']'; ++i) {
		if (exits[i] == ',')
		{
			--input;
			++i;
			if (input > 0)
				selectedExit = "";
		}

		if (input == 0)
		{
			return selectedExit;
		}
		
		selectedExit += exits[i];
	}
	if (input != 1)
		throw(input); // otherwise throw error that input was out of range
	return selectedExit;
}

string getFolder(string& selectedExit, string currentfolder)
{
	string foldername = currentfolder;
	for (int i = selectedExit.length() - 1; i >= 0; --i)
	{
		if (selectedExit[i] == '\\')
		{
			foldername = "";
			for (int n = 0; n < i + 1; ++n)
			{
				foldername += selectedExit[n];
				if (foldername == "PARENTFOLDER")
				{
					foldername = "";
					++n;
				}
			}
		}
	}
	return foldername;
}