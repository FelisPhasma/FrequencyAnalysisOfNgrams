// CompileLiterature.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>
#include <windows.h>
#include <iterator>
#include <mutex>
#include <algorithm>
#include <stdio.h>
#include <iomanip>
#include "SORT.h"
// 0 = No verification
// 1 = Verify using part of speach tagging and vowel counts
// 2 = Dictionary
#define VERIFICATIONMETHOD 2

// Prefix of the imput files
const std::string FILEPREF = "googlebooks-eng-all-1gram-20120701-";
// Input directory
const std::string PATH = "C:/Users/felis/programming/SuperComputing/WordData/data/";
// Output directory
const std::string PATHOUT = "C:/Users/felis/programming/SuperComputing/WordData/output/";
// Points of interest... Files to preprocess.
const std::string POIs = "abcdefghijklmnopqrstuvwxyz";
// Store length
const int POIs_len = POIs.length();
// Store current index
int POI_I = 0;

// assignment lock
std::mutex assLock;
// Number of processing threads
const int NUMTHREADS = 7;
// Keep track of threads
std::vector<std::thread> threads(NUMTHREADS);
// Coordinate the threads
class threadCoordinator {
public:
	static char getAssignment() {
		if (POI_I == POIs_len) {
			// We have reached the end, finally. Tell the threads to die quietly.
			return '-';
		}
		// Return the current point of interest, increment afterwords
		return POIs[POI_I++];
	}
};

// Verify is a word is valid
#if VERIFICATIONMETHOD == 1
const std::string tabooEndings[] = { "NOUN", "VERB", "ADJ", "ADV", "PRON", "DET", "ADP", "NUM", "CONJ", "PRT", "ROOT", "END", "START" };
const unsigned int tabooEndingsLength = 13;
bool hasEnding(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}
void replaceEnding(std::string& word, std::string& ending) {
	word = word.substr(0, word.length() - ending.length());
}
#elif VERIFICATIONMETHOD == 2
// Dictionary bank
std::vector<std::string> dictionary;
#endif
bool isValid(std::string& word) {
#if VERIFICATIONMETHOD == 0
	// No verification
	return true;
#elif VERIFICATIONMETHOD == 1
	// Verify using endings and vowel counts
	for (int i = 0; i < tabooEndingsLength; i++)
		if (hasEnding(word, "_" + tabooEndings[i]))
			return false; // Endings removed in initial data parse
						  // Verify based off of vowels
	return hasVowels(word);
#elif VERIFICATIONMETHOD == 2
	// Verify off of the dicitonary
	return std::binary_search(dictionary.begin(), dictionary.end(), word);
#endif
	// Should never happen, but it makes c++ happy
	return false;
}

/*struct inputData {
	std::string name;
	double frequency;
};*/
std::vector<inputData> database;
std::mutex datalock;

void MAINTHREAD(std::string name, int delay) {
	// Avoid cordination issues by staggering
	Sleep(delay * 100);
	std::cout << "INITIALIZING THREAD " << name << std::endl;
	
	// Until we have processed every file, get a new one an process it
	while (true) {
		assLock.lock();
		char letterScan = threadCoordinator::getAssignment();
		assLock.unlock();
		if (letterScan == '-') {
			break;
		}
		std::clock_t start = std::clock();
		std::cout << "INITIALIZING " << name << " scanning " << FILEPREF << letterScan << "-precomp" << std::endl;

		// Queue
		std::vector<inputData> queue;
		// Process the file line by line
		inputData data;
		std::ifstream fIn(PATH + FILEPREF + letterScan + "-precomp");
		while (fIn >> data.name >> data.frequency >> data.rawcount >> data.rawtotal) {
			if (isValid(data.name)) {
				queue.push_back(data);
			}
		}
		fIn.close();
		datalock.lock();
		std::copy(queue.begin(), queue.end(), std::back_inserter(database));
		datalock.unlock();
		std::cout << name << " FINISHED PROCESSING " << FILEPREF << letterScan << " in " << (std::clock() - start) / CLOCKS_PER_SEC << "s" << std::endl;
	}
	std::cout << name << " ending - no more assignments" << std::endl;
	return;
}
int main()
{
	// Dictionary verification
	std::cout << "INITIALIZING DICTIONARY PROCESS" << std::endl;

	std::ifstream dIn(PATH + "dictionary.txt");
	std::string t;
	while (dIn >> t) {
		dictionary.push_back(t);
	}
	dIn.clear();
	dIn.close();
	std::cout << "DICTIONARY  WITH " << dictionary.size() << " WORDS" << std::endl;
	// Assign threads
	std::cout << "INITIALIZING THREADS" << std::endl;
	for (int i = 0; i < NUMTHREADS; i++) {
		threads[i] = std::thread(MAINTHREAD, "T0x" + std::to_string(i), i);
	}
	// Wait for threads. No thread left behind!
	for (int i = 0, l = threads.size(); i < l; i++) {
		threads[i].join();
	}
	// Sort
	SORT::SORT(database);
	// Output
	std::ofstream file;
	file.open(PATHOUT + "NgramsCompiledData.dat", std::ios_base::trunc);// std::ofstream::out | std::fstream::app);
	for (unsigned int i = 0; i < database.size(); i++)
		file << database[i].name << "\t" << database[i].frequency << "\t" << database[i].rawcount << "\t" << database[i].rawtotal << "\n";
	file.close();
	std::cout << "OUTPUT COMPLETED" << std::endl;
    return 0;
}
