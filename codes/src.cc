#include "src.h"

SRC::SRC(int nn, int kk, int ff, GMatrix mm){
    this->n = nn;
    this->k = kk;
    this->f = ff;
    this->mat = mm;
}

SRC::SRC(string confFile){
    string matstr;
    int row, col;
    int word;
    Config conf(confFile);
    
    row = conf.Read("row", 0);
    col = conf.Read("col", 0);
    this->n = conf.Read("n", 0);
    this->k = conf.Read("k", 0);
    this->f = conf.Read("f", 0);
    word = conf.Read("w", 8);
    matstr = conf.Read("matrix", matstr);
    this->mat.Make_from_string(matstr, row, col, word);

}

void printest(unsigned char* pc, int len, int breaklen){
    for(int i = 0; i < len; i++){
        printf("%d", *(pc+i));
        if(((i+1)%breaklen)== 0){
            printf("\n");
        }
    }
    printf("\n");
}

/****************
 ***  Encode  ***
 ****************/

int SRC::Encode(string ori_filename, string filepath)const{
    int i, j;
    FILE *fp_en_file;
    unsigned long size_en_file;
    unsigned char *in_buff, *out_buff, *i_b, *o_b, *p_in, *p_out;
    unsigned long size_in_buff, size_out_buff;
    unsigned long blocksize_in_buff, blocksize_out_buff;
    unsigned char *f_buff;
    unsigned long size_f_buff;
    struct stat f_info;
    Config conf;

    // Get file size
    stat(ori_filename.c_str(), &f_info);
    size_en_file = f_info.st_size;

    // Open encode file
    NOTE("Open file!");
    if(NULL == (fp_en_file = fopen(ori_filename.c_str(),"r"))){
        printf("Fail to open the encode file!\n");
        exit(-1);
    }
    
    // size_in_buff size_out_buff
    if(0 == size_en_file%(k*f)){
        size_in_buff = size_en_file;
    }else{
        size_in_buff = (size_en_file/(k*f)+1)*k*f; 
    }
    size_out_buff = (size_in_buff/(k*f))*(f+1)*n;
                               
    i_b = (unsigned char *)calloc(size_in_buff+1,1);
    in_buff = i_b;
    memset(in_buff, 0, size_in_buff);
    o_b = (unsigned char *)calloc(size_out_buff+1,1);
    out_buff = o_b;
    memset(out_buff, 0, size_out_buff);
    printf("The file size/in_buff/out_buff are %lu/%lu/%lu\n",size_en_file,size_in_buff,size_out_buff);

    // blocksize_in_buff and blocksize_out_buff
    blocksize_in_buff = size_in_buff/k; 
    blocksize_out_buff = size_out_buff/n;
    printf("The blocksize_in/out_buff are %lu/%lu\n", blocksize_in_buff, blocksize_out_buff);

    // Read file into in_buff
    NOTE("Read file into in_buff");
    fread(in_buff, 1, size_en_file, fp_en_file);
    fclose(fp_en_file);

    // Encode in_buff -> out_buff
    // copy in_buff ->
    NOTE("Copying in_buff -> out_buff");
    for(i = 0; i < k;i++){
        p_in = in_buff + (blocksize_in_buff*i);
        p_out = out_buff + (blocksize_out_buff*i);
        memcpy(p_out, p_in, blocksize_in_buff);
    }
    NOTE("mem copy");
    NOTE("in_buff");
    printest(in_buff, size_in_buff, blocksize_in_buff);
    NOTE("out_buff");
    printest(out_buff, size_out_buff, blocksize_out_buff);

    // MDS coding
    NOTE("MDS Coding");
    switch(mat.ww){
        case 8:
            for(i = 0; i < (this->n - this->k); i++){
                for(j = 0; j < this->k; j++){
                    unsigned char* pmat_in_buff = in_buff + j*blocksize_in_buff;
                    int multi_num = mat.Get(i,j);
                    unsigned char* pmat_out_buff = out_buff + (i+k)*blocksize_out_buff;
                    galois_w08_region_multiply(pmat_in_buff, multi_num, blocksize_in_buff, pmat_out_buff, 1);   
                    printf(">>>mat(%d,%d)=%d * %d", i, j, mat.Get(i,j), *(pmat_in_buff));
                    NOTE("out_buff");
                    printest(out_buff, size_out_buff, blocksize_out_buff);            
                }
            }
            break;
        case 16:
            for(i = 0; i < (this->n - this->k); i++){
                for(j = 0; j < this->k; j++){
                    galois_w16_region_multiply(in_buff + j*blocksize_in_buff, mat.Get(i,j), blocksize_in_buff, out_buff+(i+k)*blocksize_out_buff, 1); 
                }
            }
            break;
        case 32:
            for(i = 0; i < (this->n - this->k); i++){
                for(j = 0; j < this->k; j++){
                    galois_w32_region_multiply(in_buff+j*blocksize_in_buff, mat.Get(i,j), blocksize_in_buff, out_buff+(i+k)*blocksize_out_buff, 1);
                }
            }
            break;
    }
    NOTE("in_buff");
    printest(in_buff, size_in_buff, blocksize_in_buff);
    NOTE("out_buff");
    printest(out_buff, size_out_buff, blocksize_out_buff);
    // f -> f+1 code
    NOTE("f -> f+1 code");
    size_f_buff = blocksize_in_buff/f;
    f_buff = (unsigned char *)calloc(size_f_buff, 1);

    unsigned char *p_f_block;
    for(i = 0; i < this->n; i++){
        memset(f_buff, 0, size_f_buff);
        for(j = 0; j < this->f; j++){
            p_f_block = out_buff+((i+j)%n)*blocksize_out_buff+j*size_f_buff;
            printf("%d %d\t\t",(i+j)%n,j);
            galois_region_xor(p_f_block, f_buff, f_buff, size_f_buff);
        }
        printf("\n");
    unsigned char* p_f_xor = out_buff+((1+(i+f)%n)*blocksize_out_buff-size_f_buff);
        memcpy(p_f_xor, f_buff, size_f_buff);
    }
    NOTE("in_buff");
    printest(in_buff, size_in_buff, blocksize_in_buff);
    NOTE("out_buff");
    printest(out_buff, size_out_buff, blocksize_out_buff);
    
    if(filepath.at(filepath.length()-1) == '/'){
        ;
    }
    else{
        filepath = filepath.append(1,'/');
    }
    NOTE("Bat write");
    Bat_Write(ori_filename, this->n, out_buff, blocksize_out_buff, filepath);
    free(f_buff);
    free(i_b);
    free(o_b);
    NOTE("buffer freed");

    return n;
}

