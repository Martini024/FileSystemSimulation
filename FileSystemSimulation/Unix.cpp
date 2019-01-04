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

HAND_TO handlerList[] = {
    { "create", do_Create},
    { "delete", do_Delete},
    { "read", do_Read},
    { "write", do_Write},
    { "ls", do_Ls},
    { "chmod", do_Chmod},
    { "chown", do_Chown},
    { "mv", do_Mv},
    { "copy", do_Copy},
    
    { "login", do_Login},
    { "logout", do_Logout},
    { "register", do_Register},
    { "passwd", do_Passwd},
    { "cancel", do_Cancel},
    
    { "help", do_Help},
    { "exit", do_Exit},
    { "clear", do_Clear},
    { "format", do_Format}
    
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
        
        bool findQuotes = false;
        string inner, post;
        int quotes1 = (int)str.find('\"');
        int quotes2 = (int)str.rfind('\"');
        if (quotes1 != -1 && quotes1 != quotes2) {
            string pre = str.substr(0, quotes1);
            inner = str.substr(quotes1 + 1, quotes2 - quotes1 - 1);
            str = pre;
            findQuotes = true;
        }
        
        stringstream sin(str);

        cmdParaNum = 0;
        for (int i = 0; i < 3; i++) {
            cmd.cmdItem[i] = "";
        }
        while (sin >> cmd.cmdItem[cmdParaNum]){
            cmdParaNum++;
        }
        if (findQuotes) {
            cmd.cmdItem[cmdParaNum] = inner;
            cmdParaNum++;
        }
        
        transform(cmd.cmdItem[0].begin(),cmd.cmdItem[0].end(),cmd.cmdItem[0].begin(),::tolower);
        int flag;
        flag = 0;
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
