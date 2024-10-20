
#include "ExtendibleHashing.h"
#include <bitset>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//Display not found
void displayNotFound(int key) {
	cout << "\t\t\t key:  " << key << " \t value : \t not found" << endl;
}

//Display one record entry
void displayItem(DataItem* dataItem) {
	if (dataItem != 0 && dataItem->valid) {
		cout << "\t\t\t key: " << bitset<8>(dataItem->key) << "\t value:\t" << dataItem->data << endl;
	} else {
		cout << "\t\t\t key:  ============ \t value:\t empty data" << endl;
	}
}

//Display one Bucket
void displayBucket(Bucket& currentBucket, string& depths, string& values, int verbose) {
	depths.append(to_string(currentBucket.localDepth));
	depths.append(",");
	values.append("[");

	if (verbose)
		cout << "\t\tBucket:\t local depth:" << currentBucket.localDepth << endl;

	if (currentBucket.currentEntries == 0) {
		if (verbose)
			cout << "\t\t\tNo Data yet\n";
		for (size_t i = 0; i < RECORDSPERBUCKET; i++) {
			values.append("null");
			values.append(",");
		}
	} else {
		for (size_t i = 0; i < RECORDSPERBUCKET; i++)
		{
			if (currentBucket.dataItem[i].valid) {
				values.append(to_string(currentBucket.dataItem[i].data));
				values.append(",");
				if (verbose)
					cout << "\t\t\t key: " << bitset<8>(currentBucket.dataItem[i].key) << "\t value:\t" << currentBucket.dataItem[i].data << endl;
			} else {
				values.append("null");
				values.append(",");
				if (verbose)
					cout << "\t\t\t key:  ============ \t value:\t empty data" << endl;
			}

		}
	}

	values.pop_back();
	values.append("]");
}

//Display Directory and it's contents 
void displayDirectory(GlobalDirectory& globaldirectory, Bucket& currentBucket, int verbose) {
	cout << "Directory:\t global depth:" << globaldirectory.globalDepth << endl;

	string values = "(";
	string depths = "(";
	int count = 0;

	if (globaldirectory.length == 0) {
		count++;
		cout << "\tNo Directory yet\n";
		displayBucket(currentBucket, depths, values, verbose);
	} else {

		for (size_t i = 0; i < globaldirectory.length; i++) {
			if (i == 0) {
				count++;
			} else if (globaldirectory.entry[i - 1] != globaldirectory.entry[i]) {
					count++;
			}
			if (verbose)
				cout << "\t key: " << bitset<8>(i) << "\t value:\t" << globaldirectory.entry[i] << endl;
			displayBucket(*globaldirectory.entry[i], depths, values, verbose);
			if (verbose)
				cout << "-----------------------------------------------\n\n";
		}
		depths.pop_back();

	}

	values.append(")");
	depths.append(")");
	cout << " buckets:\t" << count << "/" << globaldirectory.length << endl;
	cout << "values:\t" << values << endl;
	cout << "depths:\t" << depths << endl;
	cout << "=========================\n";
	char t[100];
}



//Get Hash of the bucket related to the given key
int getCurrentHash(int key, int depth) {
	return (key & MAXKEYVALUE) >> (MAXKEYLENGTH - depth);
}

//Get the peer hash of a given bucket hash
int getPeerBucketKey(int hashedKey, int bitLength) {
    string bitString = bitset<32>(hashedKey).to_string();
    bitString = bitString.substr(32 - bitLength, bitLength);

    bitString[bitLength - 1] = (bitString[bitLength - 1] == '1') ? '0' : '1';

    int modifiedKey = stoi(bitString, nullptr, 2);

    return modifiedKey;
}

//Try to insert item into a bucket
int insertItemIntoBucket(Bucket& currentBucket, DataItem data) {
	if ( data.valid == 0 ) {
		return 0;
	}

	if (currentBucket.currentEntries >= RECORDSPERBUCKET) {
		return 0;
	}

	for (size_t i = 0; i < RECORDSPERBUCKET; i++) {
		if (currentBucket.dataItem[i].valid == 0) {
			currentBucket.currentEntries = currentBucket.currentEntries + 1;
			currentBucket.dataItem[i] = data;
			return 1;
		}
	}
	
	return 0;
}


