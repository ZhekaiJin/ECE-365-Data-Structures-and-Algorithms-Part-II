/*
 * graph.hpp
 *
 *  Created on: Nov 8, 2017
 *      Author: scott
 */
#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include "hash.h"
#include "heap.hpp"

class Graph {
	public:
		Graph (long const & capacity);
		//read&write
		int readFile (std::ifstream & inFile);
		int insert (std::string const & vertex_1, std::string const & vertex_2, long dist);
		bool findVertex (std::string const & Vertex);//locate Vertex && update

		void runDijkstra (std::string const & starting_id) ;

		int writeFile (std::ofstream & outFile); //PrintPath

	private:
		const int INF=1000000000; //setting up infinity with one billion promise but also matching Heap setting
		long capacity;
		struct Vertex;
		typedef struct Edge {
			unsigned long cost;  //change if Bellman–Ford && less than 10^9 promise
			Vertex * dest;
		} Edge;

		typedef struct Vertex {  		//Textbook Implementation
				std::list<Edge> adj; 	//Adjacency list<->Linked list
				bool known;
				long dist;
				std::list<std::string> path;
				std::string ID;
		} Vertex;
		std::list<Vertex *> VertexList; //Linked list
		hashTable * VertexID; // pointers to the node (in list of nodes)
		void update(std::string const & vertex, Vertex* &vertex_ptr);

};

#endif /* GRAPH_HPP_ */
