#pragma once
#include <iostream>
using namespace std;

template <class T>
class Storage {
private:
	T static data[100];
	int count;
public:
	Storage();
	bool add(const T& item);
	bool removeByID(int id);
	T* findByID(int id);
	T* getAll();
	int size() const;
};

#include "Storage.cpp"  



