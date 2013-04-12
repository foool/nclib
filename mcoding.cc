#include "nc.h"

/* Encode/Decode the data using GMatrix matrix         *
 * mat is the Encode/Decode matrix                      *
 * p_des store the data after Encode/Decode             *
 * p_src store the data before Encode/Decode            *
 * length represent the length of the data to deal with *
 * p_des, p_src must be long-word aligned               */
int matrix_coding(  const GMatrix& mat, 
                    unsigned char * const p_des, 
                    unsigned char * const p_src, 
                    const unsigned long& length){
    int n, k, w;
    int i, j;
    int len_seg;
    unsigned int muper;
    unsigned char *pdes = p_des;
    unsigned char *psrc = p_src;

    k = mat.cc;
    n = mat.rr;
    w = mat.ww;

    assert(length%mat.cc == 0);
    
    len_seg = length/k;
    memset(pdes, 0, length*n/k);

    switch(w){
        case 8:
            for(i = 0; i < n; i++){
                for(j = 0; j < k; j++){
                    muper = mat.Get(i, j);
                    if(0 == muper){ continue; }
                    if(1 == muper){
                        galois_region_xor(psrc+j*len_seg, pdes, pdes, len_seg);
                    }else{                    
                        galois_w08_region_multiply(psrc+j*len_seg, muper, len_seg, pdes, 1);    
                    }
                }
                pdes = pdes + len_seg;
            }
            break;
        case 16:
            for(i = 0; i < n; i++){
                for(j = 0; j < k; j++){
                    galois_w16_region_multiply(psrc+j*len_seg, 
                            mat.Get(i,j), len_seg, pdes, 1);    
                }
                pdes = pdes + len_seg;
            }
            break;
        case 32:
            for(i = 0; i < n; i++){
                for(j = 0; j < k; j++){
                    muper = mat.ele32[i*mat.cc+j];
                    galois_w32_region_multiply(psrc+j*len_seg, 
                            muper, len_seg, pdes, 1);    
                }
                pdes = pdes + len_seg;
            }
            break;
    }

    return mat.rr;
}

int matrix_coding_( const GMatrix& mat, 
                    unsigned char * const p_des, 
                    unsigned char * const p_src, 
                    const unsigned long& length){
    int n, k, w;
    int i, j;
    int len_seg;
    unsigned int val;
    unsigned char *pdes = p_des;
    unsigned char *psrc = p_src;

    k = mat.cc;
    n = mat.rr;
    w = mat.ww;

    assert(length%mat.cc == 0);
    
    len_seg = length/k;
    memset(pdes, 0, length*n/k);

    switch(w){
        case 8:
            for(j = 0; j < k; j++){
                pdes = p_des;
                psrc = p_src+j*len_seg;
                for(i = 0; i < n; i++){
                    val = mat.Get(i, j);
                    if(0 == val){ continue; }
                    if(1 == val){
                        galois_region_xor(psrc, pdes, pdes, len_seg);
                    }else{                    
                        galois_w08_region_multiply(psrc, val, len_seg, pdes, 1);    
                    }
                    pdes = pdes + len_seg;
                }
            }
            break;
        case 16:
            for(j = 0; j < k; j++){
                pdes = p_des;
                psrc = p_src+j*len_seg;
                for(i = 0; i < n; i++){
                    galois_w16_region_multiply(psrc, mat.Get(i,j), len_seg, pdes, 1);    
                    pdes = pdes + len_seg;
                }
            }
            break;
        case 32:
            for(j = 0; j < k; j++){
                pdes = p_des;
                psrc = p_src+j*len_seg;
                for(i = 0; i < n; i++){
                    val = mat.ele32[i*mat.cc+j];
                    galois_w32_region_multiply(psrc, val, len_seg, pdes, 1); 
                    pdes = pdes + len_seg;
                }
            }
            break;
    }

    return mat.rr;
}

