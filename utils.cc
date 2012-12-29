#include "nc.h"

DLLEXPORT int NC_random(int max){
    return rand()%max;
}

/* Return second level time interval */
double dt_s(struct timeval end,struct timeval start){
    double ts;
    ts=(end.tv_sec-start.tv_sec)+((end.tv_usec-start.tv_usec)/1000)/1000;   
    return ts;
}


/* Return millisecond level time interval */
double dt_ms(struct timeval end,struct timeval start){
    double tm;
    tm=((end.tv_sec-start.tv_sec)*1000.0)+((end.tv_usec-start.tv_usec)*1.0)/1000;   
    return tm;
}


/* Return microsecond level time interval */
double dt_us(struct timeval end,struct timeval start){
    double tu;
    tu=((end.tv_sec-start.tv_sec)*1000.0)*1000.0+(end.tv_usec-start.tv_usec);   
    return tu;
}

void mem_print_(unsigned char* pc, int len, int breaklen){
    for(int i = 0; i < len; i++){
        printf("%d ", *(pc+i));
        if(((i+1)%breaklen)== 0){
            printf("\n");
        }
    }
    printf("\n");
}

void mem_print(unsigned char* pc, int len, int breaklen){
    for(int i = 0; i < len; i++){
        printf("%d", *(pc+i));
        if(((i+1)%breaklen)== 0){
            printf("\n");
        }
    }
    printf("\n");
}

int Get_file_len(const string& filename){
    struct stat file_info;
    char filename_f[128];
    FILE *fp;

    sprintf(filename_f , "%s" , filename.c_str());
    if((fp = fopen(filename_f , "r"))){
        fclose(fp);
        stat(filename_f, &file_info);
        return file_info.st_size;
    }else{
        throw File_not_found(filename.c_str());
    }
}

/* Encode/Decode the data using GMatrix matrix         *
 * mat is the Encode/Decode matrix                      *
 * p_des store the data after Encode/Decode             *
 * p_src store the data before Encode/Decode            *
 * length represent the length of the data to deal with *
 * p_des, p_src must be long-word aligned               */
