#include <vector>
#include <algorithm> 
#include <cctype>
#pragma once
struct inputData {
	std::string name;
	double frequency;
	int rawcount;
	unsigned long long rawtotal;
};
class SORT
{
public:
	SORT(std::vector<inputData> &a);
	static void doAlphaSort(std::vector<inputData>&a);
	static bool Verify(std::vector<inputData> &a);
	static bool AlphaVerify(std::vector<inputData> &a);
	static void str_lower(std::string & s);
	static std::string str_lower_r(std::string s);
private:
	void BottomUpMerge(std::vector<inputData>& a, int iLeft, int iRight, int iEnd, std::vector<inputData>& b);
	static void CopyArray(std::vector<inputData> &b, std::vector<inputData> &a);
	static void a_BottomUpMerge(std::vector<inputData>& a, int iLeft, int iRight, int iEnd, std::vector<inputData>& b);
	static bool initialValidation(std::string a);
};

