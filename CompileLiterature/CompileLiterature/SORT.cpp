#include "stdafx.h"
#include "SORT.h"

SORT::SORT(std::vector<inputData>& a)
{
	int n = a.size();
	std::vector<inputData> b;
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

void SORT::doAlphaSort(std::vector<inputData>& _a)
{
	std::vector<inputData> a;
	inputData __a;
	for (int i = 0, l = _a.size(); i < l; i++) {
		__a = _a[i];
		if (initialValidation(__a.name))
			a.push_back(__a);
	}

	int n = a.size();
	std::vector<inputData> b;
	b.resize(n);
	for (int width = 1; width < n; width *= 2)
	{
		for (int i = 0; i < n; i = i + 2 * width)
		{
			a_BottomUpMerge(a, i, std::min(i + width, n), std::min(i + 2 * width, n), b);
		}
		CopyArray(b, a);
	}
}

bool SORT::Verify(std::vector<inputData>& a)
{
	int n = a.size() - 1;
	for (int i = 0; i < n; i++) {
		if (!(a[i].frequency <= a[i + 1].frequency))
			return false;
	}
	return true;
}

bool SORT::AlphaVerify(std::vector<inputData>& a)
{
	int n = a.size() - 1;
	for (int i = 0; i < n; i++) {
		if (str_lower_r(a[i].name).compare(str_lower_r(a[i + 1].name)) > 0)
			return false;
	}
	return true;
}

void SORT::BottomUpMerge(std::vector<inputData>& a, int iLeft, int iRight, int iEnd, std::vector<inputData>& b)
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

void SORT::CopyArray(std::vector<inputData>& b, std::vector<inputData>& a)
{
	int n = b.size();
	for (int i = 0; i < n; i++) {
		a[i] = b[i];
	}
}

void SORT::a_BottomUpMerge(std::vector<inputData>& a, int iLeft, int iRight, int iEnd, std::vector<inputData>& b)
{
	int i = iLeft;
	int j = iRight;
	for (int k = iLeft; k < iEnd; k++) {
		std::string A(a[i].name);
		std::string B(a[j].name);
		if (i < iRight && (j >= iEnd || A.compare(B) <= 0)) {
			b[k] = a[i];
			i = i + 1;
		}
		else {
			b[k] = a[j];
			j = j + 1;
		}
	}
}
bool SORT::initialValidation(std::string a)
{
	// is fucking ascii
	str_lower(a);
	return a.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos;
}

void SORT::str_lower(std::string & s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

std::string SORT::str_lower_r(std::string s)
{
	std::string _s = s;
	std::transform(_s.begin(), _s.end(), _s.begin(), ::tolower);
	return _s;
}
