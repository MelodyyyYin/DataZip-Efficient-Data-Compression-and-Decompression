#include <iostream>
#include <string>
#include <vector>
#include "HCTree.hpp"
#include "Helper.hpp"

int main(int argc, char* argv[]){
    /*Parse the command line arguments and throw an error message if the user runs your program incorrectly*/
    if(argc != 3){
        error("argument number is not right");
    }
    FancyInputStream z(argv[1]);
    if(z.good()!= true){
        error("file invalid");
    } 
    /*Open the input file for reading*/
    if(z.filesize() == 0){
        FancyOutputStream q (argv[2]);
        return 0;
    }
    vector<int> occurences(256, 0);
    //FancyInputStream z(argv[1]);
    /*Read bytes from the file. Count the number of occurrences of each byte value*/
    while(true){
        int numbers = z.read_byte();
        if(numbers == -1){
            break;
        }
        occurences[numbers-1] +=1;
    }
    /*Use the byte counts to construct a Huffman coding tree. Each unique byte with a non-zero count will be a leaf node in the Huffman tree*/
    HCTree *temp = new HCTree();
    temp->build(occurences);
    /*Open the output file for writing*/
    FancyOutputStream q (argv[2]);
    /*Write enough information (a "file header") to the output file to enable the coding tree to be reconstructed when the file is read by your uncompress program*/
    /* original 
    for(unsigned long i = 0; i < occurences.size();i++){
        q.write_int(occurences[i]);
    }                          
    */
    int a = 0;
    vector<int> efficient;
    vector<unsigned char> second;
    for(unsigned long i = 0; i < occurences.size();i++){
        if(occurences[i] > 0){
            second.push_back((unsigned char) i);
            efficient.push_back(occurences[i]);    
            a+=1;
        }
        
    }
    q.write_int(a);
    for(int i = 0; i < efficient.size(); i++){
        q.write_byte(second[i]);
        q.write_int(efficient[i]);
    }


    /*Move back to the beginning of the input file to be able to read it, again*/
    FancyInputStream c (argv[1]);
    /*Using the Huffman coding tree, translate each byte from the input file into its code, and append these codes as a sequence of bits to the output file, after the header*/
    while(true){
        int number = c.read_byte();
        if(number == -1){
            break;
        }
        temp->encode((unsigned char)(number), q);
    }
    delete temp;
    return 0;
    /*Close the input and output files (note that this is handled for you; see Design Notes)*/
}
