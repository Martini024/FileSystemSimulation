//
//  Unix.cpp
//  File System Simulation
//
//  Created by JIN ZHAO on 12/30/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//

#include "Common.h"
#include "Unix.h"
#include "File.h"
#include "Command.h"

Command cmd;

int cmdParaNum;

HAND_TO handlerList[] =
{
    { "Chmod", do_Chmod},
    { "Chown", do_Chown},
    { "Mv", do_Mv},
    { "Copy", do_Copy},
    { "Type", do_Type},
    { "Passwd", do_Passwd},
    { "Login", do_Login},
    { "Logout", do_Logout},
    { "Create", do_Create},
    { "Delete", do_Delete},
    { "Open", do_Open},
    { "Close", do_Close},
    { "Write", do_Write},
    { "Read", do_Read},
    { "Help", do_Help},
    { "dir", do_Dir},
    { "cancel",do_Cancel},
    { "Register", do_Register},
    { "Exit", do_Exit},
    { "Clear", do_Clear}
};

extern string currentUserName;

void loginWelcome() {
    cout << "Welcome to use two level directory systems provided by ZHAO JIN !!!" << endl;
    cout << "Enter help to show all command available." << endl;
}

void processCommand() {
    //MARK: Command Parse
    while(true) {
        cout << endl << "JINs-MacBook-Pro: " << currentUserName << "$ ";

        string str;
        getline(cin, str);
        stringstream sin(str);
        
        cmdParaNum = 0;
        while (sin >> cmd.cmdItem[cmdParaNum]){
            cmdParaNum++;
        }
        int flag = 0;
        for (int i = 0; i < 20; i++)
        {
            if (strcmp(handlerList[i].cmdName, cmd.cmdItem[0].c_str()) == 0)
            {
                flag = 1;
                handlerList[i].handler();
                break;
            }
        }
        if (!flag)
            cout << "command not found" << endl;
    }
    
}
