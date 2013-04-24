#pragma once

#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "galois.h"
extern "C"{
#include "gf_complete.h"
}

#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)>(b))?(b):(a))
#define PRT_STR(ss) {fprintf(stdout, "== %s == \n",ss);}
#define NOTE(ss) {fprintf(stdout, "=== %s === \n",ss);}
#define ERROR(ss) {fprintf(stdout, "## %s ##\n",ss);exit(0);}
#define PRT_RETURN {fprintf(stdout,"\n");}
#define DLLEXPORT extern "C"

#define  ntime struct timeval

int gettimeofday(struct timeval *tv,struct timezone *tz);

using namespace std;

class GMatrix64{
    public:
        vector <uint64_t> ele;
        int rr;
        int cc;
        int ww;

        GMatrix64();
        GMatrix64(int row, int col);
        uint64_t Get(int row, int col)const;
        void Set(int row, int col, uint64_t val);
        int Make_zero(int row, int col);
        int Make_identity(int row, int col);
        int Make_random(int row, int col);
        int Make_from_string(string str, int rows, int cols, int w);
        int Make_vandermonde(int r, int c);
        int Make_sys_vandermonde(int r, int c);
        void Print()const;
        void Swap_rows(int row1, int row2);
        void Del_row(int row);
        void Del_rows(int begin, int len);
        void Row_to_irow(int row, uint64_t prod);
        void Row_plus_irow(int row1, int row2, uint64_t prod);
        void Replace_matrix(GMatrix64 mat, int begin);
        void Append_matrix(GMatrix64 mat_app);
        void Append_matrix(GMatrix64 mat_app, int begin, int len);
        void Clear_zero_rows();
        void Inverse();

    private:
        int Resize_matrix();
};
GMatrix64 Draw_rows(const GMatrix64& mat, const string& row_list, const int& len);
GMatrix64 Draw_rows(const GMatrix64& mat, const vector<int>& row_list, const int& len);

GMatrix64 Slice_matrix(const GMatrix64& mat, const int& begin, const int& len); 
int Rank(GMatrix64 mat);
bool Is_full(GMatrix64 mat);

class GMatrixU8{
    public:
        vector <uint8_t> ele;
        int rr;
        int cc;
        int ww;
        GMatrixU8();
        GMatrixU8(int r, int c, int w);
        int Make_zero(int r, int c, int w);
        int Make_identity(int r, int c, int w);
        int Make_random(int r, int c, int w);
        int Make_from_string(string str, int rows, int cols, int w);
        int Make_vandermonde(int r, int c, int w);
        int Make_sys_vandermonde(int r, int c, int word);
        void Set(int r, int c, int w);
        int Get(int r, int c)const;
        void Print()const;
        void Swap_rows(int row1, int row2);
        void Del_row(int row);
        void Del_rows(int begin, int len);
        void Row_to_irow(int row, int prod);
        void Row_plus_irow(int row1, int row2, int prod);
        void Replace_matrix(GMatrixU8 mat, int begin);
        void Append_matrix(GMatrixU8 mat_app, int begin, int len);
        void Clear_zero_rows();
        void Inverse();

    private:
        int Resize_matrix();
};

GMatrixU8 Draw_rows(const GMatrixU8& mat, const string& row_list, const int& len);
GMatrixU8 Draw_rows(const GMatrixU8& mat, const vector<int>& row_list, const int& len);

GMatrixU8 Slice_matrix(const GMatrixU8& mat, const int& begin, const int& len);
int Rank(const GMatrixU8& mat);


/*****************
 ** GMatrix.cc **
 *****************/
class GMatrix{

    public:
        vector <uint8_t> ele8;
        vector <uint16_t> ele16;
        vector <uint32_t> ele32;
        int rr;
        int cc;
        int ww;

        /* Create the matrix with/without the numbers of    *
         * rows, colomns and wordsize of each element       */
        GMatrix();
        GMatrix(int r, int c, int w);
        