/**************
 *** Decode ***
 **************/

int SRC::Decode(string encoded_filepath, string encoded_filename, string files_idx)const{
    vector <int> file_list;
    int file_amount;
    int file_num;
    struct stat file_info;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned long size_each_file;
    unsigned long size_segment;
    unsigned long size_buff;
    FILE * fp;
    
    // Encode matrix is empty
    if(1 == this->mat.Empty()){
        printf("Encode Matrix is not initialized!\n");
        return -1;
    }

    // files_idx -> file_list
    stringstream ss(files_idx);
    while(!ss.eof()){
        ss >> file_num;
        file_list.push_back(file_num);
    }

    // less than k files can't reconstruct the file
    file_amount = file_list.size();
    cout<<"files' number:"<<file_amount<<endl;
    if(file_list.size() < k){
        printf("Less than k files");
        return -1;
    }

    // 0-(k-1) files in the file_list judgement
    bool sys_ = true;
    for(int i = 0; i < k; i++){
        int j = 0;
        for( ; j < file_list.size(); j++){
            if(i == file_list.at(j)){
                break;
            }else{
                continue;
            }
        }
        if(j >= file_list.size()){
            sys_ = false;
        }
    }

    // file size
    int first_num = file_list.at(0);
    char first_filename[128];
    sprintf(first_filename, "%s%c%02d", (encoded_filepath + encoded_filename).c_str(), '_', first_num);
    stat(first_filename, &file_info);
    size_each_file = file_info.st_size;
    cout<<"The file "<<first_filename<<" size is:"<<size_each_file<<endl;
    
    // in_buff
    size_segment = size_each_file*f/(f+1);
    size_buff = size_segment*k;
    in_buff = (unsigned char*)malloc(size_buff+1);
    out_buff = (unsigned char*)malloc(size_buff+1);
    memset(out_buff, 0 , size_buff);

    // Read into the in_buffer
    string files_idx_k;
    for(int i = 0; i < this->k; i++){
        if(i != 0){
            files_idx_k = files_idx_k + ' ';
        }
        stringstream ss_idx;
        ss_idx << file_list.at(i);
        files_idx_k = files_idx_k + ss_idx.str();
    }
    
    cout<<"files_idx_k >>"<<files_idx_k<<"<< "<<endl;    
    Bat_Read(encoded_filename, files_idx_k, in_buff, size_segment);
    NOTE("in_buff");
    printest(in_buff, size_buff, size_segment);

    sprintf(first_filename, "%s", encoded_filename.c_str());
    printf("decode the file: %s\n",first_filename);
    
    // 0-k yes
    if(sys_){
        NOTE("sys_");        
        // decoded file
        if((fp = fopen(first_filename,"w")) == NULL){
            throw File_can_not_create(first_filename);
        }
        NOTE("in_buff");
        printest(in_buff, size_buff, size_segment);
        fwrite(in_buff, k*size_segment, 1, fp);
        fclose(fp);
        
    }
    else{  // 0-k no
        NOTE("non-sys_");
        GMatrix mat_en_a;
        GMatrix mat_en;
        GMatrix mat_de;

        int tmp_c = mat.cc;
        int tmp_w = mat.ww;
        mat_en_a.Make_identity(tmp_c, tmp_c, tmp_w);
        mat_en_a.Print();
        printf("rr--%d, cc--%d, ww--%d, test\n", mat_en_a.rr, mat_en_a.cc, mat_en_a.ww);
        mat_en_a.Append_matrix(this->mat);
        NOTE("mat_en_all: encoding matrix");
        mat_en_a.Print();
        mat_en = Draw_rows(mat_en_a, files_idx, k);
        NOTE("mat_en: encoding matrix(partly)");
        mat_en.Print();
        mat_de = Inverse(mat_en);
        NOTE("mat_de: decoding matrix");
        mat_de.Print();
        
        switch(mat_de.ww){
            case 8:
                for(int i = 0; i < this->n; ++i){
                    for(int j = 0; j < this->k; ++j){
                        galois_w08_region_multiply(in_buff+j*size_segment, mat_de.Get(i,j), size_segment, out_buff+i*size_segment, 1);
                    }
                }
                break;
            case 16:
                for(int i = 0; i < this->n; ++i){
                    for(int j = 0; j < this->k; ++j){
                        galois_w16_region_multiply(in_buff+j*size_segment, mat_de.Get(i,j), size_segment, out_buff+i*size_segment, 1);
                    }
                }
                break;
            case 32:
                for(int i = 0; i < this->n; ++i){
                    for(int j = 0; j < this->k; ++j){
                        galois_w32_region_multiply(in_buff+j*size_segment, mat_de.Get(i,j), size_segment, out_buff+i*size_segment, 1);
                    }
                }
                break;
        }        


        // Write into the file
        if((fp = fopen(first_filename,"w")) == NULL){
            throw File_can_not_create(first_filename);
        }
        NOTE("out_buff");
        printest(out_buff, size_buff, size_segment);
        fwrite(out_buff, this->fileLen, 1, fp);
        fclose(fp);
    
    }

    free(in_buff);
    free(out_buff);

    return k;
}

