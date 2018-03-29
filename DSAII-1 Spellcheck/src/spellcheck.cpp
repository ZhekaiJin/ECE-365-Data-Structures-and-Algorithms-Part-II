/*
 * spellcheck.cpp
 *
 *  Created on: Sep 21, 2017
 *      Author: scott
 */


#include "hash.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <cctype>

using namespace std;
int EXIT_FAILTURE=1;

void loaddictionary(hashTable *myhash, string mydict);
void spellcheck(hashTable *dict, string myfile, string output);
int main(){
	hashTable* myhash= new hashTable(50000);//according to data set given
	string mydict, myfile,output;
	cout<<"Enter name of dictionary:";
	cin >> mydict;
    clock_t t1, t2;//timing
    t1 = clock();
    loaddictionary(myhash, mydict);
    t2 = clock();
    cout<< "Total time spent (in seconds) loading dictionary: " << ((double) (t2 - t1)) / CLOCKS_PER_SEC << endl;
    cout<<"Enter name of Input file:";
    cin>>myfile;
    cout << "Enter name of output file: ";
    cin >> output;
    t1 = clock();
    spellcheck(myhash, myfile,output);
    	t2 = clock();
    cout << "Total time spent (in seconds) spelling check: " << ((double) (t2 - t1)) / CLOCKS_PER_SEC << endl;
    return 0;
}
void loaddictionary(hashTable *myhash, string mydict){
	ifstream Dictionary;	
	Dictionary.open(mydict);
	if(Dictionary.fail()) {
		cerr<<"ERROR:CANT OPEN INPUT FILE:"<<mydict<<strerror(errno)<< endl;
		exit(EXIT_FAILTURE);
	}  //error handling
	string line;
	while (getline (Dictionary, line)) {
			std::transform (line.begin (), line.end (), line.begin (), ::tolower);
			(*myhash).insert (line);
		}
	Dictionary.close();
}
void spellcheck(hashTable *myhash, string myfile, string output){
	ifstream f_in;
	f_in.open(myfile);
	if(f_in.fail()) {
		cerr<<"ERROR:CANT OPEN INPUT FILE:"<<myfile<<strerror(errno)<< endl;
		exit(EXIT_FAILTURE);
	}
	ofstream f_out;
	f_out.open(output);
	if(f_out.fail()) {
		cerr<<"ERROR:CANT OPEN INPUT FILE:"<<output<<strerror(errno)<< endl;
		exit(EXIT_FAILTURE);
	}
	string line="",validword="";
	char tempchar;
	bool containsNum = false;
	unsigned long linenumber=0;
	while (getline(f_in,line)){
		int i = 0;
		linenumber++;
		while (i <= line.size()){
			tempchar = tolower(line[i++]);
			if(isdigit(tempchar)) containsNum=true;
			if(isalnum(tempchar)||(tempchar=='-')||(tempchar=='\'')) {
				validword+=tempchar; //operator overlad
				//cout<<"validword="<<validword<<endl;
			}else if(validword.size()!=0 && containsNum==0) { //seperator encouters
				if (validword.size() > 20){
					f_out << "Long word at line " << linenumber << ", starts: "<< validword.substr(0,20) << endl;
				}else if (!myhash->contains(validword)){
					f_out << "Unknown word at line " << linenumber << ": " << validword << endl;
					}
				validword=""; //clear word
				//cout<<"cleared"<<endl;
			}else{
				validword = "";
				containsNum = false;
			}
		}
	}
	f_in.close();
	f_out.close();
}





