#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <utility>
#include <vector>
#include "Copy.h"
#include "Timer.h"
#include <sstream>

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

bool FileExists(string file)
{
    // FInd and delete [] or test
    if (file.find("[") != string::npos)
    {
        file.erase(file.begin());
        file.erase(file.end() - 1);
        // cout << file << endl;
    }
    if (file.find("test ") != string::npos)
    {
        int tempPos = file.find("test");
        file.erase(tempPos, 4);
    }
    
    // Finds flag and saves it, deleting after
    char flag = 'e';
    if (file.find("-f") != string::npos)
    {
        // cout << "-f found" << endl;
        flag = 'f';
        int tempPos = file.find("-f");
        file.erase(tempPos, 2);
    }
    else if (file.find("-d") != string::npos)
    {
        // cout << "-d found" << endl;
        flag = 'd';
        int tempPos = file.find("-d");
        file.erase(tempPos, 2);
    }
    else if (file.find("-e") != string::npos)
    {
        // cout << "-e found" << endl;
        flag = 'e';
        int tempPos = file.find("-e");
        file.erase(tempPos, 2);
    }
    else if (file.find("-") != string::npos)
    {
        cout << "Invalid flag" << endl;
        return false;
    }
    while(file.at(file.size() - 1) == ' ') //delete spaces after
    {
        file.erase(file.end() - 1);
    }
    while(file.at(0) == ' ') //delete spaces before
    {
        file.erase(0, 1);
    }
    if(file.at(0) == '/') //remove first slash
    {
        file.erase(file.end() - 1);
    }
    
    
    
    // cout << "Goes in here" << endl;
    struct stat buffer;
    // cout << file.c_str() << endl;
    
    
    if(stat(file.c_str(), &buffer) == -1) // Not a file or direct
    {
        cout << "Not found" << endl;
        return false;
    }
    else
    {
        if (flag == 'f')
        {
            // cout << "-f found" << endl;
            return ((S_ISREG(buffer.st_mode)) != 0);
        }
        else if (flag == 'd')
        {
            // cout << "-d found" << endl;
            return ((S_ISDIR(buffer.st_mode)) != 0);
        }
        else
        {
            // cout << "no flag or -e found" << endl;
            return true;
        }
    }
}

bool CopyFiles(string files)
{
    vector<string> argv; // Simulate command line arguments
    string buffer; // Used to push arguments into argv
    stringstream ss(files); // Insert the filenames into stream
    while (ss >> buffer)
    {
        argv.push_back(buffer);
    }
    int argc = argv.size();
    
    if (argc == 3)
    {
        if(FileExists(argv[1]))
        {
            if(!FileExists(argv[2]))
            {
                Timer a;
                // //Timer b;
                // double bTime;
                // a.start();
                // copyFstream(argv[1], argv[2]);
                // a.elapsedSystemTime(bTime);
                // cout << bTime << endl;
                
                
                double aTime;
                a.start();
                cout << "Copying..." << endl;
                CopyStyleUnixBuffer(argv[1], argv[2]);
                a.elapsedSystemTime(aTime);
                cout << "Elapsed time: " << aTime << endl;
                
                // double cTime;
                // a.start();
                // CopyStyleUnix(argv[1], argv[2]);
                // a.elapsedSystemTime(cTime);
                // cout << cTime << endl;
                return 1;
            }
            else
            {
                cout << "Dest file exists." << endl;
                return 0;
            }
        }
        else
        {
            cout << "Src doesn't exist." << endl;
            return 0;
        }
    }
    else if(argc == 4)
    {
        if(FileExists(argv[1]))
        {
            if(!(FileExists(argv[2])))
            {
                Timer a;
                double bTime;
                a.start();
                cout << "Copying..." << endl;
                copyFstream(argv[1], argv[2]);
                a.elapsedSystemTime(bTime);
                cout << "Elapsed time: " << bTime << endl;
                
                //remove(argv[2]);
                
                double cTime;
                a.start();
                cout << "Copying..." << endl;
                CopyStyleUnix(argv[1], argv[2]);
                a.elapsedSystemTime(cTime);
                cout << "Elapsed time: " << cTime << endl;
                
                //remove(argv[2]);
                
                double aTime;
                a.start();
                cout << "Copying..." << endl;
                CopyStyleUnixBuffer(argv[1], argv[2]);
                a.elapsedSystemTime(aTime);
                cout << "Elapsed time: " << aTime << endl;
                
                return 1;
            }
            else
            {
                cout << "Dest file exists." << endl;
                return 0;
            }
        }
        else
        {
            cout << "Src doesn't exist." << endl;
            return 0;
        }
    }
    else
    {
        cout << "Not enough arguments given" << endl;
        return 0;
    }
}

