// Precompile program.
// Precompiles the google raw data and then outputs a procompiled file.
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
// 0 = No verification
// 1 = Verify using part of speach tagging and vowel counts
// 2 = Dictionary
#define VERIFICATIONMETHOD 0

// Prefix of the imput files
const std::string FILEPREF = "googlebooks-eng-all-1gram-20120701-";
// Input directory
const std::string PATH = "C:/Users/felis/programming/SuperComputing/WordData/data/";
// Output directory
const std::string PATHOUT = "C:/Users/felis/programming/SuperComputing/WordData/output/";
// Points of interest... Files to preprocess.
const std::string POIs = "0";// "abcdefghijklmnopqrstuvwxyz";
// Store length
const int POIs_len = POIs.length();
// Store current index
int POI_I = 0;

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
#endif

struct inputData {
	std::string name;
	int year;
	double occurences;
	double volcount;
};
struct worddata {
	std::string name;
	double ussage;
	int rawcount;
	unsigned long long rawtotal;
};
struct totalCount {
	int year;
	double matches;
	double pages;
	double volumes;
};
// Dictionary bank
std::vector<std::string> dictionary;
// Total counts bank, allocates for all years after 0 BCE to 2009 BCE
totalCount totals[2009];

// assignment lock
std::mutex assLock;
// Number of processing threads
const int NUMTHREADS = 1;
// Keep track of threads
std::vector<std::thread> threads(NUMTHREADS);
// ...
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

// Helper methods
std::string str_lower_r(std::string s)
{
	std::string _s = s;
	std::transform(_s.begin(), _s.end(), _s.begin(), ::tolower);
	return _s;
}

// Verify is a word is valid
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
	// Should never happen, but it makes 
	return false;
}

void MAINTHREAD(std::string name, int delay) {
	// Avoid cordination issues by staggering
	Sleep(delay * 100);
	std::cout << "INITIALIZING THREAD " << name << std::endl;
	// Create a bank of data which we will output to the preprocess file
	std::vector<worddata> localWordBank;
	// Until we have processed every file, get a new one an process it
	while (true) {
		// Setup to pre process file
		std::string currentWord = "";
		//double currentWordInfo[2] = { 0, 0 }; // Mean, years
		std::vector<double> frequencies;
		int previousRecentCount;
		unsigned long long previousRecentTotal;
		// Get assignment
		assLock.lock();
		char letterScan = threadCoordinator::getAssignment();
		assLock.unlock();
		if (letterScan == '-') {
			break;
		}
		std::clock_t start = std::clock();
		std::cout << "INITIALIZING " << name << " scanning " << FILEPREF << letterScan << std::endl;

		// Process the file line by line
		inputData data;
		std::ifstream fIn(PATH + FILEPREF + letterScan);
		while (fIn >> data.name >> data.year >> data.occurences >> data.volcount) {
			// Only check the validity of a word once to minimize the cpu-intensive process
			if (data.name != currentWord) {
				if (currentWord != "") {
					if (isValid(currentWord)) {
						int N = frequencies.size();
						double Mu;
						if (N > 1) {
							Mu = frequencies[N - 1];
							double k = 2.0 / ((double)N + 1.0);
							for (int i = N - 2; i >= 0; i--) {
								Mu = (double)((frequencies[i] * k) + (Mu * (1 - k)));
							}
						}
						else {
							Mu = frequencies[0];
						}
						worddata wdata = { currentWord, Mu, previousRecentCount, previousRecentTotal };
						localWordBank.push_back(wdata);
					}
				}
				frequencies.clear();
				currentWord = data.name;
			}
			previousRecentCount = data.occurences;
			previousRecentTotal = totals[data.year].matches;
			frequencies.push_back((double)data.occurences / (double)totals[data.year].matches);
		}
		fIn.close();
		// Check again
		if (isValid(currentWord)) {
			double N = frequencies.size();
			double Mu;
			if (N > 1) {
				Mu = frequencies[N - 1];
				double k = 2 / (N + 1);
				double one = 1;
				for (int i = N - 2; i >= 0; i--) {
					Mu = (frequencies[i] * k) + (Mu * (1 - k));
				}
			}
			else {
				Mu = frequencies[0];
			}
			//frequencies.clear();
			worddata wdata = { currentWord, Mu, previousRecentCount, previousRecentTotal };
			localWordBank.push_back(wdata);
		}
		
		std::cout << name << " FINISHED PROCESSING " << FILEPREF << letterScan << " in " << (std::clock() - start) / CLOCKS_PER_SEC << "s" << std::endl;

		// Output the procompiled
		std::ofstream file;
		file.open(PATH + FILEPREF + letterScan + "-precomp", std::ios_base::trunc);// std::ofstream::out | std::fstream::app);
		for (unsigned int i = 0; i < localWordBank.size(); i++)
			file << localWordBank[i].name << "\t" << localWordBank[i].ussage << "\t" << localWordBank[i].rawcount << "\t" << localWordBank[i].rawtotal << "\n";
		file.close();
		std::cout << "OUTPUT COMPLETED" << std::endl;

		// Clear the bank. Memory leaks would be horrible.
		localWordBank.clear();
	}
	std::cout << name << " ending - no more assignments" << std::endl;
	
	//return;
}
int main()
{
	// Init
	std::clock_t masterStart = std::clock();
	std::cout << "INIT" << std::endl;
	// Do total counts
	std::cout << "INITIALIZING TOTALS" << std::endl;
	std::ifstream tIn(PATH + "googlebooks-eng-all-totalcounts-20120701.dat");
	int year;
	double matches;
	double pages;
	double volumes;
	totalCount tc;
	while (tIn >> year >> matches >> pages >> volumes) {
		tc = { year, matches, pages, volumes };
		totals[year] = tc;
	}
	tIn.clear();
	tIn.close();
	std::cout << "TOTALS PROCESSED IN " << double(std::clock() - masterStart) / CLOCKS_PER_SEC << "s" << std::endl;
#if VERIFICATIONMETHOD == 2
	// Dictionary verification
	std::cout << "INITIALIZING DICTIONARY PROCESS" << std::endl;

	std::ifstream dIn(PATH + "dictionary.txt");
	std::string t;
	while (dIn >> t) {
		dictionary.push_back(t);
	}
	dIn.clear();
	dIn.close();
	std::cout << "DICTIONARY PROCESSED IN " << double(std::clock() - masterStart) / CLOCKS_PER_SEC << "s WITH " << dictionary.size() << " WORDS" << std::endl;
#endif
	// Begin crunching
	std::cout << "INITIALIZING THREADS" << std::endl;
	for (int i = 0; i < NUMTHREADS; i++) {
		threads[i] = std::thread(MAINTHREAD, "T0x" + std::to_string(i), i);
	}
	// Wait for threads. No thread left behind!
	for (int i = 0, l = threads.size(); i < l; i++) {
		threads[i].join();
	}
	// Wrap it all up.
	std::cout << "ALL THREADS FINISHED" << std::endl;
	std::clock_t end = std::clock();
	double elapsed_secs = double(end - masterStart) / CLOCKS_PER_SEC;
	std::cout << "Program completed in " << elapsed_secs << "s" << std::endl;
	int i;
	std::cin >> i;
	return 0;
}

