/*
 * runDijkstra.cpp
 *
 *  Created on: Nov 8, 2017
 *      Author: scott
 */
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include "graph.hpp"

using namespace std;

int main () {
	string graphFile, starting_id, pathFile;
	ifstream inFile;
	ofstream outFile;
	Graph Dijkstra(10);
	chrono::duration<double> elapsed_seconds;

	while (!inFile.is_open ()) {
		cout << "Enter name of graph file: ";
		cin >> graphFile;
		inFile.open (graphFile); //ios::in set for default
	}
	Dijkstra.readFile(inFile);
	do{
		cout << "Enter a valid vertex id for the staring vertex:";
		cin >> starting_id;
	}while(!Dijkstra.findVertex (starting_id));//break until we have a good start
	//Timing Dijkstra
	auto start =chrono::system_clock::now();
	Dijkstra.runDijkstra(starting_id);
	auto end = chrono::system_clock::now();
	elapsed_seconds = end - start;
	cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << elapsed_seconds.count () << endl;
	while (!outFile.is_open ()) { //wait for keyboard interrupt
		cout <<"Enter name of output file: ";
		cin >> pathFile;
		outFile.open (pathFile, ios::out | ios::trunc);
	}
	if(Dijkstra.writeFile (outFile)) cerr<<"Writing Error in Printing Path"<<endl;//Print Path
	// close dangling files
	inFile.close ();
	outFile.close ();
	return 0;
}

