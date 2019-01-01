//
//  Block.cpp
//  File System Simulation
//
//  Created by JIN ZHAO on 12/30/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//
#include "Common.h"
#include "Block.h"
#include "Stream.h"
#include "File.h"

extern vector<MFD> UserList;
extern vector< vector<UFD> > FileList;
extern vector< vector<UOF> > StateList;
extern vector< Cluster> ClusterList;

extern MFD UserInput;
extern UFD FileInput;
extern UOF StateInput;
extern Cluster ClusterInput;
extern string currentUserName;
extern int currentUserId;
extern char* buffer;

extern fstream fio;
extern char * buffer;

char* readBlock(int blockNum) {
    openFile();
    fio.seekg(blockNum * 513, ios_base::beg);
    fio.read(buffer, 512);    //read函数
    closeFile();
    return buffer;
}

void writeBlock(int blockNum) {
    openFile();
    fio.seekp(blockNum * 513, ios_base::beg);
    if (blockNum == 0) {
        for (int i = 0; i < UserList.size(); i++) {
            fio.write((char*)&(UserList[i]), sizeof(UserList[i]));
        }
    }
    else if (blockNum >= 1 && blockNum <= 16) {
        for (int i = 0; i < FileList[currentUserId].size(); i++) {
            fio.write((char*)&((FileList[currentUserId])[i]), sizeof((FileList[currentUserId])[i]));
            fio.seekp(32 - sizeof((FileList[currentUserId])[i]), ios_base::cur);
        }
    }
    else if (blockNum >= 17 && blockNum <= 32) {
        for (int i = 0; i < StateList[currentUserId].size(); i++) {
            fio.write((char*)&((StateList[currentUserId])[i]), sizeof((StateList[currentUserId])[i]));
            fio.seekp(32 - sizeof((StateList[currentUserId])[i]), ios_base::cur);
        }
    }
    else if (blockNum >= 33 && blockNum <= 99) {
        fio.write((char*)&(ClusterList[blockNum - 33]), sizeof(ClusterList[blockNum - 33]));
    }
    closeFile();
    
}