//Look for an item in a bucket using key
void findItemInBucket(Bucket& currentBucket, int key) {
	for (size_t i = 0; i < currentBucket.currentEntries; i++) {
		if ( currentBucket.dataItem[i].key == key ) {
			if ( currentBucket.dataItem[i].valid == 0 ) {
				break;
			}
			displayItem(&currentBucket.dataItem[i]);
			return;
		} 
	}

	displayNotFound(key);

	return;
}

//Try to Delete item based on a key value from a bucket
int deleteItemFromBucket(Bucket& currentBucket, int key) {
	for (size_t i = 0; i < RECORDSPERBUCKET; i++) {
		if ( currentBucket.dataItem[i].key == key ) {
			if (currentBucket.dataItem[i].valid == 0) {
				return 0;
			}
			currentBucket.currentEntries = currentBucket.currentEntries - 1;
			currentBucket.dataItem[i].valid = 0;
			return 1;
		} 
	}

	return 0;
}



//Try to insert item in the file
int insertItem(DataItem data, Bucket& currentBucket, GlobalDirectory& globaldirectory) {
	if ( data.valid == 0 ) {
		return 0;
	}

	if (globaldirectory.globalDepth == 0) {
		if (insertItemIntoBucket(currentBucket, data) == 0) {
			createFirstTimeDirectory(globaldirectory, currentBucket);
		} else {
			return 1;
		}
	}

	int hashedKey =  getCurrentHash(data.key, globaldirectory.globalDepth);

	for (size_t i = 0; i < 5; i++) {
		int n = insertItemIntoBucket(*globaldirectory.entry[hashedKey], data);
		if (n == 0) {
			if (splitBucket(globaldirectory, hashedKey) == 0) {
				return 0;
			}
		} else {
			return 1;
		}
		hashedKey =  getCurrentHash(data.key, globaldirectory.globalDepth);
	}

	return 0;
}

//Search the directory for an item using the key
void searchItem(int key, Bucket& currentBucket, GlobalDirectory& globaldirectory) {
	if (globaldirectory.length == 0) {
		findItemInBucket(currentBucket, key);
		return;
	}

	int hashedKey =  getCurrentHash(key, globaldirectory.globalDepth);
	currentBucket = *globaldirectory.entry[hashedKey];
	findItemInBucket(currentBucket, key);
}

//Search on an item based on the key and delete it
int deleteItem(int key, Bucket& currentBucket, GlobalDirectory& globaldirectory) {
	if (globaldirectory.length == 0) {
		return deleteItemFromBucket(currentBucket, key);
	}

    int hashedKey = getCurrentHash(key, globaldirectory.globalDepth);

    if (deleteItemFromBucket(*globaldirectory.entry[hashedKey], key) == 1) {
        if (globaldirectory.entry[hashedKey]->currentEntries == 0 && globaldirectory.length > 2) {
            int peerHashedKey = getPeerBucketKey(hashedKey, globaldirectory.entry[hashedKey]->localDepth);

            if (globaldirectory.entry[peerHashedKey]->localDepth == globaldirectory.entry[hashedKey]->localDepth) {
                globaldirectory.entry[hashedKey] = globaldirectory.entry[peerHashedKey];
            }
        }

        int minimizationResult = checkDirectoryMinimization(globaldirectory);
        while (minimizationResult != -1) {
            minimizationResult = checkDirectoryMinimization(globaldirectory);
        }

        return 1;
    }

    return 0;
}





//Create  the first directory
int createFirstTimeDirectory(GlobalDirectory& globaldirectory, Bucket& currentBucket) {
	globaldirectory.globalDepth = 1;
	globaldirectory.length = 2;
	globaldirectory.entry = new Bucket * [globaldirectory.length];
	globaldirectory.entry[0] = new Bucket(globaldirectory.globalDepth);
	globaldirectory.entry[1] = new Bucket(globaldirectory.globalDepth);

	for (size_t i = 0; i < RECORDSPERBUCKET; i++) {
		int newKey = getCurrentHash(currentBucket.dataItem[i].key, globaldirectory.globalDepth);

		if (newKey == -1) {
			return -1;
		}
		insertItemIntoBucket(*globaldirectory.entry[newKey], currentBucket.dataItem[i]);
	}

	return 1;
}