        /* Set the matrix from a string */
        int Make_from_list(int *p, int r, int c, int w);
        int Make_from_string(string str, int r, int c, int w);

        /* Set the matrix from a file */
        int Make_from_file(FILE *fp, int rows, int cols, int w);

        /* Set the all zero matrix or identity matrix       *
         * if success, return 1 , or false return 0         */
        int Make_zero(int r, int c, int w);
        int Make_identity(int r, int c, int w);
        int Make_vandermonde(int r, int c, int w);
        int Make_sys_vandermonde(int r, int c, int w);
        int Make_random(const int& r, const int& c, const int& w);
        int Make_random_nz(const int& rows, const int& cols, const int& w);
        int Make_random(const int& r, const int& c, const int& w, const int& mod);

        /* Empyt or Not empty */
        bool Empty()const;

        /* Set/Get the value of row c , colomn c */
        void Set(int r, int c, uint32_t val);
        uint32_t Get(int r, int c)const;

        /* overwrites the values of matrix with values of m */
        int Copy(const GMatrix *&mat);
        
        /* Print the value of the matrix to stdout */
        void Print(int type = 1)const;

        /* Print the value of the matrix as a string */
        void Print_as_str()const;

        /* Inverse the matrix */
        void Inverse();
        
        /* Is full-rank matrix or not? yes:1 ; no:0*/
        bool Is_full()const;

        /* swap r1 and r2 */
        void Swap_rows(int r1 , int r2);

        /* swap c1 and c2 */
        void Swap_cols(int c1 , int c2);

        /* row r1 = row r1 + row r2*/
        void Row_plus_row(int r1 , int r2);

        /* row r1 = row r1 + row r2 * prod */
        void Row_plus_irow(int r1 , int r2 , int prod);

        /* row r = row r * prod*/
        void Row_to_irow(int r, int prod);
        
        /* Delete row */
        void Del_row(int row);

        /* Delete len rows from the begin row*/
        void Del_rows(int begin, int len);

        /* Delete rows which numbers are in vec*/
        void Del_rows(vector<int> vec);

        /* Delete column col*/
        void Del_col(int col);

        /* Delete len cols from the begin */
        void Del_cols(int begin, int len);

        /* Insert a matrix before the from-th row */
        void Insert_matrix(GMatrix mat_ins, int from); 
        void Insert_matrix(GMatrix mat_ins, int from, int len); 

        /* Append a matrix mat_app after the matrix */
        void Append_matrix(GMatrix mat_app);

        /* Append a matrix from the begin-th row after the matrix*/
        void Append_matrix(GMatrix mat_app, int begin, int len);

        /* Replace from the from-th row of the matrix with mat */
        void Replace_matrix(GMatrix mat, int begin, int len);

        /* Replace the matrix with mat from the from-th row */
        void Replace_matrix(GMatrix mat, int begin);

        /* Wipe the matrix all value with value */
        void Wipe_matrix(int value);
        
        /* Wipe length rows of the matrix from begin with value */
        void Wipe_matrix(int begin, int length, int value);

        /* Delete rows which value equals to 0 */
        void Clear_zero_rows();

    private:
        /* Resize the size of elems, and set elements */
        int Resize_matrix();
};

/* Return matrix m1 * m2 */
GMatrix Prod(GMatrix m1 , GMatrix m2);

/* The row-th row of mat multiply the p_src 
 * and store in the p_des
 * the length of the string p_src point to is length */
int Prod(GMatrix mat, int row, unsigned char *p_des, unsigned char *p_src, int len);

/* matrix m1*unsigned char [] */
GMatrix Prod(GMatrix &m1 , unsigned char * m2);

/* Matrix m3 = m1 * m2 */
int Prod(GMatrix *m3 , GMatrix *m1 , GMatrix *m2);

/* Add m1 and m2 */
GMatrix Add( GMatrix m1, GMatrix m2);

/* m3 = m2 + m1 */
void Add(GMatrix &m3, GMatrix &m1, GMatrix &m2);

