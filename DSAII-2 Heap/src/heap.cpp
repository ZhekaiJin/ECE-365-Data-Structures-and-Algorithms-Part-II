/*
 * heap.cpp
 *
 *  Created on: Oct 13, 2017
 *      Author: scott
 */


#include "heap.hpp"

Heap::Heap(int capacity){
	this->capacity=capacity;
	this->Heapsize=0;
	this->data.resize(capacity+1);
	this->mapping=new hashTable(capacity*2);
}
//parent child calculation
int Heap::left(int index){return 2*index;}

int Heap::right(int index){return 2*index+1;}

int Heap::parent(int index){return index/2;}

void Heap::swap(int a, int b){
	Node Temp=this->data[a];
	this->data[a]=this->data[b];
	this->data[b]=Temp;
	this->mapping->setPointer(this->data[a].id,&this->data[a]);//update final position pointer
	this->mapping->setPointer(this->data[b].id,&this->data[b]);
}

void Heap::percolateUp (int posCur){
	Node Temp=this->data[posCur];
	int Parent=this->parent(posCur);
	while(posCur>1 && (Temp.key)<(this->data[Parent].key)){
		this->data[posCur] = this->data[Parent];
		this->mapping->setPointer(this->data[posCur].id, &this->data[posCur]);//update current parent pointer
		posCur=Parent;
		Parent=this->parent(posCur);
	}
	this->data[posCur]=Temp; //more efficient than swapping by putting Temp only once
	this->mapping->setPointer(this->data[posCur].id,&this->data[posCur]);//update final position pointer
}

int Heap::insert(const std::string &id, int key, void *pv){
	// check prior conditions before inset
	if(this->Heapsize+1>this->capacity) return 1; //could call data.resize() for continuity
	else if (this->mapping->contains(id)) return 2;
	else { 										//cleaned up, insert
		Node Insert;
		Insert.id=id;
		Insert.key=key;
		Insert.pv=pv;
		this->data[++Heapsize]=Insert; //add first, since we leave [0] empty and start with heapsize 0
		this->mapping->insert(id,&(this->data[this->Heapsize]));
		this->percolateUp(this->Heapsize);
		return 0;
	}
}

void Heap::percolateDown (int posCur){
    this->mapping->setPointer(this->data[posCur].id,&this->data[posCur]);//update pointer first
    do{
		int j=0;
		int r=right(posCur);
		if( r<=(this->Heapsize) && this->data[posCur].key>this->data[r].key){//heapsize issue consider later
			int l=left(posCur);
			if(this->data[l].key<this->data[r].key){
				j=l;
			}else{
				j=r;
			}
		}else{
			int l=left(posCur);
			if( l<=(this->Heapsize) && this->data[posCur].key>this->data[l].key){//heapsize issue consider later
				j=l;
			}
		}
		if(j>=1) this->swap(j,posCur);//can be optimize by temp//swap and update
		posCur=j;
	}while(posCur>=1);

}

int Heap::getPos (Node * pn){
	return pn - &this->data[0];
}

int Heap::deleteMin(std::string *pId , int *pKey , void *ppData ){
	if(this->Heapsize==0){
		return 1;
	}else{
		if (pId != NULL) *pId = this->data[1].id;  //pass deleted data value before deletion
		if (pKey != NULL) *pKey = this->data[1].key;
		if (ppData != NULL) *(static_cast <void**> (ppData)) = this->data[1].pv;
		this->mapping->remove(this->data[1].id);//lazily deletion
		this->data[1] = this->data[this->Heapsize--];
		this->percolateDown(1);
		return 0;
	}
}

int Heap::setKey(const std::string &id, int key){
	if(!this->mapping->contains(id)){
		return 1;
	}else{
		Node* pn =static_cast<Node *>(this->mapping->getPointer(id));
		int tempkey=pn->key;
		pn->key=key;
		int index= this->getPos(pn);
		if (key > tempkey){
			this->percolateDown(index);
		}else if(key < tempkey){
			this->percolateUp (index);
		}
		return 0;
	}
}

int Heap::remove(const std::string &id, int *pKey , void *ppData ){
	if(!this->mapping->contains(id)) {
		return 1;
	}else {
		Node* pn= static_cast<Node *>(this->mapping->getPointer (id));
		if (pKey != nullptr) *pKey = pn->key;
		if (ppData != nullptr) *(static_cast <void**> (ppData)) = pn->pv;
		int tempkey= pn->key;
		this->mapping->remove (id);
		*pn = this->data[this->Heapsize--]; //put last Node into deleted position
		int newKey = pn->key;				//recall setkey seems not workable here
		int index=this->getPos(pn);
		if (tempkey < newKey) {
			this->percolateDown (index);
		}else if (tempkey > newKey) {
			this->percolateUp (index);
		}
		return 0;
	}

}
