//
//  Unix.hpp
//  File System Simulation
//
//  Created by JIN ZHAO on 12/30/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//

#ifndef Unix_h
#define Unix_h

struct Command{
    string cmdItem[3];
};

struct HAND_TO{
    char cmdName[10];
    void (*handler)();
};

void format();

void loginWelcome();

void processCommand();
#endif /* Unix_hpp */