/* Copy dst_mat from src_mat */
void Copy(GMatrix &dst_mat, const GMatrix &src_mat);
        
/* Return the rank of a matrix */
int Rank(GMatrix mat);

/* Is full-rank matrix or not */
bool Is_full(GMatrix mat);

/* Inverse the matrix m and return it */
GMatrix Inverse(GMatrix mat);
int Inverse(GMatrix &dst_mat, GMatrix &src_mat);

/* Transpose the matrix along the diagonal */
GMatrix Transpose(GMatrix mat);

/* Generate a r(rows)*c(cols) size GMatrix matrix */
/* GMatrix Random_GMatrix(int r, int c); */

/* Drag a matrix from the matrix with rows from begin to end */
GMatrix Slice_matrix(const GMatrix& mat, const int& begin, const int& len); 

/* Get len rows from mat */
GMatrix Draw_rows(const GMatrix& mat, const vector<int>& row_list, const int& len);
GMatrix Draw_rows(const GMatrix& mat, const int * row_list, const int& len);
GMatrix Draw_rows(const GMatrix& mat, const string& row_list, const int& len);

/* GMatrix transform*/
void Mat8to32(GMatrix& mat);
void Mat8to16(GMatrix& mat);
void Mat16to8(GMatrix& mat);
void Mat16to32(GMatrix& mat);
void Mat32to8(GMatrix& mat);
void Mat32to16(GMatrix& mat);

/*****************
 ** property.cc **
 *****************/
bool CL_property(GMatrix mat, int n, int k);

/* RP_property is used to check which nodes cann't repair  *
 * in the next loop, but it cann't be a decisive condition *
 * as NK_property                                          */
bool RP_property(const GMatrix& mat, int n, int k, int beta);

/* NK_property checks the system combining the new node *
 * satisfing (n,k) property, this is, k out of n nodes  *
 * can reconstruct the original data                    *
 * piece : how many rows of matrix mat in a node        */
int NK_property(GMatrix mat, int piece, int k);
int NK_property_(const GMatrix mat, const int piece, const int k, const int beta);
int NK_property(GMatrix64 mat, int piece, int k);
int NK_property(GMatrixU8 mat, int piece, int k);

/* 
 * If any of cols rows of mat are indenpendent 
 */
bool AnyCols(const GMatrix& mat, const int dbeta);

/*
 * from n nodes, select any beta vectors from any d=(n-1) nodes have a full rank
 */ 
bool DB_property(const GMatrix& mat, const int n, const int beta);


/**************
 ** utils.cc **
 **************/

gf_t gf_complete_init(int w);

/* Generate a 0~num number */
DLLEXPORT int NC_random(int i);
int NC_random(int min, int max);

/* Return the time interval */
double dt_s(struct timeval end,struct timeval start);

double dt_ms(struct timeval end,struct timeval start);

double dt_us(struct timeval end,struct timeval start);

void mem_print(unsigned char* pc, int len, int breaklen);
void mem_print_(unsigned char* pc, int len, int breaklen);

int Get_file_len(const string& filename);


DLLEXPORT int NC_code_py(char * mat_c_p, int r, int c, int w, char *p_des, char *p_src, int length);

/* Encode a file using mat_en matrix                    *
 * fp_src points to the file to be encoded              *
 * fpp_des point to the files where encoded data store  */
int NC_encode_file(GMatrix mat_en, FILE *fp_src, FILE **fpp_des);

/* Decode files to a file                               *
 * fpp_src point to the files to be decoded             *
 * fp_des point to the file where decoded data store    */
int NC_decode_file(GMatrix mat_de, FILE **fpp_src, FILE *fp_des);

string _Trim(string ss);

/* Batch to write/read/delete filenums files to/from buffer  *
 * filename_00, filename_01, ... , filename_filenums         */
int Bat_Write(  const string& filename, 
                const int& filenum, 
                unsigned char* out_buff, 
                const unsigned long& size_each_file, 
                const string& filepath
                );
