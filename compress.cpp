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
    int filesize = input.filesize();

    // Update the frequency vector by iterating the whole file
    for(int i = 0; i < input.filesize(); i++){
        vect[input.read_byte()]++;
    }

    // Write vect to compressed file's header
    // Takes 256*4 bytes
    if(filesize != 0){
        for(unsigned int i = 0; i < vect.size(); i++){
            output.write_int(vect[i]);
        }
    }else{
        return 0;
    }
    

    // Run the HCTree to compress the file
    // reset input stream?
    input.reset();
    
    /*if(filesize == 0){
        // output.write_bit();
        return 0;
    }*/
    hct.build(vect);
    for(int i = 0; i < filesize; i++){
        unsigned char c = char(input.read_byte());
        hct.encode(c, output);
    }

    return 0;
}
