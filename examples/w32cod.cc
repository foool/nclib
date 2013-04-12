#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

void print_bitmat(uint32_t* p_bitmat, int m, int k, int w){
    int i, j, p;
    uint32_t elt;

    for( i = 0; i < m; i++ ){
        for( j = 0; j < k; j++ ){
            for( p = 0; p < w; p++ ){
                printf("%u ", *(p_bitmat+i*(k*w)+j*w+p));
            }
            printf("\n");
        }
        printf("\n");
    }
    
}


void Matrix_To_Bitmatrix(const GMatrix & mat , uint32_t *bitmat){

    int i,j,p;
    uint64_t elt;
    int m = mat.rr;
    int k = mat.cc;
    int w = mat.ww;

    for( i = 0; i < m; i++ ){
        for( j = 0; j < k; j++ ){
            elt = mat.Get(i,j);
            for( p = 0; p < w; p++ ){
                *(bitmat+i*(k*w)+j*w+p) = elt;
                elt = (unsigned int) galois_single_multiply( elt, 2, w );
            }
        }
    }
}

void w32coding2( const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint32_t val;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_seg = size_in_buff/k;
    unsigned long size_seg_32 = size_seg/4;
    bool start_2;
    uint32_t * psrc;
    uint32_t * pdes;
    uint32_t bitmat[m][k][mat.ww];
    uint32_t *p_bitmat;

    memset(out_buff , 0 , size_in_buff*m/k);

    p_bitmat = &bitmat[0][0][0];
    Matrix_To_Bitmatrix( mat , p_bitmat);
    //print_bitmat(p_bitmat, m, k, mat.ww);

    for(int i = 0 ; i < m ; ++i){               //rows of matrix
        pdes = (uint32_t *)(out_buff + i*size_seg);
        for(int j = 0 ; j < k ; ++j){           //cols of matrix
            psrc = (uint32_t *)(in_buff + j*size_seg);
            for(unsigned long p = 0 ; p < size_seg_32 ; ++p ){       //mem buf segment
                val = 0;
                uint32_t val_in = *(psrc + p);
                for(int cbit = 0; cbit < 32; ++cbit){                   //each bit in a word
                    if(1 == ((val_in&(1<<cbit))?1:0)){
                        *(pdes + p) ^= bitmat[i][j][cbit];
                    }
                }
            }
        }
    }
}


void w32coding( const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint32_t val;
    uint32_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    int w = mat.ww;
    unsigned long size_comp_buff = size_in_buff/k;
    bool start_2;
    unsigned char * psrc;
    unsigned char * pdes;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);

    for(int i = 0 ; i < m ; ++i){
        pdes = out_buff + i*size_comp_buff;
        for(int j = 0 ; j < k ; ++j){
            
            val = mat.ele32[i*k + j];
            memset(pcom , 0 , size_comp_buff);
            psrc = in_buff + j*size_comp_buff;
            start_2 = false;
            
            if(0 == val){continue;}
            if(1 == val){galois_region_xor(psrc, pcom, pcom, size_comp_buff); continue;}
            
            for(mask = 0x80000000 ; 0 < mask; mask >>= 1){
                if(mask & 1){           //the lowest bit, the last bit to deal with
                    if(val & 1){        //the last bit is 1
                        galois_region_xor(psrc , pcom , pcom , size_comp_buff);
                    }
                    continue;
                }
                if(0 != (val & mask)){
                    // the shift bit is 1
                    start_2 = true;
                    galois_region_xor(psrc , pcom , pcom , size_comp_buff);
                    galois_w32_region_multby_2(pcom , size_comp_buff);
                }else{
                    // the shift bit is 0
                    if(start_2){
                        galois_w32_region_multby_2(pcom , size_comp_buff);
                    }
                }
            }
            galois_region_xor(pcom , pdes , pdes , size_comp_buff);
        }
    }
    free(pcom);
}

/* Test galois_region_multiply speed and multiply 2 speed */
int main(void){
    ntime t_begin, t_end;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned char * out_buff2;
    GMatrix mat_i;
    GMatrix mat_m;
    GMatrix mat;
    unsigned long size_in_buff = 1024*1024;
    unsigned long size_out_buff = 1024*1024;

    mat.Make_identity(4, 4, 32);
    mat_m.Make_random(4, 4, 32);
    while(!Is_full(mat_m)){
        mat_m.Make_random(4, 4, 32);
    }
    //mat.Append_matrix(mat_m);
    NOTE("mat_m");
    mat_m.Print(0);
    
    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);
    out_buff2 = (unsigned char *)malloc(size_out_buff);

    memset(in_buff , 23, size_in_buff);
   
    NOTE("matrix_coding");
    gettimeofday(&t_begin , NULL);
    matrix_coding(mat_m, out_buff, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(matrix coding):%3.3f MB/S\n\n", 1024.0/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0));

    
    NOTE("w32_coding");
    gettimeofday(&t_begin , NULL);
    w32coding(mat_m, out_buff2, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(w32 coding):%3.3f MB/S\n", 1024.0/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0));

    if(0 == memcmp(out_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    
    NOTE("w32_coding2");
    gettimeofday(&t_begin , NULL);
    w32coding2(mat_m, out_buff2, in_buff, size_in_buff);
    gettimeofday(&t_end, NULL);
    printf("Speed(w32 coding):%3.3f MB/S\n", 1024.0/((t_end.tv_sec-t_begin.tv_sec)*1000.0 + (t_end.tv_usec - t_begin.tv_usec)/1000.0));
    
    if(0 == memcmp(out_buff , out_buff2 , size_out_buff)){
        printf("The Same\n\n");
    }else{
        printf("The Different\n\n");
    }
    
    free(in_buff);
    free(out_buff);
    free(out_buff2);
}
