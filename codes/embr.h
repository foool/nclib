#pragma once

#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

class E_MBR{
    public:
		int n;
		int k;
        string filename;
        int file_len;
        string en_from_filepath;
        string en_to_filepath;
        string de_from_filepath;
        string de_to_filepath;
        int file_len;
		GMatrix mat;

		//constructor
        E_MBR(int n_nodes, int k_nodes);
        E_MBR(int n_nodes, int k_nodes, int word);
        E_MBR(int n_nodes, int k_nodes, int word, GMatrix mat_t);
        E_MBR(string conf_file);
		
        //methodes
		int Encode()const;
		int Decode(string files_idx)const;
		int Repair(int vnode)const;

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
