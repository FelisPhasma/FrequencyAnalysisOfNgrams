// AccumulateDatabase.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

//http://en.cppreference.com/w/cpp/algorithm/lower_bound
template<class ForwardIt, class T, class Compare = std::less<>>
ForwardIt binary_find(ForwardIt first, ForwardIt last, const T& value, Compare comp = {})
{
	// Note: BOTH type T and the type after ForwardIt is dereferenced 
	// must be implicitly convertible to BOTH Type1 and Type2, used in Compare. 
	// This is stricter than lower_bound requirement (see above)

	first = std::lower_bound(first, last, value, comp);
	return first != last && !comp(value, *first) ? first : last;
}
//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string> split(const std::string &text, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

struct worddata {
	std::string word;
	double frequency;
	int rawCount;
};

// Using sort methods from SORT.h
void BottomUpMerge(std::vector<worddata>& a, int iLeft, int iRight, int iEnd, std::vector<worddata>& b)
{
	int i = iLeft;
	int j = iRight;
	for (int k = iLeft; k < iEnd; k++) {
		if (i < iRight && (j >= iEnd || a[i].frequency <= a[j].frequency)) {
			b[k] = a[i];
			i = i + 1;
		}
		else {
			b[k] = a[j];
			j = j + 1;
		}
	}
}
void CopyArray(std::vector<worddata>& b, std::vector<worddata>& a)
{
	int n = b.size();
	for (int i = 0; i < n; i++) {
		a[i] = b[i];
	}
}
void SORT(std::vector<worddata>& a)
{
	int n = a.size();
	std::vector<worddata> b;
	b.resize(n);
	for (int width = 1; width < n; width *= 2)
	{
		// Array A is full of runs of length width.
		for (int i = 0; i < n; i = i + 2 * width)
		{
			// Merge two runs: A[i:i+width-1] and A[i+width:i+2*width-1] to B[]
			// or copy A[i:n-1] to B[] ( if(i+width >= n) )
			BottomUpMerge(a, i, std::min(i + width, n), std::min(i + 2 * width, n), b);
		}
		// Now work array B is full of runs of length 2*width.
		// Copy array B to array A for next iteration.
		// A more efficient implementation would swap the roles of A and B.
		CopyArray(b, a);
		// Now array A is full of runs of length 2*width.
	}
}

const std::string dataPath = "C:/Users/felis/programming/SuperComputing/WordData/data/";
const std::string outputPath = "C:/Users/felis/programming/SuperComputing/WordData/output/";
std::vector<std::string> dictionary;
std::vector<int> counts;
int main()
{
	// Totals
	int totalWords = 0;
	// Dictionary
	std::ifstream dIn(dataPath + "dictionary.txt");
	std::string t;
	while (dIn >> t) {
		std::transform(t.begin(), t.end(), t.begin(), ::tolower);
		dictionary.push_back(t);
		counts.push_back(0);
	}
	dIn.clear();
	dIn.close();
	std::cout << "FINISHED DICTIONARY" << std::endl;
	// Raw data
	std::string row;
	std::ifstream fIn(dataPath + "twitter-1003-rawdata");
	int thecounter = 0;
	while (std::getline(fIn, row))
	{
		if (thecounter % 10000 == 0) {
			std::cout << thecounter << std::endl;
		}
		// Replace \uE032 with a space
		std::replace(row.begin(), row.end(), '\uE032', ' ');
		// Split into 1-grams
		std::vector<std::string> words = split(row, ' ');
		//int index;
		std::string word;
		for (int i = 0, l = words.size(); i < l; i++) {
			word = words[i];
			std::transform(word.begin(), word.end(), word.begin(), ::tolower);
			//index = binsearch(dictionary.begin(), dictionary.end(), words[i]);
			auto it = binary_find(dictionary.begin(), dictionary.end(), word);
			if (it != dictionary.cend()) {
				int index = std::distance(dictionary.begin(), it);
				counts[index]++;
				totalWords++; // I could also just sum up the counts however this is more efficient on cpu
			}
		}
		thecounter++;
	}
	std::cout << "FINISHED RAW" << std::endl;
	// frequencies
	std::vector<worddata> words;
	for (int i = 0, l = dictionary.size(); i < l; i++) {
		if (counts[i] < 5)
			continue;
		dictionary[i][0] = toupper(dictionary[i][0]);
		worddata wdata = {dictionary[i], (double)counts[i] / (double)totalWords, counts[i]};
		words.push_back(wdata);
	}
	SORT(words);
	std::cout << "FINISHED FREQUENCIES" << std::endl;
	// output
	std::ofstream file;
	file.open(outputPath + "TwitterCompiledData.dat", std::ios_base::trunc);
	for (unsigned int i = 0; i < words.size(); i++)
		file << words[i].word << "\t" << words[i].frequency << "\t" << words[i].rawCount << "\n";
	file.close();
	// Output totals
	std::ofstream tfile;
	tfile.open(outputPath + "TwitterCompiledDataTotals.dat", std::ios_base::trunc);
	tfile << totalWords << "\n";
	tfile.close();
	std::cout << "OUTPUT COMPLETED" << std::endl;
    return 0;
}

