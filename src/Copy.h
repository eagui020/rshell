#ifndef COPY_H
#define COPY_H

#include <errno.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BufferSize 8192

using namespace std;

void copyFstream(string src, string dest)
{
    ifstream source;
    source.open(src.c_str());
    if(!source.is_open())
    {
        cout << "Error opening file." << src << endl;
        return;
    }
    else
    {
        ofstream destination;
        destination.open(dest.c_str());
        if(!destination.is_open())
        {
            cout << "Error opening file" << dest << endl;
            return;
        }
        else
        {
            char c = source.get();

            while (source.good())
            {
                destination.put(c);
                c = source.get();
            }
        }
        
    }
    return;
}
void CopyStyleUnix(string src, string dest)
{
    int input_fd, output_fd;
    int  ret_in;
    char buffer[1];
    input_fd = open (src.c_str(), O_RDONLY);
        // if it returns 99 it is an error from input_fd
        if (input_fd == -1) 
        {
            perror ("input_fd didn't work");
            //return 99;
        }
    output_fd = open(dest.c_str() ,O_WRONLY | O_CREAT, 0644);
        // if it returns 99 it is an error from input_fd
        if(output_fd == -1)
        {
            perror( "output_fd didn't work ");
            //return 98;
        }
    
    while((ret_in = read(input_fd, &buffer, 1)) > 0)
    {
        write(output_fd, &buffer, 1);
    }
    close(input_fd);
    close(output_fd);
}

void CopyStyleUnixBuffer(string src, string dest)
{
    int input_fd, output_fd;
    ssize_t  ret_in;
    char buffer[BufferSize];
    input_fd = open (src.c_str(), O_RDONLY);
        // if it returns 99 it is an error from input_fd
        if (input_fd == -1) 
        {
            perror ("input_fd didn't work");
            //return 99;
        }
    output_fd = open(dest.c_str() ,O_WRONLY | O_CREAT, 0644);
    // if it returns 99 it is an error from input_fd
    if(output_fd == -1)
    {
        perror( "output_fd didn't work ");
        //return 98;
    }
    while( (ret_in = read(input_fd, &buffer, BufferSize)) > 0)
    {
        write(output_fd, &buffer,(ssize_t) ret_in);
    }
    close(input_fd);
    close(output_fd);
    
    
}

#endif




