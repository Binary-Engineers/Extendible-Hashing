#include <iostream>
#include "ExtendibleHashing.h"




int main() {
    GlobalDirectory globalDirectory;
    Bucket initialFile;


    displayDirectory(globalDirectory, initialFile, 1);
	//============================================================
    insertItem(DataItem(13, 33), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);

	//============================================================
    searchItem(13, initialFile, globalDirectory);
    deleteItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);

	//============================================================
    insertItem(DataItem(1, 20), initialFile, globalDirectory);
    insertItem(DataItem(2, 70), initialFile, globalDirectory);
    insertItem(DataItem(42, 11), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);

	//============================================================
    insertItem(DataItem(112, 54), initialFile, globalDirectory);
    insertItem(DataItem(240, 16), initialFile, globalDirectory);
	insertItem(DataItem(241, 99), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);

	//============================================================
	insertItem(DataItem(159, 30), initialFile, globalDirectory);
	displayDirectory(globalDirectory, initialFile, 0);

	//============================================================
    insertItem(DataItem(14, 45), initialFile, globalDirectory);
    insertItem(DataItem(13, 88), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);

	//============================================================
    insertItem(DataItem(37, 1), initialFile, globalDirectory);
    searchItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);

	//============================================================
    deleteItem(14, initialFile, globalDirectory);
    deleteItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);

	//============================================================
    insertItem(DataItem(158, 5), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    insertItem(DataItem(3, 9), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);

    return 0;
}


