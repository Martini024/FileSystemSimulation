//
//  Stream.cpp
//  File System Simulation
//
//  Created by JIN ZHAO on 12/30/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//
#include "Common.h"
#include "Stream.h"

fstream fio;

void openFile() {
    //MARK: 1.Open disk.txt
    fio.open("disk.txt", ios::in | ios::out | ios::binary);
    if (!fio.is_open()) {
        cout << "Fail to read file from disk" << endl;
        return;
    }
}

void closeFile() {
    fio.close();
}

void format() {
    char str='\0';
    int nextBlock = -1;
    openFile();
    fio.seekp(ios_base::beg);
    for (int i = 0; i < 100; i++) {
        if (i >= 17) {
            fio.write((char*)&nextBlock, sizeof(nextBlock));
        }
        else {
            for (int i = 0; i < sizeof(int); i++) {
                fio.write(&str, 1);
            }
        }
        for (int j = 0; j < 512 - sizeof(int); j++) {
            fio.write(&str, 1);
        }
        str = '\n';
        fio.write(&str, 1);
        str = '\0';
    }
    str = '\n';
    fio.write(&str, 1);
    closeFile();
}
