#include "functions.h"

using namespace std;

int main(){
    /*
    string cmdLine = "ls -a && echo hello";
    pair<vector<string>, vector<char> > output = parse(cmdLine);
    for(unsigned int i=0; i<output.first.size() ; i++){
        cout << output.first.at(i) << endl;
    }
    for(unsigned int i=0; i<output.second.size() ; i++){
        cout << output.second.at(i) << endl;
    }
    
    char * charCommand[output.first.at(0).size()];
    getCommand(output.first.at(0), charCommand);
    
    char * charCommand1[output.first.at(1).size()];
    getCommand(output.first.at(1), charCommand1);
    
    terminalPrompt();

    executeProgram(charCommand);
    executeProgram(charCommand1);
   */
   
   shell();
   return 0;
}

/*
Some usefult test cases:
"echo hello shell"
"hack CIA || echo invalid command" //check if cmd 2 runs after cmd 1 fails
"hack CIA && echo valid command" //check if cmd 2 doesnt run after cmd 1 fails !!!!!!!!!!NEEDS FIX
"ls -a && echo valid command" //check if cmd 2 does run after cmd 1 runs
*/