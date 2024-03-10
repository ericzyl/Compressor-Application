#include <iostream>
#include <fstream>
#include "Helper.hpp"
#include "HCTree.hpp"

int main(int argc, char* argv[]) {
    if(argc != 3){
        error("Invalid Argument");
    }
    
    vector<int> vect(256, 0);
    HCTree hct;
    FancyInputStream input(argv[1]);
    FancyOutputStream output(argv[2]);
    int numChar = 0; 
    int filesize = input.filesize();
    //int filesize = input.filesize();

    // Get header, count characters, and restore the Huffman Coding Tree
    if(filesize != 0){
        for(int i = 0; i < 256; i++){
            vect[i] = input.read_int();
            numChar += vect[i];
        }
    }else{
        return 0;
    }
    
    hct.build(vect);
    
    // Each iteration decode one character (byte)
    int count = 0; 
    //filesize - 1024
    while(count < numChar){
        unsigned char c = hct.decode(input);
        output.write_byte(c);
        count++;
    }

    return 0;
}