int matrix_coding(const GMatrix& mat, unsigned char * const p_des, unsigned char * const p_src, const int& length){
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

DLLEXPORT int NC_code_py(char *mat_c_p, int r, int c, int w, char *p_des, char *p_src, int length){
    int *mat_i_p;
    GMatrix mat;
    
    mat_i_p = (int *)mat_c_p;
    mat.Make_from_list(mat_i_p, r, c, w);
    matrix_coding(mat, (unsigned char *)p_des, (unsigned char *)p_src, length);
    return 1;
}


/* Encode a file using mat_en matrix                    *
 * fp_src points to the file to be encoded              *
 * fpp_des point to the files where encoded data store  */
int NC_encode_file(GMatrix mat_en, FILE *fp_src, FILE **fpp_des){
    return 1;
}
 
/* Decode files to a file                               *
 * fpp_src point to the files to be decoded             *
 * fp_des point to the file where decoded data store    */
int NC_decode_file(GMatrix mat_de, FILE **fpp_src, FILE *fp_des){
    return 1;
}


/* NK_property checks the system combining the new node *
 * satisfing (n,k) property, this is, k out of n nodes  *
 * can reconstruct the original data                    *
 * piece : how many rows of matrix mat in a node        */
int NK_property(GMatrix mat, int piece, int k){
    int i;
    int n;
    int rank;
    int a[100];
    int cur;
    GMatrix mat_chk;
    vector<int> shift;   /* shift used to move k non-zero elements *
                          * and store the result                   */ 

    assert(0 == mat.rr%piece);
    n = (mat.rr/piece);
    assert(k < n);

    /* Any node matrix is full rank */
    for(i = 0; i < n; i++){
        mat_chk = Slice_matrix(mat, i*piece, piece);
        if(0 == Is_full(mat_chk)){
            return 0;
        }
    }

    /* Any k nodes combine into a new matrix *
     * whose rank must equal its cols        */
    for(i = 0; i < k; i++){
        a[i] = i;
    }
    cur = k-1;
    do{
        if(a[cur]-cur <= n-k){
            for(i = 0; i < k; i++){
                if(0 == i){
                    mat_chk = Slice_matrix(mat, a[i]*piece, piece);
                }else{
                    mat_chk.Append_matrix(mat, a[i]*piece, piece); 
                }
            }
            
            rank = Rank(mat_chk);
            if(rank != mat_chk.cc){
                return 0;
            }
            a[cur]++;
            continue;
        }else{
            if(0 == cur){
                break;
            }
            a[--cur]++;
            for(i = 1; i < k - cur; i++){
                a[cur+i]=a[cur]+i;
            }
            if(a[cur] - cur < n - k)
                cur = k - 1;
        }
    }while(1);
    return 1;
}

// Not consider memory overflow in buff
int Bat_Write(  const string& filename, 
                const int& filenum, 
                unsigned char* out_buff, 
                const unsigned long& size_each_file,
                const string& filepath
                ){
    FILE * fp;
    int i;
    char c_filename[128];
    string filename_full;
    unsigned char* p_f_buff;
    
    filename_full = filepath+filename;
    for(i = 0; i < filenum; i++){
        sprintf(c_filename, "%s%c%02d", filename_full.c_str(), '_', i);
        if(NULL == (fp = fopen(c_filename, "w"))){
            throw File_can_not_create(c_filename);
        }
        p_f_buff = out_buff;
        p_f_buff = p_f_buff  + i*size_each_file;
        fwrite(p_f_buff, size_each_file, 1, fp);
        fclose(fp);
    }

    return filenum;
}

int Bat_Write(  const string& filename,
                const vector<int>& files_idx_list,
                unsigned char * out_buff,
                const unsigned long& size_each_file,
                const string& filepath
            ){
    FILE *fp;
    int amount_files = files_idx_list.size();
    char c_filename[128];
    unsigned char* p_f_buff = out_buff;

    for(int i = 0 ; i < amount_files ; ++i){
        sprintf(c_filename , "%s%c%02d" , (filepath+filename).c_str() , '_' , files_idx_list.at(i));    
        if(NULL == (fp = fopen(c_filename , "w"))){
            throw File_not_found(c_filename);
        }
        p_f_buff = p_f_buff + i*size_each_file;
        fclose(fp);
    }

    return amount_files;
}

int Bat_Read(   const string& filename,
                const vector<int>& files_idx_list,
                unsigned char * in_buff,
                const unsigned long& size_each_file
            ){
    FILE *fp;
    int amount_files = files_idx_list.size();
    char c_filename[128];

    for(int i = 0 ; i < amount_files ; ++i){
        sprintf(c_filename , "%s%c%02d" , filename.c_str() , '_' , files_idx_list.at(i));    
    
        if(NULL == (fp = fopen(c_filename , "r"))){
            throw File_not_found(c_filename);
        }
        fread(in_buff + i*size_each_file , size_each_file , 1 , fp);
        fclose(fp);
    }

    return amount_files;
}

// Deprecated
// Not consider memory overflow in buff
int Bat_Read(   string filename, 
                string files_index, 
                unsigned char* in_buff, 
                unsigned long size_each_file
                ){
        stringstream ss_index(files_index);
        int file_i;
        int ins = 0;
        char c_filename[128];
        FILE* fp;

        while(!ss_index.eof()){
            ss_index >> file_i;
            sprintf(c_filename, "%s%c%02d", filename.c_str(), '_', file_i);
            if(NULL == (fp = fopen(c_filename, "r"))){
                throw File_not_found(c_filename);
            }
            fread(in_buff+ins*size_each_file, size_each_file, 1, fp);
            ins++;
            fclose(fp);
        }
        return ins;
}
int Bat_Delete( string filename,
                string files_index
                ){
    stringstream ss_index(files_index);
    int file_i;
    int ins = 0;
    char c_filename[128];

    while(!ss_index.eof()){
        ss_index >> file_i;
        sprintf(c_filename, "%s%c%02d", filename.c_str(), '_', file_i);
        if(remove(c_filename)!=0){
            printf("Error deleting file");
        }
        ins++;
    }
    return ins;
}


int Bat_Delete( const string& filename,
                const vector<int>& files_idx_list
        ){
    int amount_files = files_idx_list.size();
    char c_filename[128];

    for(int i = 0 ; i < amount_files ; ++i){
        sprintf(c_filename, "%s%c%02d", filename.c_str(), '_', files_idx_list.at(i));
        if(remove(c_filename)!=0){
            printf("Error deleting file");
        }
    }

    return amount_files;
}

void fsread_buff(const string& filename, unsigned char* p_buff, const unsigned long& len){
    FILE *fp;
    unsigned char * pb = p_buff;
    char filename_[128];
    
    sprintf(filename_ , "%s" , filename.c_str());
    if(NULL != (fp = fopen(filename_ , "r"))){
        fread(pb , len , 1 , fp);
        fclose(fp);
    }else{
        throw File_not_found(filename_);
    }
}

void fswrite_buff(const string& filename, unsigned char* p_buff, const unsigned long& len){
    FILE *fp;
    unsigned char * pb = p_buff;
    char filename_[128];
    
    sprintf(filename_ , "%s" , filename.c_str());
    if(NULL != (fp = fopen(filename_ , "w"))){
        fwrite(pb , len , 1 , fp);
        fclose(fp);
    }else{
        throw File_not_found(filename_);
    }
}

void fswrite_buff(const string& filename, const int num , unsigned char *p_buff , const long& len){
    FILE *fp;
    unsigned char *pb = p_buff;
    char filename_[128];

    sprintf(filename_ , "%s%c%02d" , filename.c_str() , '_' , num);
    if(NULL != (fp = fopen(filename_ , "w"))){
        fwrite(pb , len , 1 , fp);
        fclose(fp);
    }else{
        throw File_not_found(filename_);
    }
}