int Bat_Write(  const string& filename, 
                const vector<int>& files_idx_list, 
                unsigned char * out_buff, 
                const unsigned long& size_each_file, 
                const string& filepath
                );
int Bat_Read(   const string& filename, 
                const vector<int>& files_idx_list,
                unsigned char * in_buff, 
                const unsigned long& size_each_file
                );
int Bat_Read(   string filename, 
                string files_index, 
                unsigned char* in_buff, 
                unsigned long size_each_file
                );
int Bat_Delete( const string& filename, 
                const vector<int>& files_idx_list
                );
int Bat_Delete( string filename, 
                string files_index
                );

/* read/write from/to single file(string filename)*/
void fsread_buff(   const string& filename, 
                    unsigned char* p_buff, 
                    const unsigned long& len
                    );

void fswrite_buff(  const string& filename, 
                    unsigned char* p_buff, 
                    const unsigned long& len
                    );
void fswrite_buff(  const string& filename, 
                    const int num , 
                    unsigned char *p_buff , 
                    const long& len
                    );


//Exception types
struct File_not_found{
    const char* filename;
    File_not_found(const char* filename_):filename(filename_){
    }
};

struct File_can_not_create{
    const char* filename;
    File_can_not_create(const char* filename_):filename(filename_){
    }
};

template <class T>
T Get_fixsize(T size , int fixnum){
    T ret;
    
    if(0 == (size%fixnum)){
        ret = size;
    }else{
        ret = (size/fixnum+1)*fixnum;
    }

    return ret;
}


GMatrix64 Mat8to64(GMatrix& mat);
GMatrix64 Mat16to64(GMatrix& mat);
GMatrix64 Mat32to64(GMatrix& mat);
GMatrix64 Prod(GMatrix64 m1, GMatrix64 m2);

/* Is full-rank matrix or not */
bool Is_full(GMatrix64 mat);
bool Is_full(GMatrixU8 mat);

GMatrixU8 Prod(GMatrixU8 m1, GMatrixU8 m2);

GMatrix U8tomat(GMatrixU8 mat, int w);
GMatrix64 U8to64(GMatrixU8 mat);

/***********************
 *     mcoding.c       *
 ***********************/


/* Encode/Decode the data using GMatrix matrix         *
 * mat is the Encode/Decode matrix                      *
 * p_des store the data after Encode/Decode             *
 * p_src store the data before Encode/Decode            *
 * length represent the length of the data to deal with *
 * p_des and p_src must be long-word aligned            */
int matrix_coding(  const GMatrix& mat, 
                    unsigned char * const p_des, 
                    unsigned char * const p_src, 
                    const unsigned long& length
                    );

int matrix_coding_( const GMatrix& mat, 
                    unsigned char * const p_des, 
                    unsigned char * const p_src, 
                    const unsigned long& length
                    );

int matrix_coding_opt( const GMatrix& mat, 
                    unsigned char * const p_des, 
                    unsigned char * const p_src, 
                    const unsigned long& length,
                    gf_t gfm
                    );

void w4coding(   const GMatrixU8& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w4coding_( const GMatrixU8& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w4coding_opt( const GMatrixU8& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length,
                gf_t gfm);

void w8coding(   const GMatrix& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w8coding_( const GMatrix& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w8coding_opt( const GMatrix& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length,
                gf_t gfm);

void w16coding(  const GMatrix& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w16coding_(const GMatrix& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w16coding_opt( const GMatrix& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length,
                gf_t gfm);

void w32coding(  const GMatrix& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w32coding_(  const GMatrix& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w32coding_opt( const GMatrix& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length,
                gf_t gfm);

void w64coding(  const GMatrix64& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w64coding_(const GMatrix64& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length);

void w64coding_opt( const GMatrix64& mat,
                unsigned char * const p_des, 
                unsigned char * const p_src,
                const unsigned long& length,
                gf_t gfm);