int matrix_coding_opt( const GMatrix& mat, 
                    unsigned char * const p_des, 
                    unsigned char * const p_src, 
                    const unsigned long& length,
                    gf_t gfm){
    int n, k, w;
    int i, j;
    int len_seg;
    unsigned int val;
    unsigned char *pdes = p_des;
    unsigned char *psrc = p_src;

    k = mat.cc;
    n = mat.rr;
    w = mat.ww;

    assert(length%mat.cc == 0);
    
    len_seg = length/k;
    memset(pdes, 0, length*n/k);

    switch(w){
        case 8:
            for(j = 0; j < k; j++){
                pdes = p_des;
                psrc = p_src+j*len_seg;
                for(i = 0; i < n; i++){
                    val = mat.Get(i, j);
                    gfm.multiply_region.w32(&gfm, psrc, pdes, val, len_seg, 1);
                    pdes = pdes + len_seg;
                }
            }
            break;
        case 16:
            for(j = 0; j < k; j++){
                pdes = p_des;
                psrc = p_src+j*len_seg;
                for(i = 0; i < n; i++){
                    val = mat.Get(i, j);
                    gfm.multiply_region.w32(&gfm, psrc, pdes, val, len_seg, 1);
                    pdes = pdes + len_seg;
                }
            }
            break;
        case 32:
            for(j = 0; j < k; j++){
                pdes = p_des;
                psrc = p_src+j*len_seg;
                for(i = 0; i < n; i++){
                    val = mat.ele32[i*mat.cc+j];
                    gfm.multiply_region.w32(&gfm, psrc, pdes, val, len_seg, 1);
                    pdes = pdes + len_seg;
                }
            }
            break;
    }

    return mat.rr;
}

void w4coding(  const GMatrixU8& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    unsigned int val;
    unsigned int mask_;
    unsigned int mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    bool start_2;
    unsigned char * psrc;
    unsigned char * pdes;

    assert(mat.ww == 4);
    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    mask_ = 1<<(4-1);
    /*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(int i = 0 ; i < m ; ++i){
        pdes = out_buff + i*size_comp_buff;
        for(int j = 0 ; j < k ; ++j){
            
            val = mat.ele[i*k + j];
            memset(pcom, 0, size_comp_buff);
            psrc = in_buff + j*size_comp_buff;
            start_2 = false;
            
            if(0 == val){continue;}
            if(1 == val){galois_region_xor(psrc, pdes, pdes, size_comp_buff); continue;}
            
            for(mask = mask_ ; 0 < mask; mask >>=1){
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
                    galois_w04_region_multby_2_64(pcom , size_comp_buff);
                }else{
                    // the shift bit is 0
                    if(start_2){
                        galois_w04_region_multby_2_64(pcom , size_comp_buff);
                    }
                }
            }
            galois_region_xor(pcom , pdes , pdes , size_comp_buff);
        }
    }
    /*
    gettimeofday(&t_end, NULL);
    printf("internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
    */
    free(pcom);
}

void w4coding_( const GMatrixU8& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint8_t val;
    uint8_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;
    
    assert(mat.ww == 4);

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int *)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            int val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }

    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift <= h_index[j]; shift++, mask<<=1){
            for(i = 0; i < m; i++){
                val = mat.ele[i*k+j];
                if(mask&val){
                    galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                }
            }
            galois_w04_region_multby_2_64(pcom , size_comp_buff);
        }
    }

    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}

void w4coding_opt( const GMatrixU8& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff, 
                gf_t gfm){
    uint8_t val;
    uint8_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;
    
    assert(mat.ww == 4);

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int*)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            int val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }

    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift <= h_index[j]; shift++, mask<<=1){
            for(i = 0; i < m; i++){
                val = mat.ele[i*k+j];
                if(mask&val){
                    gfm.multiply_region.w32(&gfm, pcom, pdes[i], 1, size_comp_buff, 1);
                    //galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                }
            }
            gfm.multiply_region.w32(&gfm, pcom, pcom, 2, size_comp_buff, 0);
            //galois_w04_region_multby_2_64(pcom , size_comp_buff);
        }
    }

    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}

