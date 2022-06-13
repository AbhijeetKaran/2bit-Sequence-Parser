# TwobitParser
twobitParser is a cpp library for accessing content of 2bit files. It also offers extra terminal output functions.

# fasta vs 2bit
- FASTA file is a text-based file for representing either nucleotide sequences or amino acid (protein) sequences.Each record in fasta file format, has a header followed by the sequence, nucleotide or amino acids from the next line. Fasta file can store sigle or multiple sequences and has no upper limit for storage. Data from a fasta file is fetched sequentially but random access can be achived using an index file of the same fasta file.
File extensions for fasta file are '.fasta', '.fna', '.ffn', '.faa', '.frn' and '.fa'.

- 2Bit file is a serialized file format which stores only nucleotide sequences. Highly efficient and compact, uses ~4 fold less memory for storage as compared to fasta format, Since each nucleotide is represente by 2 bits instead of 8 bits in ASCII-based text format. 2bit file can store upto 4GB of total genomic size.
More detailed information about structure of 2bit file format is avialable at UCSC website "http://genome.ucsc.edu/FAQ/FAQformat.html#format7"
File extensions for a 2bit file is '.2bit'.
	
### Advantage in term of storage and accessibility
| Parameter           | Fasta     | 2bit     |
| :------------------ | :-------- | :------- |
| **storage meamory** | 51.8 MB   | 13.2 MB  |
| **Access time**     | 3 sec     | 1 sec    |
  
# Usage
Usage example is also provide in example.cpp file.

Example is also provide in example.cpp file 
  #### Including header
    #include "twobitparser.cpp"
  
  #### Creating an object
    string filename = "example.2bit";
    TwoBitReader tbr_obj(filename);

  #### Accessing list of sequencesIDs along with sequence length
    map<string,size_t> *sequenceMap;
    sequenceMap = tbr_obj.showSequenceIds();
   
    // for viewing content of map
    for (auto m : *sequenceMap){
        cout<<m.first<<"\t"<<m.second<<endl;
    }
  
  #### Fetch a sequence or sub-sequence
    // using sequence Id
    char *sequence = nullptr;
    sequence = tbr_obj.extractSequence("SampleChr1");
    cout << sequence << endl; 
    free(sequence);
  
    // using sequence id with start and end
    sequence = tbr_obj.extractSequence("SampleChr1",10,100);
    cout << sequence << endl;
    free(sequence);
  
    // using a Bed file
    string bedfile = "example.bed";
    tbr_obj.extractSequenceFromBedFile(bedfile,0);
                      or
    tbr_obj.extractSequenceFromBedFile(str_bedfile,1);
	
  
   #### Fetch Hard masked informations/N block informations
    // Number of N's in complete file
    cout << "total N's in 2bit file: " << tbr_obj.countNs() << endl;
    
    // Number of N's in a sequence
    string str_seqId = "SampleChr1";
    cout << "total N's in sequence " << str_seqId << " : " << tbr_obj.countNs(str_seqId) << endl;
    
    // all N Blocks in the 2bit file
    std::map<string,std::map<int,int>> NMap = tbr_obj.NMap;
  
    
   #### Fetch Soft masked informations
    // Total number of nucleotide in soft masked region
    cout << "total softmasked nucleotide in 2bit file: " << tbr_obj.countSoftMasked()<<endl;
  
    // Number of nucleotides in soft masked region in a sequence
    cout<<"total softmasked nucleotide in fasta file: "<<tbr_obj.countSoftMasked("SampleChr1")<<endl;
  
    // all soft masked region in 2bit file
    std::map<string,std::map<int,int>> maskMap = tbr_obj.maskMap;
  
Functions with terminal based output
   #### Converting sequence from 2bit to fasta
    string outputfile = "exampleOutput.fasta";
    tbr_obj.twoBitToFasta(outputfile);
  
   #### Soft masking sequences using coordinates from bed file
    string bedFile = "example.bed";
    tbr_obj.softMaskSequencesUsingBedFile(bedFile);
  
   #### Hard masking sequences using coordinates from bed file
    string bedFile = "example.bed";
    tbr_obj.hardMaskSequencesUsingBedFile(bedFile);

# License
