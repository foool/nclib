#pragma once

#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include "../gf_complete.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

//#define repair_path "/home/libai/frepair/"
//#define decode_path "/home/libai/fdecode/"
//#define primary_path "/home/libai/fprimary/"
//#define meta_path "/home/libai/metadata/"


class FRC{
    public:
        int rows;
        int cols;
		int n;
		int k;
        int i;
        int alpha;
        int beta;
        int d;
        int bw;
        int w_s;
        int w;
        int gamma;
        unsigned long file_len;
        string mat;
        string file_name;


		//constructor
        FRC(int n, int k, int d, int i, int w, int ws);
        FRC(int rows, int cols, int n, int k, int beta, int d, int w, int ws);
        FRC(int rows, int n, int k, int beta, int d, int w, int ws);
        FRC(string confFile);

        //methodes
        int GetEncodeMatrix();
        int MemCoding(GMatrix & gm, unsigned char *psrc, unsigned char *pdes, int size, int type)const;
        int MemCoding(GMatrix64 & gm, unsigned char *psrc, unsigned char *pdes, int size, int type)const;
        int MemCoding(GMatrixU8 & gm, unsigned char *psrc, unsigned char *pdes, int size, int type)const;
		int Encode(string filepath, string filename);
		int Decode(string path_decode, string nodes_idx);

        /*********  to do****************
        int Encode(string filename)const;
        int Encode(string filename, int file_len);
        int Encode(string filepath, string filename, int file_len);
        int Decode(string filename)const;
        *********************************/

        //meta repair
		int Repair(string filepath, int vnode);
        int RepairFile(string filepath, int vnode)const;

        //Get class member values
        int GetN()const;
        int GetK()const;
        int GetW()const;
        int GetAlpha()const;
        int GetFilelen()const;

        //Set class member values
        void SetN(const int& n_set);
        void SetK(const int& k_set);
        void SetW(const int& w_set);
        void SetAlpha(const int& alpha);

        //Print the class member values
        void SRCmeta_print()const;
};


//get random fileidx
int GetRandomFileidx(vector<int> & files_idx_list, int n, int d, int alpha, int vnode, int vpnode, int rnode);

//sort the fileids
int SortIds(vector<int> & idx_list, int begin, int end);

int Gcd ( int a, int b );
int GetBW(int alpha, int k, int beta, int d);

bool TestParameters(int rows, int cols, int n, int k, int beta, int d, int w, int ws);

template <class T>
int UpdateMeta(const T & matrix, const T & matrix_rand, const vector<int> & files_idx_list, Config & conf)
{
    string mat;
    for(int i = 0; i < matrix.rr; i++)
    {
        for(int j = 0; j < matrix.cc; j++)
        {
            ostringstream oss;
            oss<<matrix.Get(i, j);
            mat = mat + oss.str() + " ";
        }
    }
    mat = _Trim(mat);
    string mat_rand;
    for(int i = 0; i < matrix_rand.rr; i++)
    {
        for(int j = 0; j < matrix_rand.cc; j++)
        {
            ostringstream oss;
            oss<<matrix_rand.Get(i, j);
            mat_rand = mat_rand + oss.str() + " ";
        }
    }
    mat_rand = _Trim(mat_rand);
    string files_idx;
    for(vector<int>::const_iterator iter = files_idx_list.begin(); iter != files_idx_list.end(); iter++)
    {
        ostringstream oss;
        oss<<(*iter);
        files_idx = files_idx + oss.str() + " ";
    }
    files_idx = _Trim(files_idx);
    conf.Set("mat", mat);
    conf.Set("mat_repair", mat_rand);
    conf.Set("files_idx_repair", files_idx);
    return 1;
}

template <class T>
int GetFullRankRows(const T & matrix, T & mat_de, const vector<int> & nodes_idx_list, vector<int> & files_idx_list, int k, int alpha)
{
    int cols = matrix.cc;
    int vpnode = cols / k;
    int rnode = cols % k;
    int randnum;
    srand((unsigned)time(NULL));
    while(true)
    {
        files_idx_list.clear();
        for(int i = 0 ; i < k ; i++)
        {
            vector<int> vpn_idx;
            //printf("the size:%d at i:%d\n", vpn_idx.size(), i);
            for(int j = 0; j < vpnode; j++)
            {
                do
                {
                    randnum = rand()%alpha;
                    //printf("randnum:%d\n", randnum);
                    //getchar();
                    vector<int>::iterator findit = find(vpn_idx.begin(), vpn_idx.end(), randnum);
                    if(findit == vpn_idx.end())
                    {
                        vpn_idx.push_back(randnum);
                        files_idx_list.push_back(nodes_idx_list.at(i)*alpha + randnum);
                        break;
                    }
                }while(1);
            }
        }
        printf("NOTED\n");
        vector<int> nodes_idx_add;
        int i = 0;
        while(i < rnode)
        {
            int randnode = rand()%k;
            vector<int>::iterator findit_nodes = find(nodes_idx_add.begin(), nodes_idx_add.end(), randnode);
            if(findit_nodes == nodes_idx_add.end())
            {
                nodes_idx_add.push_back(randnode);
                do
                {
                    randnum = nodes_idx_list.at(randnode)*alpha + rand()%alpha;
                    //printf("the random num:%d\n", randnum);
                    //getchar();
                    vector<int>::iterator findit = find(files_idx_list.begin(), files_idx_list.end(), randnum);
                    if(findit == files_idx_list.end())
                    {
                        files_idx_list.push_back(randnum);
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }while(1);
                i++;
            }
        }
        mat_de = Draw_rows(matrix, files_idx_list , cols);
        if(Rank(mat_de) == cols)
        {
            break;
        }
    }
    return 1;
}


