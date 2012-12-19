#pragma once

#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

class Reed_Sol{
    public:
		int n;
		int k;
        string filename;
        int file_len;
        string en_from_filepath;
        string en_to_filepath;
        string de_from_filepath;
        string de_to_filepath;
		GMatrix mat;

		//constructor
        Reed_Sol(int n_nodes, int k_nodes);
        Reed_Sol(int n_nodes, int k_nodes, int word);
        Reed_Sol(int n_nodes, int k_nodes, GMatrix mat_t);
        Reed_Sol(string conf_file);
		
        //methodes
		int Encode()const;
		int Decode(const string& files_idx)const;
        /*******************************************************
         *              repair types                           *
         * 10 repair one failure in systematic part(S)         *
         * 11 repair P checksum failure(P)                     *
         * 12 repair Q checksum failure(Q)                     *
         * 22 repair one systematic part and Q failures(S+Q)   *
         * 99 repair other more than one failures              *
         * P represents XOR checksum, Q represents RS checksum *
         *******************************************************/
		int Repair( int fnode )const;
        int Repair( string fnodes )const;

        //Get class member values
        int GetN()const;
        int GetK()const;
        string GetFilename()const;
        int GetFilelen()const;
        GMatrix GetMat()const;

        //Set class member values
        void SetN(const int& n_set);
        void SetK(const int& k_set);
        void SetMat(const GMatrix& mat_set);
        void SetEnpath(const string& en_from_filepath, const string& en_to_filepath);
        void SetDepath(const string& de_from_filepath, const string& en_to_filepath);
        void SetFile(const string& fname, const int& flen);
        void SetFile(const string& fname);

        //Print the class member values
        void meta_print()const;
};

void fread_buff(const string& filename, unsigned char* p_buff, const unsigned long& len);

// exception types
struct File_Length_Error{
    const int file_len;
    File_Length_Error(const int file_len_):file_len(file_len_){
    }
};
