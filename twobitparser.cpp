
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<bitset>

#define bits32 unsigned int
#define bits64 unsigned long long
#define UBYTE char

using namespace std;

class Header{
    public:
        bits32 signature;
        bits32 version;
        bits32 seqCount;
        bits32 reserved;
};

class BitIndex{
    public:
        UBYTE nameSize;
        char *name;	
        bits32 offset;
};

class Data{
    public:
        bits32 size;
        bits32 nBlockCount;
        bits32 *nStarts;
        bits32 *nSizes;
        bits32 maskBlockCount;
        bits32 *maskStarts;
        bits32 *maskSizes;
        bits32 reserved;
};

inline void readHeader(fstream &rf,Header &h){
    // std::cout<<"\t\tHEADER info"<<std::endl;
    rf.read(reinterpret_cast<char *>(&h.signature),sizeof(h.signature));
    rf.read(reinterpret_cast<char *>(&h.version),sizeof(h.version));
    rf.read(reinterpret_cast<char *>(&h.seqCount),sizeof(h.seqCount));
    rf.read(reinterpret_cast<char *>(&h.reserved),sizeof(h.reserved));

    // std::cout<<"signature: "<<h.signature<<std::endl;
    std::cout<<"number of seq: "<<h.seqCount<<std::endl;
}

inline void readIndex(fstream &rf,vector<BitIndex*> &bii,bits32 &seqCount){
    // std::cout<<"\t\tINDEX info"<<std::endl;
    for(int i=0;i<seqCount;i++){
        BitIndex *bi = new BitIndex();
        
        rf.read(reinterpret_cast<char *>(&bi->nameSize),sizeof(bi->nameSize));
        
        char n[int(bi->nameSize)];
        bi->name = (char*)malloc(sizeof(char)*int(bi->nameSize));
        bi->name[int(bi->nameSize)+1] = '\0';
        rf.read(reinterpret_cast<char *>(&n),int(bi->nameSize));
        for(int i=0;i<bi->nameSize;i++){
            bi->name[i] = n[i];
        }
        rf.read(reinterpret_cast<char *>(&bi->offset),sizeof(bi->offset));
        bii.emplace_back(bi);

        // std::cout<<"sequence name size: "<<int(bi->nameSize)<<std::endl;
        // std::cout<<"sequence name: "<<bi->name<<std::endl;
        // std::cout<<"sequence data offset value: "<<bi->offset<<std::endl;
    }
}

void readData(fstream &rf,BitIndex *bi,std::bitset<100> *bitArray,std::bitset<100> *bitNs){
    bitNs[0]=0;
    Data d;
    std::cout<<bi->name<<std::endl;
    rf.seekg(bi->offset,ios::beg);

    rf.read(reinterpret_cast<char *>(&d.size),sizeof(d.size));
    rf.read(reinterpret_cast<char *>(&d.nBlockCount),sizeof(d.nBlockCount));

    d.nStarts = (bits32*)malloc(sizeof(bits32)*d.nBlockCount);
    for(int i=0;i<d.nBlockCount;i++){
        rf.read(reinterpret_cast<char *>(&d.nStarts[i]),sizeof(d.nStarts[i]));
    }

    d.nSizes = (bits32*)malloc(sizeof(bits32)*d.nBlockCount);
    for(int i=0;i<d.nBlockCount;i++){
        rf.read(reinterpret_cast<char *>(&d.nSizes[i]),sizeof(d.nSizes[i]));
    }

    rf.read(reinterpret_cast<char *>(&d.maskBlockCount),sizeof(d.maskBlockCount));

    d.maskStarts = (bits32*)malloc(sizeof(bits32)*d.maskBlockCount);
    for(int i=0;i<d.maskBlockCount;i++){
        rf.read(reinterpret_cast<char *>(&d.maskStarts[i]),sizeof(d.maskStarts[i]));
    }

    d.maskSizes = (bits32*)malloc(sizeof(bits32)*d.maskBlockCount);
    for(int i=0;i<d.maskBlockCount;i++){
        rf.read(reinterpret_cast<char *>(&d.maskSizes[i]),sizeof(d.maskSizes[i]));
    }

    rf.read(reinterpret_cast<char *>(&d.reserved),sizeof(d.reserved));

    // std::cout<<d.size<<std::endl;
    // std::cout<<d.nBlockCount<<std::endl;
    // std::cout<<d.nStarts[0]<<std::endl;
    // std::cout<<d.nSizes[0]<<std::endl;
    // std::cout<<d.maskBlockCount<<std::endl;
    // std::cout<<d.maskStarts[0]<<std::endl;
    // std::cout<<d.maskSizes[0]<<std::endl;
    // std::cout<<d.reserved<<std::endl;
    char c;
    size_t seqRun=0;
    unsigned long long int run = 0;
    int n=0;
    while(seqRun<d.size/4){
        rf.get(c);
        for (int i = 7; i >= 0; i--){
            bool b = ((c >> i) & 1);
            bitArray->set(run,b);
            run++;
        }
        seqRun++;
    }

    for(int i=0;i<d.nBlockCount;i++){
        unsigned long long int start = d.nStarts[i];
        unsigned long long int stop = start+d.nSizes[i];
        unsigned long long int cord=0;
        for (unsigned long long int j = start;j<=stop;j++){
            cord=j*2;
            bitNs[0].set(cord,1);
            bitNs[0].set(cord+1,1);
        }
    }

    unsigned long long int dn = d.size*2;

    for(int n=0;n<dn;n++){
        cout<<bitNs[0][n];
    }
    cout<<endl;

    for(int n=0;n < dn;n++){
        cout<<bitArray[0][n];
    }
    cout<<endl;



}

void readTwoBit(string &str_filename){
    fstream rf(str_filename,ios::in|ios::binary);
    Header h;
    readHeader(rf,h);
    
    vector<BitIndex*> bii;
    readIndex(rf,bii,h.seqCount);

    // std:cout<<"\t\tData info"<<std::endl;
    for(int i=0;i<bii.size();i++){
        BitIndex *bi = bii[i];
        
        std::bitset<100> *b_bitArray;
        b_bitArray = (std::bitset<100> *)malloc(sizeof(std::bitset<100>));
        
         std::bitset<100> *b_bitNs;
        b_bitNs = (std::bitset<100> *)malloc(sizeof(std::bitset<100>));
        
        readData(rf,bi,b_bitArray,b_bitNs);
    }
}

int main()
{
    string filename = "sample.2bit";
    readTwoBit(filename);

    return 0;
}