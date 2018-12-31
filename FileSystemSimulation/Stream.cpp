//
//  Stream.cpp
//  File System Simulation
//
//  Created by JIN ZHAO on 12/30/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//
#include "Common.h"
#include "Stream.h"

ifstream fin;
ofstream fout;



void openFile() {
    //MARK: 1.Open disk.txt
    fin.open("disk.txt", ios::in | ios::binary);
    fout.open("disk.txt", ios::out | ios::binary);
    if (!fin.is_open()) {
        cout << "Fail to read file from disk" << endl;
        return;
    }
}

void closeFile() {
    fin.close();
    fout.close();
}

void format() {
    char str='\0';
    openFile();
    for (int i = 0; i < 51301; i++) {
        fout.write(&str, 1);
    }
    closeFile();
}