void w8coding(  const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint8_t val;
    uint8_t mask_;
    uint8_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    bool start_2;
    unsigned char * psrc;
    unsigned char * pdes;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    mask_ = 1<<(8-1);
    /*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(int i = 0 ; i < m ; ++i){
        pdes = out_buff + i*size_comp_buff;
        for(int j = 0 ; j < k ; ++j){
            
            val = mat.ele8[i*k + j];
            memset(pcom, 0, size_comp_buff);
            psrc = in_buff + j*size_comp_buff;
            start_2 = false;
            
            if(0 == val){continue;}
            if(1 == val){galois_region_xor(psrc, pdes, pdes, size_comp_buff); continue;}
            
            for(mask = mask_ ; 0 < mask; mask >>=1){
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
                    galois_w08_region_multby_2_64(pcom , size_comp_buff);
                }else{
                    // the shift bit is 0
                    if(start_2){
                        galois_w08_region_multby_2_64(pcom , size_comp_buff);
                    }
                }
            }
            galois_region_xor(pcom , pdes , pdes , size_comp_buff);
        }
    }
    /*
    gettimeofday(&t_end, NULL);
    printf("internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
    */
    free(pcom);
}

void w8coding_( const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint8_t val;
    uint8_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int *)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            int val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }

/*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift <= h_index[j]; shift++, mask<<=1){
            for(i = 0; i < m; i++){
                val = mat.ele8[i*k+j];
                if(mask&val){
                    galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                }
            }
            galois_w08_region_multby_2_64(pcom , size_comp_buff);
        }
    }
/*
    gettimeofday(&t_end, NULL);
    printf("_internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
*/
    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}

void w8coding_opt( const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff,
                gf_t gfm){
    uint8_t val;
    uint8_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int *)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            int val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }

/*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift < h_index[j]; shift++, mask<<=1){
            for(i = 0; i < m; i++){
                val = mat.ele8[i*k+j];
                if(mask&val){
                    //galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                    gfm.multiply_region.w32(&gfm, pcom, pdes[i], 1, size_comp_buff, 1);
                }
            }
            //galois_w08_region_multby_2_64(pcom , size_comp_buff);
            gfm.multiply_region.w32(&gfm, pcom, pcom, 2, size_comp_buff, 0);
        }
    }
/*
    gettimeofday(&t_end, NULL);
    printf("_internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
*/
    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}

void w16coding( const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint16_t val;
    uint16_t mask_ = 1<<(16-1);
    uint16_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    bool start_2;
    unsigned char * psrc;
    unsigned char * pdes;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    /*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(int i = 0 ; i < m ; ++i){
        pdes = out_buff + i*size_comp_buff;
        for(int j = 0 ; j < k ; ++j){
            
            val = mat.ele16[i*k + j];
            memset(pcom , 0 , size_comp_buff);
            psrc = in_buff + j*size_comp_buff;
            start_2 = false;
            
            if(0 == val){continue;}
            if(1 == val){galois_region_xor(psrc, pdes, pdes, size_comp_buff); continue;}
            
            for(mask = mask_ ; 0 < mask; mask >>=1){
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
                    galois_w16_region_multby_2_64(pcom , size_comp_buff);
                }else{
                    // the shift bit is 0
                    if(start_2){
                        galois_w16_region_multby_2_64(pcom , size_comp_buff);
                    }
                }
            }
            galois_region_xor(pcom , pdes , pdes , size_comp_buff);
        }
    }
    /*
    gettimeofday(&t_end, NULL);
    printf("internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
    */
    free(pcom);
}

void w16coding_(const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint16_t val;
    uint16_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int *)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            int val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }

