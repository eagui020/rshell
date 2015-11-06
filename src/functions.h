#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <utility>
#include <vector>

using namespace std;

char *lgnID = getlogin();   //cstring with userID
char hostname[256];         //array to be used in hostnameCheck
int hostnameCheck = gethostname(hostname, sizeof hostname); //returns fail/pass

void terminalPrompt() //Loads the username or
{
    if(lgnID == NULL || hostnameCheck == -1) //nothing returned for ID
    {
        cout << "Get UserID or HostName failed." << endl;
        cout << "$ ";
    }
    else
    {
        cout << "[" <<  lgnID << "@" << hostname << "]" << "$ ";
    }
    return;
}

pair<vector<string>, vector<char> > parse(string line){
//function gets a full command line as a parameter
//and returns a pair of vectors: the first is a
//vector of idividual commands and args 
//the second is a vector of connector in the order they appear
    vector<char> connectors;
    vector<string> commands;
    char delimit[]= ";&|#"; //delimiters used by strtok
    
    //get commands
    for(unsigned int i=0; i<line.size(); i++){
        if(line.at(i)=='&' && line.at(i+1) == '&'){
            connectors.push_back('&');
        }
        else if(line.at(i)=='|' && line.at(i+1) == '|'){
            connectors.push_back('|');
        }
        else if(line.at(i)==';'){
            connectors.push_back(';');
        }
        else if(line.at(i)=='#'){
            connectors.push_back('#');
        }
    }
    
    
    //create char pointer for use with strtok
    char* charLine = new char[line.size()+1];
    copy(line.begin(), line.end(), charLine);
    charLine[line.size()]='\0';
    
    char *linePoint = strtok(charLine, delimit);
    while(linePoint != NULL){
        string newCommand = string(linePoint);
            for(unsigned int i=0; i<newCommand.size(); i++){
                if(newCommand.at(0)==' '){
                    newCommand.erase(0,1);
                }
                if(newCommand.at(newCommand.size()-1)==' '){
                    newCommand.erase(newCommand.size()-1,1);
                }
            }
            commands.push_back(newCommand);
        
        
        linePoint = strtok(NULL, delimit);
    }
    
    pair< vector<string>, vector<char> > endPair 
                            = make_pair(commands, connectors);
    delete[] charLine;
    return endPair;
}

void getCommand(string singleCommandLine, char** charCommand )
{
    char* charLine = new char[singleCommandLine.size()+1]; //pointer to cstring
    copy(singleCommandLine.begin(), singleCommandLine.end(), charLine);
    charLine[singleCommandLine.size()]='\0'; //last character empty set
    
    //char * charCommand[singleCommandLine.size()];
    
    while (*charLine != '\0') //while not at end of line
    {
        while (*charLine == ' ' || *charLine == '\t' || *charLine == '\n')
        { //while charLine points to whitespace
            *charLine = '\0';     // replace white spaces with \0
            charLine++;
        }
        
        *charCommand++ = charLine;
        
        
        while (*charLine != '\0' && *charLine != ' ' &&  
                *charLine != '\t' && *charLine != '\n')
        {

            charLine++;             //Keep parsing until end of word
        }
        
        *charCommand = '\0';
    }
    
}

int executeProgram(char **commandLine)
{
    pid_t pid = fork();
    int programStatus;

    if(pid < 0) //Error making child
    {
        cout << "Forking child failed." << endl;
        exit(2);
    }
    else if(pid == 0) //parent
    {
        execvp(*commandLine, commandLine); //execute and save state
        cout << "Command execution failed. Command '"
	     << *commandLine << "' not found." << endl;
  	    exit(1);
    }
    else
    {
        while (waitpid(-1, &programStatus, 0) != pid);  //wait for children
    	if(WIFEXITED(programStatus)) //Check status of child
    	{
            if(WEXITSTATUS(programStatus) == 0) //execvp never ended
        	{
            	//cout << "Program succeeded." << endl;
    		    return 0;
        	}
    	    else //ended and exited
    	    {
                //cout << "Program failed but exited normally." << endl;
    		    return -1;
    	    }
    	}
    	else //scary stuff happened in the program if it gets here
    	{
        	//cout << "Program exited abnormally." << endl;
    		exit(3);
    	}    
    }
}

void shell()
{
    while(1)
    {
        terminalPrompt(); // display terminal promt
        
        string rawLine;
        getline(cin, rawLine); // get full raw command line
        
        pair<vector<string>, vector<char> > parsedInfo = parse(rawLine);
        //parse raw line into vector of command and vector of connecors
        
        
        for(unsigned int i=0; i<parsedInfo.first.size(); i++)
        {
            //construct array of cstring for individual commadn + arguments
            char ** charCommand = new char *[parsedInfo.first.at(i).size()];
            //fill array of cstrings
            getCommand(parsedInfo.first.at(i), charCommand);
            
            //check if command was exit 
            if(strcmp(charCommand[0], "exit")==0){
                exit(0);
            }
            
            //execute command and save return status
            int cmdStatus = executeProgram(charCommand); 
            
            //if another command exists after i
            if(i + 1 < parsedInfo.first.size())
            {
                //cout << parsedInfo.second.at(i);
                if(parsedInfo.second.at(i)=='#') //check connectors and status 
                {
                    break;
                }
		        else if((parsedInfo.second.at(i)=='&' && cmdStatus<0) ||
                            (parsedInfo.second.at(i)=='|' && cmdStatus==0))
		        {
		            ++i; //Skip next command
		        }
            }
            delete[] charCommand;
        }
    }
}

#endif
