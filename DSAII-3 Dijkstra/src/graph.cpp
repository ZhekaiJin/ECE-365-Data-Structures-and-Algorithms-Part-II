/*
 * graph.cpp
 *
 *  Created on: Nov 8, 2017
 *      Author: scott
 */

#include <limits>
#include <iterator>
#include "graph.hpp"

Graph::Graph (long const & capacity){
	this->capacity=0;
	this->VertexID=new hashTable(capacity*2); //old rules still applies
}
int Graph::readFile (std::ifstream & inFile) {
	std::string vertex_1, vertex_2;
	long dist;
    while (inFile>>vertex_1>>vertex_2>>dist){
		this->insert (vertex_1, vertex_2, dist);
	}
	return 0;
}
void Graph::update(std::string const & vertex, Vertex* &vertex_ptr){
	if (this->findVertex (vertex)) {
		vertex_ptr = (Vertex *) this->VertexID->getPointer (vertex);
	}else{
		vertex_ptr = new Vertex();  //let default list constructor initilize it for me
		vertex_ptr->ID=vertex;
		vertex_ptr->known=false;
		vertex_ptr->dist=INF;
		this->VertexList.push_back (vertex_ptr); 		// insert new Vertex at end of VertexList
		this->VertexID->insert(vertex, vertex_ptr); // update hashTable
		this->capacity++;
	}
	return;
};

int Graph::insert (std::string const & vertex_1, std::string const & vertex_2, long dist){//processing the linefeed
	Vertex* Vp1 =nullptr,* Vp2=nullptr;
	Edge edge;
	this->update(vertex_1,Vp1);
	this->update(vertex_2,Vp2);
	edge.cost = dist;
	edge.dest = Vp2;
	Vp1->adj.push_back (edge); //update V1->V2
	return 0;
}

bool Graph::findVertex (std::string const & Vertex){          //locate Vertex && update
	return this->VertexID->contains(Vertex);
}

void Graph::runDijkstra (std::string const & starting_id){
	Vertex * start = (Vertex *) this->VertexID->getPointer(starting_id); //getting Pointer position
	start->dist = 0;
	start->path.push_back (starting_id);
	Heap	 binaryHeap (this->capacity); //get a Heap on stack
	binaryHeap.insert(starting_id,start->dist,start); //value key *ptr
	for (std::list<Vertex *>::const_iterator it = this->VertexList.begin (), end = this->VertexList.end (); it != end; ++it) {
		if ((*it)->ID != starting_id) binaryHeap.insert((*it)->ID, (*it)->dist, *it);
	}
	Vertex * waited;
	for (int i = 0; i < this->capacity; ++i) {
		binaryHeap.deleteMin (NULL, NULL, &waited); //should stay nullptr for c++11 &&Extracting smallest distance
		for (std::list<Edge>::const_iterator it = waited->adj.begin (), end = waited->adj.end (); it != end && waited->dist!=INF; ++it) {
			//inf as end condition since Priority Queue implementation
			//now update the unknown && shorter path
			if (!it->dest->known && it->dest->dist > (it->cost + waited->dist)) { // v.dist + cvw < w.dist
				// update the path leading to it
				it->dest->path.clear();
				it->dest->path.insert (it->dest->path.begin (), waited->path.begin (), waited->path.end ());
				it->dest->path.push_back (it->dest->ID);
				// update its distance and key in the priority queue
				it->dest->dist = (it->cost + waited->dist);
				binaryHeap.setKey (it->dest->ID, (it->cost + waited->dist));//setkey to v.dist + cvw
			}
		}
		waited->known = true;
	}
}

int Graph::writeFile (std::ofstream & outFile){ //PrintPath
	for (std::list<Vertex *>::const_iterator it = this->VertexList.begin (), end = this->VertexList.end (); it != end; ++it) {
		outFile << (**it).ID << ": ";
		if ((**it).dist == INF) {
			outFile << "NO PATH" << std::endl;
		}else{
			outFile << (*it)->dist << " [";
			for (std::list<std::string>::const_iterator ti = (*it)->path.begin(), last = (*it)->path.end (); ti != last; ++ti) {
				outFile << *ti;
				if (std::next (ti, 1) != last) outFile << ", "; //get a peek to see if next is last
			}
			outFile << "]" << std::endl;
		}
	}
	return 0;
}