int SRC::Repair(string encoded_filename, int vnode)const{
    string rest_idx;
    struct stat file_info;
    unsigned long size_each_file;
    unsigned char *in_buff;
    
    // rest file index 
    for(int i = 0; i < this->n; i++){
        stringstream ssint;
        if(i == 0){
            if(vnode == 0){
                continue;
            }else{
                ssint << i;
                ssint >> rest_idx;
                continue;
            }
        }
        if((i == 1)&&(vnode == 0)){
            ssint << i;
            ssint >> rest_idx;
            continue;
        }
        if(vnode == i){
            continue;
        }else{
            string str_int;
            ssint<<i;
            str_int = ssint.str();
            rest_idx = rest_idx+' '+str_int;
        }
        cout<<"for rest_idx>>"<<rest_idx<<endl;
    }
    cout<<"The index string: >>"<<rest_idx<<"<<"<<endl;
    
    // file size
    stringstream ss(rest_idx);
    int first_num;
    ss >> first_num;
    char first_filename[128];
    sprintf(first_filename, "%s%c%02d", (encoded_filename).c_str(), '_', first_num);
    stat(first_filename, &file_info);
    size_each_file = file_info.st_size;
    cout<<"The file "<<first_filename<<" size is:"<<size_each_file<<endl;
    
    // in_buff
    in_buff = (unsigned char *)malloc(this->n * size_each_file);
    memset(in_buff, 0, this->n*size_each_file);
    NOTE("Read repair file into the memory buffer!");
    Bat_Read(encoded_filename, rest_idx, in_buff, size_each_file);
    printest(in_buff , this->n*size_each_file , size_each_file);

    // repair in the in_buff
    unsigned char* p_des = in_buff + vnode*(size_each_file);
    int fsize_in_buff = size_each_file/(f+1);
    unsigned char* pf_buff;
    pf_buff = (unsigned char *)malloc(fsize_in_buff);
    for(int repair_f_idx = 0; repair_f_idx <= f; repair_f_idx++){
        memset(pf_buff, 0, fsize_in_buff);
        int xor_begin = (n + vnode - repair_f_idx)%n;
        
        // xor_n_idx and xor_f_idx dicide the block to XOR
        int xor_f_idx = 0;
        int xor_n_idx = xor_begin;
        for(; xor_f_idx <= f; xor_f_idx++){
            xor_n_idx = (xor_begin + xor_f_idx)%n;
            if(xor_n_idx == vnode){
                continue;
            }
            unsigned char * p_xor = in_buff + size_each_file*xor_n_idx + fsize_in_buff*xor_f_idx; 
            printf("xor_n_idx = %d, xor_f_idx = %d\n", xor_n_idx, xor_f_idx);
            printest(p_xor, fsize_in_buff, fsize_in_buff);
            galois_region_xor(p_xor, pf_buff, pf_buff, fsize_in_buff);
        }
        memcpy(p_des + repair_f_idx*fsize_in_buff, pf_buff, fsize_in_buff);
    }

    NOTE("Repaired!");
    printest(in_buff, n*size_each_file, size_each_file);
    
    // repair file
    FILE * fp_repair;
    sprintf(first_filename, "%s%c%02d", (encoded_filename).c_str(), '_', vnode);
    if((fp_repair = fopen(first_filename, "w")) == NULL){
        printf("Cann't open file %s\n",first_filename);
    }
    fwrite(in_buff+vnode*size_each_file, size_each_file, 1, fp_repair);
    fclose(fp_repair);
    free(in_buff);

    return vnode;
}

//Get class member values
int SRC::GetN()const{
    return n;
}
int SRC::GetK()const{
    return k;
}
int SRC::GetF()const{
    return f;
}
int SRC::GetFilelen()const{
    return fileLen;
}
GMatrix SRC::GetMat()const{
    return mat;
}

//Set class member values
int SRC::SetN(const int& n_set){
    n = n_set;
    
    return n;
}
int SRC::SetK(const int& k_set){
    k = k_set;

    return k;
}
int SRC::SetF(const int& f_set){
    f = f_set;

    return f;
}
int SRC::SetMat(const GMatrix& mat_set){
    mat = mat_set;

    return mat.rr;
}

//Print SRC class member values
void SRC::SRCmeta_print()const{
    cout<<">> n = "<<n<<endl;
    cout<<">> k = "<<k<<endl;
    cout<<">> f = "<<f<<endl;
    mat.Print();
}
