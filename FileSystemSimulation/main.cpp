//
//  main.cpp
//  FileSystemSimulation
//
//  Created by JIN ZHAO on 12/31/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//
#include "Common.h"
#include "Unix.h"
#include "File.h"
#include "Block.h"
#include "Stream.h"

int main() {
//    format();
    initFileSystem();
    loginWelcome();
    processCommand();
//    cout<<"it's the end"<<endl;
    return 0;
}
