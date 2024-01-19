#include <iostream>
#include <string>
#include <vector>
#include "Helper.hpp"
#include "HCTree.hpp"

int main(int argc, char* argv[]){
    if(argc != 3){
        error("argument number is not right");
    }
    if(FancyInputStream(argv[1]).good()!= true){
        error("file invalid");
    } 
    vector<int> occurences(256, 0);
    /*Open the input file for reading*/
    FancyInputStream z (argv[1]);
    if(z.filesize()==0){
        FancyOutputStream q(argv[2]);
        return 0;
    }
    /*Read the file header at the beginning of the input file, and use it to reconstruct the Huffman coding tree*/
    /* original file
    int total = 0;
    for(int i = 0; i < 256; i++){
        int number = z.read_int();
        if(number != 0){
            occurences[i] = number;
            total += occurences[i]; 
        }
    }
    */
    int total = 0;
    int a = z.read_int();
    for(int i = 0; i < a; i++){
        unsigned char ii = z.read_byte();
        int freqs = z.read_int();
        occurences[ii] = freqs;
        total += freqs;
    }


    HCTree *temp = new HCTree();
    temp->build(occurences);
    /*Open the output file for writing*/
    FancyOutputStream q(argv[2]);
    /*Using the Huffman coding tree, decode the bits from the input file into the appropriate sequence of bytes, writing them to the output file*/
    for(int i = 0; i < total; i++){
        q.write_byte(temp->decode(z));
    }
    delete temp;
    return 0;
}
