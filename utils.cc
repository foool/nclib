#include "nc.h"
#include "utils.h"
/*
gf_t gf_complete_init(int w){
    gf_t gfm;
    if(w == 16){
        printf("w = 16\n");
        if (!gf_init_easy(&gfm, w)) {
            printf("Bad gf spec\n");
            exit(1);
        }
    }

#if 0
    if (w == 16 || w == 32) {
        if (!gf_init_hard(&gfm, w, GF_MULT_SPLIT_TABLE, GF_REGION_ALTMAP | GF_REGION_SSE, GF_DIVIDE_DEFAULT, 0, 4, w, NULL, NULL)) {
            printf("Bad gf spec\n");
            exit(1);
        }
    } else if (w == 8 || w == 64 || w == 128 || w == 4) {
        if (!gf_init_easy(&gfm, w)) {
            printf("Bad gf spec\n");
            exit(1);
        }
    } else {
        printf("Not supporting w = %d\n", w);
    }
#endif

    return gfm;
}
*/
DLLEXPORT int NC_random(int max){
    return rand()%max;
}

int NC_random(int min, int max){
    return rand()%(max-min)+min;
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
        printf("%03d ", *(pc+i));
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
    if(mat_en.rr == 0 || fp_src == NULL || fpp_des == NULL){
        return 1;
    }
    return 1;
}
 
/* Decode files to a file                               *
 * fpp_src point to the files to be decoded             *
 * fp_des point to the file where decoded data store    */
int NC_decode_file(GMatrix mat_de, FILE **fpp_src, FILE *fp_des){
    if(mat_de.rr == 0 || fpp_src == NULL || fp_des == NULL){
        return 1;
    }
    return 1;
}

string _Trim(string ss){
    static const char whitespace[] = " \n\t\v\r\f";
    
    ss.erase(0, ss.find_first_not_of(whitespace));
    ss.erase(ss.find_last_not_of(whitespace)+1);

    return ss;
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
    //printf("NOTE1\n");
    for(int i = 0 ; i < amount_files ; ++i){
        sprintf(c_filename , "%s%c%02d" , (filepath+filename).c_str() , '_' , files_idx_list.at(i));    
                                            //printf("%s\n", c_filename);
        if(NULL == (fp = fopen(c_filename , "w"))){
            throw File_not_found(c_filename);
        }
        p_f_buff = out_buff;
        p_f_buff = p_f_buff + i*size_each_file;
        fwrite(p_f_buff, size_each_file, 1, fp);
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
        
        files_index = _Trim(files_index);
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

    files_index = _Trim(files_index);
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

void sort_v_i(vector<int> &v){
    //size of vector is small, unnecessary to use recursive quick sort algorithm
    int size_v = v.size();
    int i,j;
    int temp;

    for(i = 0; i < size_v; ++i){
        for(j = i; j < size_v; ++j){
            if(v.at(i) > v.at(j)){
                temp = v.at(i);
                v.at(i) = v.at(j);
                v.at(j) = temp;
            }
        }
    }
}

// from 0,1,...,inall-1 , combination of a.size() numbers 
bool next_combination(vector<int> &a, int inall){
    int size;
    int i;
    int value;

    size = a.size();
    // 0 1 2 3 4 
    if(a.at(0) >= (inall-size)){
        return false;
    }

    for(i = size-1; i > 0; --i){
        if(a.at(i) < inall-(size-i)){
            break;
        }
    }
    
    value = a.at(i);
    for(;i < size; ++i){
        ++value;
        a.at(i) = value;
    }
    return true;
}

bool next_permutation(vector<int> &v){
    int size;
    int i, j;
    int temp;

    size = v.size();
    //stop at the first place where the latter element is greater than the previous element
    for(i = size-1; i > 0&&v.at(i)<v.at(i-1); --i);

    if(0 == i) return false; //at the end of the permutation

    //find first element after v.at(i-1) greater than it
    for(j = size-1; j > i&&v.at(j)<v.at(i-1); --j);

    temp = v.at(j);
    v.at(j) = v.at(i-1);
    v.at(i-1) = temp;
    
    //inverse from v.at(i) to v.at(size-1)
    for(j = size-1; i < j; --j, ++i){
        temp = v.at(i);
        v.at(i) = v.at(j);
        v.at(j) = temp;
    }

    return true;
}
