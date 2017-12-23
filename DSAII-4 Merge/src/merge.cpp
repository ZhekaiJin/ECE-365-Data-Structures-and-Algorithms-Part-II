/*
 * merge.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: scott
 */
#include <iostream>
#include <fstream>
#include <cerrno>

using namespace std;

bool Merge[10001][10001];
int main(){
	int i,j;
	string fIn, fOut, A, B, C;
	ifstream inFile;
	ofstream outFile;
	cout << "Enter name of input file: ";
	cin >> fIn;
	cout << "Enter name of output file: ";
	cin >> fOut;
	inFile.open (fIn);
	outFile.open (fOut);
	if (inFile.is_open () && outFile.is_open ()) {
		while (inFile >> A && inFile >> B && inFile >> C) {
			if ( ( (A.length () + B.length()) != C.length () )||((C[C.length ()-1]!=A[A.length ()-1]) && (C[C.length()-1]!=B[B.length ()-1]))){
				outFile << "*** NOT A MERGE ***" << endl;
				continue;
			}
			//******************The Algorithm Implementation********************
			//merge[i][j] will be true if C[0..i+j-1]is an interleaving of A[0..i-1] and B[0..j-1].
			//BASE
			for (i = 0; i <= B.length (); ++i) {
				for (j = 0; j <= A.length (); ++j) {
					if (i == 0 && j > 0) {
						if (C[j - 1] == A[j - 1]) {
							Merge[i][j] = (j == 1) ? true : Merge[i][j - 1]; //now that this bit match follow last decision
							continue;
						}
					} else if (j == 0 && i > 0) {
						if (C[i - 1] == B[i - 1]) {
							Merge[i][j] = (i == 1) ? true : Merge[i - 1][j];
							continue;
						}
					}
					Merge[i][j] = false;
				}
			}
			//SUBPROBLEMS:if current letter match, last decision floows
			for (i = 1; i <= B.length(); ++i) {
				for (j = 1; j <= A.length(); ++j) {
					Merge[i][j] = false;
					if (Merge[i - 1][j] || Merge[i][j - 1]) {
						if (B[i - 1] == A[j - 1] && C[i + j - 1] == A[j - 1]) {
							Merge[i][j] = true;
						} else if (Merge[i - 1][j]) {
							if (C[i + j - 1] == B[i - 1]) Merge[i][j] = true;
						} else if (Merge[i][j - 1]) {
							if (C[i + j - 1] == A[j - 1]) Merge[i][j] = true;
						}
					}
				}
			}
			// output results
			if (Merge[B.length()][A.length()]==1) {
				i = B.length ();
				j = A.length ();

				while (j > 0 && i >= 0) {
					if (Merge[i][j] && (i == 0 || !Merge[i - 1][j])) {
						C[i + j - 1] = toupper (C[i + j - 1]); //Match A but not B
						--j;
					} else --i;
				}

				outFile << C << endl;

			} else outFile << "*** NOT A MERGE ***" << endl;
		}
		// *****************The Algorithm Implementation**********************
	}else{
		cerr << "ERROR: Failed to open file!" <<strerror(errno)<< endl;
	}
	inFile.close();
	outFile.close();
	return 0;
}
