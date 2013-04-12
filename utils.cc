#include "nc.h"

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

/* Default d = n-1*/
bool RP_property(const GMatrix& mat, int n, int k, int beta){
    int alpha;
    GMatrix mat_;

    alpha = mat.rr/n;
    for(int i = 0; i < n; i++){
        mat_ = mat;
        mat_.Del_rows(i*alpha, alpha);
        if(false == DR_property(mat_, n-1, k, beta)){
            printf("@@ node %d false @@", i);
            return false;
        }
    }

    return true;
}

/* d nodes' repair property*/
bool DR_property(GMatrix mat, int d, int k, int beta){
    int i, ii;
    int rows = mat.rr;
    int cur;
    int idx[200][5];  //beta < 5
    int ncab = -1;
    int alpha = rows/d;
    int cab=1;  // cab = C(alpha, beta)

    if(mat.rr > 100){
        printf("Error: rows of the matrix is greater than 100!\n");
    }
    if(beta > 5){
        printf("Error: beta > 5!\n");
    }
    if(d > 39){
        printf("Error: d > 40!\n");
    }
    if(alpha > 30){
        printf("Error: alpha > 30!\n");
    }

    for(i = 0; i < beta; ++i){
        cab = cab*((alpha-i)/(i+1));
    }
    
    int a[10];   // beta<10 in alpha<30
    for(i = 0; i < beta; i++){
        a[i] = i;
    }
    cur = beta-1;

    do{
        if(a[cur]-cur <= alpha-beta){
            ++ncab;
            /**///printf("%d : ",ncab);
            for( i = 0 ; i < beta ; ++i){
                idx[ncab][i] = a[i];
                /**///printf("%d ", idx[ncab][i]);
            }
            /**///printf("\n");
            
            a[cur]++;
            continue;            
        }else{
            if(0 == cur){
                break;
            }
            a[--cur]++;
            for(i = 1; i < beta - cur; i++){
                a[cur+i]=a[cur]+i;
            }
            if(a[cur] - cur < alpha - beta)
                cur = beta - 1;
        }        
    }while(1);
    
    bool rtop = false;
    int ntag[40];    // requires d <39
    for(i = 0; i < d; ++i){
        ntag[i] = 0;
    }
    do{
        // for d nodes
        vector<int> lst;
        for(i = 0; i < d; ++i){
            for(ii = 0; ii < beta; ++ii){
                //printf("%d ",idx[ntag[i]][ii]+i*alpha);
                lst.push_back(idx[ntag[i]][ii]+i*alpha);
            }
        }
        //printf("\n");
        //GMatrix mat_t = Draw_rows(mat, lst, beta*d);
        
        // all possible k-1 nodes from d nodes
        int ccur;
        int kk= k-1;
        int aa[80];
        for(i = 0; i < kk; ++i){
            aa[i] = i;
        }
        ccur = kk-1;
                /**///NOTE(" d beta ");
                /**///for(unsigned i = 0; i < lst.size(); ++i){
                    /**///printf("%d ", lst.at(i));
                /**///}
                /**///printf("\n");
        do{
            if(aa[ccur]-ccur <= d-kk){
                //do something
                vector<int> sel;
                int iidx = 0;
                for(int npos = 0; npos < d; ++npos){
                    if((aa[iidx] == npos)&&(iidx < k)){
                        for(int jj = 0; jj < alpha; ++jj){
                            sel.push_back(npos*alpha+jj);
                        }
                        ++iidx;
                    }else{
                        for(int jj = 0; jj < beta; ++jj){
                            sel.push_back(lst[npos*beta+jj]);
                        }
                    }
                }
                //my work
                /**///for(unsigned i = 0; i < sel.size(); ++i){
                    /**///printf("%d ", sel.at(i));
                /**///}
                /**///printf("\t\t nodes:");
                /**///for(unsigned i = 0; i < kk; ++i){
                    /**///printf("%d ", aa[i]);
                /**///}
                /**///printf("\n");
                GMatrix mat_c;
                mat_c = Draw_rows(mat, sel, sel.size());
                if(Rank(mat_c) < mat_c.cc){
                    /**///printf("  nonsingular matrix !");
                    /**///getchar();
                    break;
                }
                aa[ccur]++;
                continue;

            }else{
                if(ccur == 0){
                    return true;//test all k-1 possible nodes from d nodes
                }
                --ccur;
                aa[ccur]++;
                for(int i = 1; i < kk - ccur; i++){
                    aa[ccur+i] = aa[ccur] + i;
                }
                if(aa[ccur]-ccur < d - kk){
                    ccur = kk-1;
                }
            }            
        }while(1);
        // all possible k-1 nodes from d nodes

        ++ntag[0];
        if(ntag[0] == cab){
            ntag[0] = 0;
            ++ntag[1]; 
        }
        
        for(ii = 1; ii < d; ++ii){
            if(ntag[ii] == cab){
                if(ii == d-1){
                    rtop = true;
                    break;
                }
                ++ntag[ii+1];
                ntag[ii] = 0;
            }
        }
        if(rtop){
            break;
        }
    }while(1);

    return false;
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
            printf("the %d-th node is not full-rank !\n",i);
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
            //if(a[0] == 0){
#if 0
            if(true){
                for(int y = 0; y<k; y++){
                    printf("%d  ", a[y]);
                }
                printf("rank = %d \n",rank);
            }
#endif
            if(rank != mat_chk.cc){
                //NOTE("Non-full matrix");
                //printf("the rank is %d\n", rank);
                //mat_chk.Print();
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

int NK_property_(GMatrix mat, int piece, int k){
    int i;
    int n;
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

    /* Any k nodes combine into a new matrix    *
     * which any cols of vector are idenpendent */
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
            
            if(AnyCols(mat_chk) == false){
                printf("AnyCols failed matrix RANK = %d\n",Rank(mat_chk));
                mat_chk.Print();
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

int NK_property(GMatrix64 mat, int piece, int k){
    int i;
    int n;
    int rank;
    int a[100];
    int cur;
    GMatrix64 mat_chk;
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

int NK_property(GMatrixU8 mat, int piece, int k){
    int i;
    int n;
    int rank;
    int a[100];
    int cur;
    GMatrixU8 mat_chk;
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