//Split a given bucket because a bucket is full
int splitBucket(GlobalDirectory &globaldirectory,int splitIndex) {

	int hashKey;
	int peerKey;
	int end;

	Bucket bucketToSplit = * globaldirectory.entry[splitIndex];

	if (globaldirectory.globalDepth == bucketToSplit.localDepth) {
		return extendDirectory(globaldirectory, splitIndex);
	}

	int bucketRepetitions = 0;
	vector<int> matchingIndices;
	for(int i = 0; i < globaldirectory.length; i++) {
		if (globaldirectory.entry[i] == globaldirectory.entry[splitIndex]) {
			matchingIndices.push_back(i);
			bucketRepetitions++;
		}
	}
	sort(matchingIndices.begin(), matchingIndices.end());

	hashKey = matchingIndices[0];
	peerKey = matchingIndices[bucketRepetitions/2];
	end = matchingIndices[bucketRepetitions-1];

	globaldirectory.entry[hashKey] =  new Bucket(bucketToSplit.localDepth+1);
	globaldirectory.entry[peerKey] = new Bucket(bucketToSplit.localDepth+1);

   for (size_t i = 0; i < globaldirectory.length; i++) {
        if (hashKey <= i && peerKey > i) {
            globaldirectory.entry[i] = globaldirectory.entry[hashKey];
        } else if (peerKey <= i && end >= i) {
            globaldirectory.entry[i] = globaldirectory.entry[peerKey];
        }
    }
	
	for (size_t i = 0; i < bucketToSplit.currentEntries; i++) {
		int newKey = getCurrentHash(bucketToSplit.dataItem[i].key, globaldirectory.globalDepth);
		if (hashKey <= newKey && peerKey > newKey) {
			insertItemIntoBucket(*globaldirectory.entry[splitIndex], bucketToSplit.dataItem[i]);
		} else if (peerKey <= newKey && end >= newKey) {
			insertItemIntoBucket(*globaldirectory.entry[peerKey], bucketToSplit.dataItem[i]);
		}
		else{
			matchingIndices.clear();
			return 0;
		}
	}

	matchingIndices.clear();

	return 1;
}

//Extend global directory because a given bucket can not be split
int extendDirectory(GlobalDirectory &globaldirectory,int splitIndex) {
    if (globaldirectory.globalDepth == 8) {
        return 0;
    }

	Bucket bucketToSplit = * globaldirectory.entry[splitIndex];

	Bucket** oldEntry = globaldirectory.entry;

	globaldirectory.length= globaldirectory.length *2;
	globaldirectory.globalDepth ++;
	globaldirectory.entry = new Bucket * [globaldirectory.length];

	int count = 0;
	for (size_t i = 0; i < globaldirectory.length/2; i++) {
		globaldirectory.entry[i+count] = oldEntry[i];
		globaldirectory.entry[i+1+count] = oldEntry[i];
		count++;
	}

	delete [] oldEntry;

	int peerHashedKey = getPeerBucketKey(splitIndex, globaldirectory.globalDepth);

	globaldirectory.entry[splitIndex] =  new Bucket(bucketToSplit.localDepth+1);
	globaldirectory.entry[peerHashedKey] = new Bucket(bucketToSplit.localDepth+1);
	
	for (size_t i = 0; i < bucketToSplit.currentEntries; i++) {
		int newKey = getCurrentHash(bucketToSplit.dataItem[i].key, globaldirectory.globalDepth);
		cout << "newkey" << newKey << endl;
		if (newKey == splitIndex) {
			insertItemIntoBucket(*globaldirectory.entry[splitIndex], bucketToSplit.dataItem[i]);
		} else if (newKey == peerHashedKey) {
			insertItemIntoBucket(*globaldirectory.entry[peerHashedKey], bucketToSplit.dataItem[i]);
		} else{
			return 0;
		}
	}

	return 1;
}



//Minimize directory if all buckets have depth less than global depth
int checkDirectoryMinimization(GlobalDirectory& globaldirectory) {

	for (size_t i = 0; i < globaldirectory.length; i++) {
		if (globaldirectory.entry[i]->localDepth == globaldirectory.globalDepth)
			return -1;
	}

	if (globaldirectory.length == 1)
		return -1;

	int oldGlobalDepth, oldLength;
	Bucket** oldEntry;

	oldGlobalDepth = globaldirectory.globalDepth;
	oldLength = globaldirectory.length;
	globaldirectory.globalDepth -= 1;
	globaldirectory.length /= 2;
	oldEntry = globaldirectory.entry;
	globaldirectory.entry = new Bucket * [globaldirectory.length];

	for (size_t i = 0; i < globaldirectory.length; i++) {
		globaldirectory.entry[i] = oldEntry[2*i];
	}
	
	delete [] oldEntry;

	return 1;
}