pair<vector<string>, vector<char> > bigParse(string line){
//function gets a full command line as a parameter
//and returns a pair of vectors: the first is a
//vector of idividual commands and args 
//the second is a vector of connector in the order they appear

//cout << "begin bigParse" << endl; //TEST ONLY
    vector<char> connectors;
    vector<string> commands;
    char delimit[]= "()"; //delimiters used by strtok
    bool insideParenthasis=0;
    bool inQuotes = false;
    
    //get connectors
    for(unsigned int i=0; i<line.size(); i++)
    {
        if(line.at(i)== '(' ){
            insideParenthasis=1;
        }
        else if(line.at(i)== ')' ){
            insideParenthasis=0;
        }
        else if(line.at(i) == '"' && (!inQuotes))
        {
            inQuotes = true;
        }
        else if(line.at(i) == '"' && (inQuotes))
        {
            inQuotes = false;
        }
        if(!insideParenthasis && !inQuotes){
            if(line.at(i)=='&' && line.at(i+1) == '&'){
                connectors.push_back('&');
                line.at(i)=')';
                line.erase(i+1,1);
            }
            else if(line.at(i)=='|' && line.at(i+1) == '|'){
                connectors.push_back('|');
                line.at(i)=')';
                line.erase(i+1,1);
            }
            else if(line.at(i)==';'){
                connectors.push_back(';');
                line.at(i)=')';
            }
            else if(line.at(i)=='#'){
                connectors.push_back('#');
                line.at(i)=')';
            }    
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
                while(!newCommand.empty() && newCommand.at(0)==' '){ //remove spaces infront 
                    newCommand.erase(0,1);
                }
                while(!newCommand.empty() && newCommand.at(newCommand.size()-1)==' '){ //remove spaces in back  
                    newCommand.erase(newCommand.size()-1,1);
                }
            }
            if(!newCommand.empty() && newCommand!="&&" && newCommand!="||" )
            {
                commands.push_back(newCommand);
            }
        
        linePoint = strtok(NULL, delimit);
    }
    
    pair< vector<string>, vector<char> > endPair 
                            = make_pair(commands, connectors);
    delete[] charLine;
//TEST ONLY
// cout << "command vector = " << endl;
// for (unsigned int i=0; i < commands.size(); i++){
//     cout << commands.at(i) << endl; 
// }
// cout << "connector vector = " << endl;
// for (unsigned int i=0; i < connectors.size(); i++){
//     cout << connectors.at(i) << endl; 
// }  
// cout << "end bigParse" << endl;
//TEST ONLY
    return endPair;
}

