/*
 * hash.cpp
 *
 *  Created on: Sep 21, 2017
 *      Author: scott
 */

#include "hash.h"
#include <iostream>

using namespace std;

hashTable::hashTable(int size){
	this->capacity=getPrime(size);
	this->filled=0;
	hashItem* hashitem=new hashItem();
	hashitem->key="";
	hashitem->isOccupied=false;
	hashitem->pv=nullptr;
	hashitem->isDeleted=false;
	this->data.resize(capacity,*hashitem);
}

unsigned long hashTable::doublehash(const string &key)
{
unsigned int hash = 0;
char const *c=key.c_str();
while (*c++)
    hash += int(*c);
return hash;
}

unsigned long hashTable::hash(const string &key)
{
    unsigned long hash = 5381;
    char const *c=key.c_str();
    while (*c++)
        hash = ((hash << 5) + hash) + (int)*c; /* hash * 33 + c */
    return hash;
}

int hashTable::insert (const string &key, void *pv){
	if(this->findPos(key)<0){
		unsigned long position,jumper;
		position=this->hash(key)%this->capacity;
		jumper=this->doublehash(key)%(this->capacity-1)+1; //+1 to avoid 0 and -1 to avoid jumper=capacity
        //cout<<"position="<<position<<endl;
        //cout<<"jumper="<<jumper<<endl;
		for(int i=0;i<this->capacity;i++){
			position=(position+i*jumper)%(this->capacity);//  h(i,k) = ( h_1(k) + i \cdot h_2(k) ) \mod |T|.
            //cout<<"final position="<<position<<endl;
			hashItem & A =this->data.at(position);
			if (!A.isOccupied){
				A.key=key;
				A.isOccupied=true;
				A.pv=pv;
				this->filled++;
				break;
			}
			else if ((A.isDeleted)&&(A.key==key)){
				A.isDeleted=false;
				break;  //break since checking capacity afterwords;
			}
		}
	}else return 1; //findPos()>0, key already there

	if((float)this->capacity/(float)this->filled<2){
		if(!this->rehash())
			return 2;
	}
	//cout<<"capacity="<<capacity<<endl;
	return 0;
}

bool hashTable::contains(const std::string &key){
	if (this->findPos(key)>= 0){
		return true;
	}else{
		return false;
	}
}


void* hashTable::getPointer(const std::string &key, bool *b ){
	int position = this->findPos(key);
	if (position >= 0) {
		if(b != nullptr)
			*b=true;
		return this->data.at(position).pv;
	} else {
		if (b != nullptr)
			*b = false;
		return nullptr;
	}
}

int hashTable::setPointer(const std::string &key, void *pv){
	int position = this->findPos(key);
	if (position >= 0) {
		this->data.at(position).pv=pv;
		return 0;
	}else return 1;
}

bool hashTable::remove(const std::string &key){
	int position = this->findPos(key);
		if (position >= 0) {
			(this->data.at (position)).isDeleted = true;
			return true;
		} else {
			return false;
		}
}

unsigned int hashTable::getPrime (int size){
	unsigned int prime[]={53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
		24593, 49157, 98317, 196613, 393241, 786433,
		1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189,
		805306457, 1610612741
	};   //prime list from http://planetmath.org
	int i=0;
	for(;i<(sizeof(prime)/sizeof(prime[0]));i++){
		if(prime[i]>=size*2)
			break;
	}
	return prime[i];
}

// Search for an item with the specified key.
  // Return the position if found, -1 otherwise.
int hashTable::findPos(const std::string &key){
	unsigned long jumper, position;
	position=this->hash(key)%this->capacity;
	jumper=this->doublehash(key)%(this->capacity-1)+1;
	for(int i=0;i<this->capacity;i++){
		position=(position+i*jumper)%(this->capacity);   //  h(i,k) = ( h_1(k) + i \cdot h_2(k) ) \mod |T|.
		hashItem & A=this->data.at(position);
		if (A.key==key && !A.isDeleted && A.isOccupied){ //considering key=""
			return position;
		}else if (!A.isOccupied){   //the first unOccupied means no more after it
			return -1;
		}
	}
	return -1;
}
// The rehash function; makes the hash table bigger.
 // Returns true on success, false if memory allocation fails.
bool hashTable::rehash(){
    //cout<<"rehasing"<<endl;
	int old_cap=this->capacity;
	//int new_cap=2*old_cap;
	int new_cap=this->getPrime(old_cap);
	hashItem* nullitem=new hashItem();
	nullitem->key="";
	nullitem->isOccupied=false;
	nullitem->pv=nullptr;
	nullitem->isDeleted=false;
    vector<hashItem> Temp(this->data);
    this->data.resize(0);//clear data set
	this->data.resize(new_cap,*nullitem);
	int c=this->data.size();
	if(!(c==new_cap))return false;
	this->capacity=new_cap;
	for(int i=0; i<old_cap;i++){
		hashItem olditem=Temp.at(i);
		if (!olditem.isDeleted && olditem.isOccupied){
			this->insert(olditem.key,olditem.pv);
		}
	}
	vector<hashItem>().swap(Temp);  //clear memory of Temporary vector
	return 0;
}



