#pragma once

#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

class SRC{
    public:
		int n;
		int k;
		int f;
        int fileLen;
		GMatrix mat;

		//constructor
		SRC(int n, int k, int f, GMatrix m);
        SRC(string confFile);
		
        //methodes
		int Encode(string filename, string filepath)const;
		int Decode(string file_path, string encoded_filename, string files_idx)const;

        //meta repair
		int Repair(string filepath, int vnode)const;

        //Get class member values
        int GetN()const;
        int GetK()const;
        int GetF()const;
        int GetFilelen()const;
        GMatrix GetMat()const;

        //Set class member values
        int SetN(const int& n_set);
        int SetK(const int& k_set);
        int SetF(const int& f_set);
        int SetMat(const GMatrix& mat_set);

        //Print the class member values
        void SRCmeta_print()const;
};
