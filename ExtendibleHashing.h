#pragma once
#include <cmath>
#include <string>
using namespace std;



#define RECORDSPERBUCKET 2
#define MAXKEYLENGTH 8
#define MAXKEYVALUE (int)(pow(2,MAXKEYLENGTH)-1)

struct DataItem {
	int valid; //0--> INVALID,  1 --> VALID
	int data;  //data value
	int key;   //key value

	DataItem()
	{
		valid = 0;
		data = 0;
		key = 0;
	}

	DataItem(int k, int d) {
		data = d;
		key = k;
		valid = 1;
	}
};

struct Bucket {
	int localDepth;
	int currentEntries;   
	struct DataItem  dataItem[RECORDSPERBUCKET];

	Bucket() {
		currentEntries = 0;
		localDepth = 0;
	}

	Bucket(int depth) {
		currentEntries = 0;
		localDepth = depth;
	}
};

struct GlobalDirectory {
	int globalDepth;
	int length;
	int notUsedVariable;
	Bucket** entry;
	
	GlobalDirectory() {
		globalDepth = 0;
		length = 0;
		entry = { 0 };
		notUsedVariable = 0;
	}

};



void displayItem(DataItem* dataItem);
void displayBucket(Bucket& currentBucket, string& depths, string& values, int verbose);
void displayDirectory(GlobalDirectory& globaldirectory, Bucket& currentBucket, int verbose);

int getBucketHash(int key, int depth);
int getPeerBucketHash(int hashKey, int bitLength);

int  insertItemIntoBucket(Bucket& currentBucket, DataItem data);
void findItemInBucket(Bucket& currentBucket, int key);
int  deleteItemFromBucket(Bucket& currentBucket, int key);

int  insertItem(DataItem data, Bucket& currentBucket, GlobalDirectory& globaldirectory);
void searchItem(int key, Bucket& currentBucket, GlobalDirectory& globaldirectory);
int  deleteItem(int key, Bucket& currentBucket, GlobalDirectory& globaldirectory);

int createFirstTimeDirectory(GlobalDirectory& globaldirectory, Bucket& currentBucket);

int splitBucket(GlobalDirectory &globaldirectory,int splitIndex);
int extendDirectory(GlobalDirectory& globaldirectory, int splitIndex);
int checkDirectoryMinimization(GlobalDirectory& globaldirectory);


