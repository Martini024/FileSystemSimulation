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
vector< Cluster> ClusterList;

MFD UserInput;
UFD FileInput;
UOF StateInput;
Cluster ClusterInput;
string currentUserName = "";
int currentUserId = -1;
char* buffer = new char[513];



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
            memmove(&FileInput, buffer + i * sizeof(FileInput), sizeof(FileInput));
            if (FileInput.fileName[0] != '\0') {
                FileList[i].push_back(FileInput);
            }
            else {
                break;
            }
        }
    }
}

void initUOF() {
    vector<UOF> tmpUOF;
    for (int i = 16; i < UserList.size() + 16; i++) {
        StateList.push_back(tmpUOF);
        buffer = readBlock(i);
        for (int j = 0; j < 16; j++) {
            memmove(&StateInput, buffer + i * sizeof(StateInput), sizeof(StateInput));
            if (StateInput.fileName[0] != '\0') {
                StateList[i].push_back(StateInput);
            }
            else {
                break;
            }
        }
    }
}

void initFileSystem() {
    initMFD();
    initUFD();
    initUOF();
}