/*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift < h_index[j]; shift++, mask<<=1){
            for(i = 0; i < m; i++){
                val = mat.ele16[i*k+j];
                if(mask&val){
                    galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                }
            }
            galois_w16_region_multby_2_64(pcom , size_comp_buff);
        }
    }
/*
    gettimeofday(&t_end, NULL);
    printf("_internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
*/
    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}

void w16coding_opt(const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff,
                gf_t gfm){
    uint16_t val;
    uint16_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int*)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            int val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }

/*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift < h_index[j]; shift++, mask<<=1){
            for(i = 0; i < m; i++){
                val = mat.ele16[i*k+j];
                if(mask&val){
                    //galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                    gfm.multiply_region.w32(&gfm, pcom, pdes[i], 1, size_comp_buff, 1);
                }
            }
            //galois_w16_region_multby_2_64(pcom , size_comp_buff);
            gfm.multiply_region.w32(&gfm, pcom, pcom, 2, size_comp_buff, 0);
        }
    }
/*
    gettimeofday(&t_end, NULL);
    printf("_internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
*/
    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}

void w32coding( const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint32_t val;
    uint32_t mask_ = 1<<(32-1);
    uint32_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    bool start_2;
    unsigned char * psrc;
    unsigned char * pdes;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    ntime t_begin;
    gettimeofday(&t_begin, NULL);

    for(int i = 0 ; i < m ; ++i){
        pdes = out_buff + i*size_comp_buff;
        for(int j = 0 ; j < k ; ++j){
            val = mat.ele32[i*k + j];
            memset(pcom , 0 , size_comp_buff);
            psrc = in_buff + j*size_comp_buff;
            start_2 = false;
            
            if(0 == val){continue;}
            if(1 == val){galois_region_xor(psrc, pdes, pdes, size_comp_buff); continue;}
            
            for(mask = mask_ ; 0 < mask; mask >>= 1){
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
                        galois_w32_region_multby_2_64(pcom , size_comp_buff);
                    }
                }
            }
            galois_region_xor(pcom , pdes , pdes , size_comp_buff);
        }
    }
    /*
    gettimeofday(&t_end, NULL);
    printf("internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
   */
    free(pcom);
}

void w32coding_( const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint32_t val;
    uint32_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int *)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            uint32_t val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }

    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift < h_index[j]; shift++, mask<<=1){
            for(i = 0; i < m; i++){
                val = mat.ele32[i*k+j];
                if(mask&val){
                    galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                }
            }
            galois_w32_region_multby_2_64(pcom , size_comp_buff);
        }
    }

    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}

void w32coding_opt( const GMatrix& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff,
                gf_t gfm){
    uint32_t val;
    uint32_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int *)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            uint32_t val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }

/*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift < h_index[j]; shift++, mask<<=1){
            for(i = 0; i < m; i++){
                val = mat.ele32[i*k+j];
                if(mask&val){
                    //galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                    gfm.multiply_region.w32(&gfm, pcom, pdes[i], 1, size_comp_buff, 1);
                }
            }
            //galois_w32_region_multby_2_64(pcom , size_comp_buff);
            gfm.multiply_region.w32(&gfm, pcom, pcom, 2, size_comp_buff, 0);
        }
    }
/*
    gettimeofday(&t_end, NULL);
    printf("_internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
*/
    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}

