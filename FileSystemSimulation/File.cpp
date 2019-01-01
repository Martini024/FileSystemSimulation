//
//  File.cpp
//  File System Simulation
//
//  Created by JIN ZHAO on 12/30/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//

#include "Common.h"
#include "File.h"
#include "Block.h"
#include "Stream.h"

vector<MFD> UserList;
vector< vector<UFD> > FileList;
vector< vector<UOF> > StateList;
vector<Cluster> ClusterList;
vector<int> FreeBlockList;

MFD UserInput;
UFD FileInput;
UOF StateInput;
Cluster ClusterInput;
string currentUserName = "";
int currentUserId = -1;
char* buffer = new char[512];



void initMFD() {
    buffer = readBlock(0);
    for (int i = 0; i < 16; i++) {
        memmove(&UserInput, buffer + i * sizeof(UserInput), sizeof(UserInput));
        if (UserInput.userName[0] != '\0') {
            UserList.push_back(UserInput);
        }
        else {
            break;
        }
    }
}

void initUFD() {
    vector<UFD> tmpUFD;
    for (int i = 1; i < UserList.size() + 1; i++) {
        FileList.push_back(tmpUFD);
        buffer = readBlock(i);
        for (int j = 0; j < 16; j++) {
            memmove(&FileInput, buffer + j * 32, sizeof(FileInput));
            if (FileInput.fileName[0] != '\0') {
                FileList[i - 1].push_back(FileInput);
            }
            else {
                break;
            }
        }
    }
}

void initUOF() {
    vector<UOF> tmpUOF;
    for (int i = 17; i < UserList.size() + 17; i++) {
        StateList.push_back(tmpUOF);
        buffer = readBlock(i);
        for (int j = 0; j < 16; j++) {
            memmove(&StateInput, buffer + j * 32, sizeof(StateInput));
            if (StateInput.fileName[0] != '\0') {
                StateList[i - 17].push_back(StateInput);
            }
            else {
                break;
            }
        }
    }
}

void initCluster() {
    for (int i = 33; i < 100; i++) {
        buffer = readBlock(i);
        memmove(&ClusterInput, buffer, sizeof(ClusterInput));
        ClusterList.push_back(ClusterInput);
        if (ClusterInput.nextBlock == -1) {
            FreeBlockList.push_back(i);
        }
    }
}

void initFileSystem() {
    initMFD();
    initUFD();
    initUOF();
    initCluster();
}