pair<vector<string>, vector<char> > parse(string line){
//function gets a full command line as a parameter
//and returns a pair of vectors: the first is a
//vector of idividual commands and args 
//the second is a vector of connector in the order they appear
//cout << "begin Parse" << endl; //TEST ONLY
vector<char> connectors;
    vector<string> commands;
    bool inQuotes = false;
    char delimit[]= ";&|#"; //delimiters used by strtok
    
    vector<string> quotedLines;
    //get commands
    for(unsigned int i=0; i<line.size(); i++){
        if(line.at(i) == '"' && (!inQuotes))
        {
            inQuotes = true;
            string str;
            int j = i + 1;
            while(line.at(j) != '"')
            {
                str.push_back(line.at(j));
                ++j;
            }
            quotedLines.push_back(str);
        }
        else if(line.at(i) == '"' && (inQuotes))
        {
            inQuotes = false;
        }
        else if(!inQuotes)
        {
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
    }
    for (unsigned int i = 0; i < quotedLines.size(); ++i)
    {
        int quotedSize = quotedLines.at(i).size();
        size_t found = line.find(quotedLines.at(i));
        if (found != string::npos)
        {
            for(int i = 0; i <= quotedSize; ++i)
            {
                line.erase(line.begin() + found);
            }
        }
    }
    
    //create char pointer for use with strtok
    char* charLine = new char[line.size()+1];
    copy(line.begin(), line.end(), charLine);
    charLine[line.size()]='\0';
    int quotedLinesPlace = 0;
    
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
        size_t quotePos = newCommand.find('"');
        if(quotePos != string::npos)
        {
            newCommand.erase(newCommand.begin() + quotePos);
            newCommand.append(quotedLines.at(quotedLinesPlace));
            commands.push_back(newCommand);
            ++quotedLinesPlace;
        }
        else
        {
            commands.push_back(newCommand);
        }
        linePoint = strtok(NULL, delimit);
    }
    
    // for (unsigned int i = 0; i < commands.size(); ++i)
    // {
    //     cout << commands[i] << endl;
    // }
    
    pair< vector<string>, vector<char> > endPair 
                            = make_pair(commands, connectors);
    delete[] charLine;
//cout << "end Parse" << endl; //TEST ONLY
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

bool  execCmdSegment(pair<vector<string>, vector<char> > parsedInfo)
{
    bool validity = true;
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
            // cout << charCommand[0] << endl;
            // cout << parsedInfo.first.at(i) << endl;
            if((parsedInfo.first.at(i).find("test ") != string::npos) || 
                    parsedInfo.first.at(i).find("[") != string::npos)
            {
                // cout << parsedInfo.first.at(i) << endl;
                validity = FileExists(parsedInfo.first.at(i));
            }
            else if(parsedInfo.first.at(i).find("copy ") != string::npos)
            {
                validity = CopyFiles(parsedInfo.first.at(i));
            }
            else
            {
                //execute command and save return status
                int cmdStatus = executeProgram(charCommand); 
                if ( cmdStatus == -1)
                {
                    validity = false;    
                }
                else 
                {
                    validity = true;
                }
            }
            //if another command exists after i
            if(i + 1 < parsedInfo.first.size() )
            {
                //cout << parsedInfo.second.at(i);
                if(parsedInfo.second.at(i)=='#') //check connectors and status 
                {
                    break;
                }
		        else if((parsedInfo.second.at(i)=='&' && !validity) ||
                            (parsedInfo.second.at(i)=='|' && validity))
		        {
		            while ((parsedInfo.second.at(i)=='&' && !validity) ||
                            (parsedInfo.second.at(i)=='|' && validity))
                            {
                                ++i; //Skip next command
                                if (i >= parsedInfo.second.size())
                                {
                                    break;
                                }
                            }
		        }
            }
            delete[] charCommand;
        }
        
        return validity;
}

void shell()
{
    while(1)
    {
        terminalPrompt(); // display terminal promt
        
        string rawLine;
        getline(cin, rawLine); // get full raw command line
        
        //parse raw line into vector of command segments and vector of connecors
        pair<vector<string>, vector<char> > parsedLine = bigParse(rawLine);
        
        for(unsigned int i=0; i<parsedLine.first.size(); i++)
        {
            // for each segment 
            pair<vector<string>, vector<char> > cmdSegment = 
                                                parse(parsedLine.first.at(i));
            
            //execute the segment and save the result 
            bool lastCmdValidity = execCmdSegment(cmdSegment);
            
            //if another command exists after i
            if(i + 1 < parsedLine.first.size() )
            {
                //check connectors;
                if(parsedLine.second.at(i)=='#') //check connectors and status 
                {
                    break;
                }
		        else if((parsedLine.second.at(i)=='&' && !lastCmdValidity) ||
                            (parsedLine.second.at(i)=='|' && lastCmdValidity))
		        {
		            while ((parsedLine.second.at(i)=='&' && !lastCmdValidity) ||
                            (parsedLine.second.at(i)=='|' && lastCmdValidity))
                            {
                                ++i; //Skip next command
                                if (i >= parsedLine.second.size())
                                {
                                    break;
                                }
                            }
		        }
            }
        }
    }
}

#endif
