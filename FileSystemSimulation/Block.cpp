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

extern ifstream fin;
extern ofstream fout;
extern char * buffer;

char* readBlock(int blockNum) {
    openFile();
    fin.seekg(blockNum * 513);
    fin.read(buffer, 512);    //read函数
    closeFile();
    return buffer;
}