void w64coding( const GMatrix64& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint64_t val;
    uint64_t mask_ = 1UL<<(64-1);
    uint64_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    bool start_2;
    unsigned char * psrc;
    unsigned char * pdes;

    pcom = (unsigned char *)malloc(size_comp_buff);
    memset(out_buff , 0 , size_in_buff*m/k);
/*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(int i = 0 ; i < m ; ++i){
        pdes = out_buff + i*size_comp_buff;
        for(int j = 0 ; j < k ; ++j){
            
            val = mat.ele[i*k + j];
            memset(pcom , 0 , size_comp_buff);
            psrc = in_buff + j*size_comp_buff;
            start_2 = false;
            
            if(0 == val){continue;}
            if(1 == val){galois_region_xor(psrc, pdes, pdes, size_comp_buff); continue;}
            
            for(mask = mask_; 0 < mask; mask >>= 1){
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
                    galois_w64_region_multby_2(pcom , size_comp_buff);
                }else{
                    // the shift bit is 0
                    if(start_2){
                        galois_w64_region_multby_2(pcom , size_comp_buff);
                    }
                }
            }
            galois_region_xor(pcom , pdes , pdes , size_comp_buff);
        }
    }
    /*gettimeofday(&t_end, NULL);
    printf("internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
*/
    free(pcom);
}

void w64coding_( const GMatrix64& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff ){
    uint64_t val;
    uint64_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;

    pcom = (unsigned char *)malloc(size_comp_buff*sizeof(unsigned char));
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int *)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            uint64_t val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1UL;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }

/*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift < h_index[j]; shift++, mask<<=1UL){
            for(i = 0; i < m; i++){
                val = mat.ele[i*k+j];
                if(mask&val){
                    galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                }
            }
            galois_w64_region_multby_2(pcom , size_comp_buff);
        }
    }
/*
    gettimeofday(&t_end, NULL);
    printf("_internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
*/
    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}


void w64coding_opt( const GMatrix64& mat, 
                unsigned char * const out_buff, 
                unsigned char * const in_buff, 
                const unsigned long& size_in_buff,
                gf_t gfm){
    uint64_t val;
    uint64_t mask;
    unsigned char * pcom;
    int k = mat.cc;
    int m = mat.rr;
    unsigned long size_comp_buff = size_in_buff/k;
    unsigned char ** psrc;
    unsigned char ** pdes;
    int *h_index;
    int i, j, shift;

    pcom = (unsigned char *)malloc(size_comp_buff*sizeof(unsigned char));
    memset(out_buff , 0 , size_in_buff*m/k);
    psrc = (unsigned char **)malloc(k*sizeof(unsigned char *));
    pdes = (unsigned char **)malloc(m*sizeof(unsigned char *));
    h_index = (int *)malloc(k*sizeof(int));
    for(i = 0; i < k; i++){
        psrc[i] = in_buff + i*size_comp_buff; 
    }
    for(i = 0; i < m; i++){
        pdes[i] = out_buff + i*size_comp_buff; 
    }
    for(i = 0 ; i < k; i++){
        h_index[i] = 0;
        for(j = 0; j < m; j++){
            uint64_t val = mat.Get(j,i);
            int h_tmp=0;
            while(val){
                val >>= 1UL;
                ++h_tmp;
            }
            h_index[i] = MAX(h_index[i], h_tmp);
        }
    }
/*
    ntime t_begin, t_end;
    gettimeofday(&t_begin, NULL);
*/
    for(j = 0 ; j < k ; ++j){
        memcpy(pcom, psrc[j], size_comp_buff);
        for(shift = 0, mask = 1; shift < h_index[j]; shift++, mask<<=1UL){
            for(i = 0; i < m; i++){
                val = mat.ele[i*k+j];
                if(mask&val){
                    //galois_region_xor(pcom, pdes[i], pdes[i], size_comp_buff);
                    gfm.multiply_region.w64(&gfm, pcom, pdes[i], 1, size_comp_buff, 1);
                }
            }
            //galois_w64_region_multby_2(pcom , size_comp_buff);
            gfm.multiply_region.w64(&gfm, pcom, pcom,2, size_comp_buff, 0);
        }
    }
/*
    gettimeofday(&t_end, NULL);
    printf("_internal time: %04.3f ms\n", 1024*(t_end.tv_sec-t_begin.tv_sec)+(t_end.tv_usec-t_begin.tv_usec)/1024.0);
*/
    free(psrc);
    free(pdes);
    free(pcom);
    free(h_index);
}

