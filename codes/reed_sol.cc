#include "reed_sol.h"

Reed_Sol::Reed_Sol(int n_nodes, int k_nodes){
    GMatrix mat_t;

    this->n = n_nodes;
    this->k = k_nodes;
    mat_t.Make_sys_vandermonde(this->n, this->k, 8);
    this->mat = mat_t;
}

Reed_Sol::Reed_Sol(int n_nodes, int k_nodes, int word){
    GMatrix mat_t;

    this->n = n_nodes;
    this->k = k_nodes;
    this->mat.Make_sys_vandermonde(this->n, this->k, word);
}

Reed_Sol::Reed_Sol(int n_nodes, int k_nodes, GMatrix mat_t){
    this->n = n_nodes;
    this->k = k_nodes;
    this->mat = mat_t;
}

Reed_Sol::Reed_Sol(string conf_file){
    string mat_str;
    int row, col;
    int word = 8;
    FILE *fp;
    Config conf(conf_file);
    
    // n , k 
    this->n = conf.Read("n" , 0);
    this->k = conf.Read("k" , 0);
    row = n;
    col = k;

    // filename
    this->filename = conf.Read("filename" , string(""));
    
    // file_len
    if(conf.KeyExist(string("file_len"))){
        this->file_len = conf.Read("file_len", -1);
    }else{
        if(filename.empty()){
            this->file_len = -1;
        }else{
            struct stat file_info;
            char filename_f[128];

            sprintf(filename_f , "%s%s" , (this->en_from_filepath).c_str(),(this->filename).c_str());
            if((fp = fopen(filename_f,"r"))){
                fclose(fp);
                stat(filename_f, &file_info);
                this->file_len =  file_info.st_size;
            }else{
                throw File_not_found((en_from_filepath+filename).c_str());
            }
        }
    }

    // path
    if(conf.KeyExist(string("en_from_path"))){
        this->en_from_filepath = conf.Read("en_from_path", string("./"));
    }else{
        this->en_from_filepath.assign("./");
    }
    if(conf.KeyExist(string("en_to_path"))){
        this->en_to_filepath = conf.Read("en_to_path", string("./"));
    }else{
        this->en_to_filepath.assign("./");
    }
    if(conf.KeyExist(string("de_from_path"))){
        this->de_from_filepath = conf.Read("de_from_path", string("./"));
    }else{
        this->de_from_filepath.assign("./");
    }
    if(conf.KeyExist(string("de_to_path"))){
        this->de_to_filepath = conf.Read("de_to_path", string("./"));
    }else{
        this->de_to_filepath.assign("./");
    }

    // mat
    if(conf.KeyExist(string("mat"))){
        if(conf.KeyExist(string("w"))){
            word = conf.Read("w" , 8);
        }else{
            word = 8;
        }
        mat_str = conf.Read("mat", mat_str);
        this->mat.Make_from_string(mat_str, row, col, word);
    }else{
        this->mat.Make_sys_vandermonde(row, col, word);
    }

}
    
int Reed_Sol::Encode()const{
    string en_from_filename = this->en_from_filepath + this->filename;
    string en_to_filename = this->en_to_filepath + this->filename;
    unsigned char *in_buff, *out_buff, *i_b, *o_b;
    int size_encoded;
    int size_in_buff, size_out_buff;
    
    // in_buff and out_buff
    if(0 == (this->file_len % 64)){
        size_in_buff = this->file_len;
    }else{
        size_in_buff = (this->file_len/64 + 1)*64;
    }
    size_out_buff = size_in_buff*(this->mat.rr)/(this->mat.cc);
    
    i_b = (unsigned char *)malloc(size_in_buff);
    in_buff = i_b;
    o_b = (unsigned char *)malloc(size_out_buff);
    out_buff = o_b;
    
    // en_from_filename -> in_buff
    if(this->file_len < 1){
        throw File_Length_Error(this->file_len);
    }
    fread_buff(en_from_filename , in_buff , this->file_len);

    // in_buff -> out_buff
    matrix_coding(this->mat , out_buff , in_buff , size_in_buff);   
    

    // out_buff -> encoded files
    size_encoded = this->file_len/this->mat.cc;
    Bat_Write(this->filename , this->mat.rr , out_buff , size_encoded , en_to_filepath);

    NOTE("in_buff");
    mem_print(in_buff, size_in_buff, size_encoded);
    NOTE("out_buff");
    mem_print(out_buff, size_out_buff, size_encoded);
    free(i_b);
    free(o_b);

    return 1;
}

int Reed_Sol::Decode(const string& files_idx)const{
    vector <int> files_idx_list;
    string de_from_filename = this->de_from_filepath + this->filename;
    string de_to_filename = this->de_to_filepath + this->filename;
    unsigned char *in_buff, *out_buff, *i_b, *o_b;
    int size_in_buff, size_out_buff;
    int size_each_file;
    
    // files_idx to file_idx_list
    int file_num;
    stringstream ss(files_idx);
    while(!ss.eof()){
        ss >> file_num;
        files_idx_list.push_back(file_num);
    }

    // more than k files to decode
    int files_amount = files_idx_list.size();
    cout<<"files_amout "<<files_amount<<endl;
    if(files_amount < k){
        printf("Less than k files");
        return -1;
    }

    // size_in_buff  and size_out_buff
    if(this->file_len > 0){
        if(0 == (this->file_len % 64)){
            size_out_buff = this->file_len;
        }else{
            size_out_buff = (this->file_len/64 + 1)*64;
        }
        size_in_buff = size_out_buff;
        size_each_file = size_in_buff/k;
    }else{
        throw File_Length_Error(this->file_len);
    }
    
    // files_idx_k
    string files_idx_k;
    for(int i = 0 ; i < this->k ; i++){
        if(i != 0){
            files_idx_k = files_idx_k + ' ';
        }
        stringstream ss_idx;
        ss_idx << files_idx_list.at(i);
        files_idx_k = files_idx_k + ss_idx.str();
    }
    cout<<"files_idx_k >>"<<files_idx_k<<"<<"<<endl;

    // buff 
    i_b = (unsigned char*)malloc(size_in_buff);
    in_buff = i_b;
    o_b = (unsigned char*)malloc(size_out_buff);
    out_buff = o_b;
    
    //Bat_read
    Bat_Read(de_from_filename , files_idx_k , in_buff , size_each_file);
    NOTE("in_buff");
    mem_print(in_buff , size_in_buff , size_each_file);

    // mat_en mat_de
    GMatrix mat_en;
    GMatrix mat_de;
    mat_en = Draw_rows(this->mat , files_idx_k , k);
    NOTE("encode matrix");
    mat_en.Print();
    mat_de = Inverse(mat_en);
    NOTE("decode matrix");
    mat_de.Print();
    
    // in_buff -> out_buff
    matrix_coding(mat_de , out_buff , in_buff , size_in_buff );
    NOTE("out_buff");
    mem_print(out_buff , size_out_buff , size_each_file);

    // out_buff -> file
    fswrite_buff(this->de_to_filepath+this->filename , out_buff , this->file_len );

    free(i_b);
    free(o_b);
    return 1;
}

// repair type(S/P/Q)
int Reed_Sol::Repair( int fnode )const{
    int repair_type = 10;
    vector<int> files_idx_list;
    unsigned char *in_buff, *out_buff;
    unsigned long size_in_buff = Get_fixsize(this->file_len , 64);
    unsigned long size_each_file = size_in_buff/mat.cc;
    unsigned long size_out_buff = size_each_file;

    cout<<"Repair the node:"<<fnode<<endl;

    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_out_buff);
    // S or P repair
    if(fnode <= mat.cc){
        if(fnode == mat.cc){
            repair_type = 11;
        }
        
        //files_idx_list
        for(int i = 0 ; i <= mat.cc ; ++i){
            if(i == fnode){
                continue;
            }
            files_idx_list.push_back(i);
        }

        // files-> in_buff
        Bat_Read(this->de_from_filepath + this->filename , files_idx_list , in_buff , size_each_file);
        NOTE("in_buff");
        mem_print(in_buff, size_in_buff, size_each_file);
        memset(out_buff, 0, size_out_buff);
        for(int i = 0 ; i < this->mat.cc ; ++i){
            unsigned char * in_p = in_buff + i*size_each_file;
            galois_region_xor(in_p, out_buff, out_buff, size_each_file);
        }
        NOTE("out_buff");
        mem_print(out_buff, size_out_buff, size_out_buff);
        // out_buff -> file
        fswrite_buff(this->de_from_filepath + this->filename , fnode , out_buff , size_each_file);
    }else{

        //Q repair
        if(fnode == (mat.cc + 1)){
            repair_type = 12;    
            for(int i = 0; i < mat.cc; ++i){
                files_idx_list.push_back(i);
            }

            // files -> in_buff
            Bat_Read(this->de_from_filepath + this->filename , files_idx_list , in_buff , size_each_file);
            mem_print(in_buff, size_in_buff, size_each_file);
            memset(out_buff, 0, size_out_buff);
            GMatrix mat_rpr = Slice_matrix(this->mat, 3, 1);
            NOTE("repair matrix");
            mat_rpr.Print();
            matrix_coding(mat_rpr, out_buff, in_buff, size_in_buff);
            NOTE("out_buff");
            mem_print(out_buff, size_out_buff, size_out_buff);
            fswrite_buff(this->de_from_filepath + this->filename , fnode , out_buff , size_each_file);
        }else{
            free(in_buff);
            free(out_buff);
            return -1;
        }
    }
    NOTE("Free buffer!");
    free(in_buff);
    free(out_buff);
    
    return repair_type;
}

int Reed_Sol::Repair( string fnodes )const{
    static const char whitespace[] = " \n\t\v\r\f";
    vector <int> fnodes_list;
    int fnode;
    unsigned char *in_buff, *out_buff;
    unsigned long size_in_buff = Get_fixsize(this->file_len , 64);
    unsigned long size_each_file = size_in_buff/mat.cc;
    int amount_failures;
    unsigned long size_out_buff;

    fnodes.erase(0, fnodes.find_first_not_of(whitespace));
    fnodes.erase(fnodes.find_last_not_of(whitespace)+1);
    stringstream ss(fnodes);
    
    in_buff = (unsigned char *)malloc(size_in_buff);
    while(!ss.eof()){
        ss >> fnode;
        fnodes_list.push_back(fnode);
    }

    if(1 == fnodes_list.size()){
        // one failure S/P/Q 
        Repair(fnodes_list.at(0));
    }else{
        // More than one failure
        if((fnodes_list.at(1) > this->mat.cc)&&(fnodes_list.at(0) < this->mat.cc)){
            // with a S failure and P is OK
            Repair(fnodes_list.at(0));
            fnodes_list.erase(fnodes_list.begin());

            amount_failures = fnodes_list.size();
            size_out_buff = amount_failures*size_each_file;
            out_buff = (unsigned char *)malloc(size_out_buff);
            vector<int> vec_k;

            for(int i = 0 ; i < this->mat.cc ; ++i){
                vec_k.push_back(i);
            }
            Bat_Read(this->de_from_filepath + this->filename , vec_k , in_buff , size_each_file);
            NOTE("In_buff");
            mem_print(in_buff, size_in_buff, size_each_file);
            memset(out_buff, 0, size_out_buff);
            GMatrix mat_en = Draw_rows(this->mat , fnodes_list , amount_failures);
            matrix_coding(mat_en , out_buff , in_buff , size_in_buff);
            Bat_Write(this->filename, fnodes_list , out_buff , size_out_buff, this->de_from_filepath);
            NOTE("out_buff S+Q");
            mem_print(out_buff, size_out_buff, size_each_file);
            free(out_buff);
        }else{
            // normal method
            NOTE("normal repair");
            amount_failures = fnodes_list.size();
            GMatrix mat_fnodes = Draw_rows(this->mat, fnodes_list, amount_failures);

            // live nodes list
            vector<int> live_nodes;
            for(int i = 0; i < mat.rr; ++i){
                live_nodes.push_back(i);
            }
            for(int i = 0; i < amount_failures; ++i){
                live_nodes.erase(fnodes_list.begin()+i);
            }
            GMatrix mat_live = Draw_rows(this->mat, live_nodes, mat.cc);
            NOTE("mat_live");
            mat_live.Print();
            GMatrix mat_live_inv = Inverse(mat_live);
            NOTE("mat_live_inv");
            mat_live_inv.Print();
            GMatrix mat_rpr = Prod(mat_fnodes , mat_live_inv);

            // out_buff
            size_out_buff = amount_failures*size_each_file;
            out_buff = (unsigned char *)malloc(size_out_buff);
            memset(out_buff, 0, size_out_buff);
            Bat_Read(this->de_from_filepath + this->filename, live_nodes, in_buff, size_each_file);
            matrix_coding(mat_rpr, out_buff, in_buff, size_in_buff);
            Bat_Write(this->filename, fnodes_list , out_buff , size_out_buff, this->de_from_filepath);
            NOTE("out_buff");
            mem_print(out_buff, size_out_buff, size_each_file);
            free(out_buff);
        }
    }
    
    free(in_buff);

    return 1;
}

int Reed_Sol::GetN()const{
    return this->n;
}

int Reed_Sol::GetK()const{
    return this->k;
}

string Reed_Sol::GetFilename()const{
    return filename;
}

int Reed_Sol::GetFilelen()const{
    return file_len;
}

GMatrix Reed_Sol::GetMat()const{
    return mat;
}

void Reed_Sol::SetN(const int& n_set){
    this->n = n_set;

}

void Reed_Sol::SetK(const int& k_set){
    this->k = k_set;

}

void Reed_Sol::SetMat(const GMatrix& mat_set){
    this->mat = mat_set;
}

void Reed_Sol::SetEnpath(const string& from_filepath, const string& to_filepath){
    this->en_from_filepath = from_filepath;
    this->en_to_filepath = to_filepath;
}

void Reed_Sol::SetDepath(const string& from_filepath, const string& to_filepath){
    this->de_from_filepath = from_filepath;
    this->de_to_filepath = to_filepath;
}

void Reed_Sol::SetFile(const string& fname, const int& flen){
    this->filename = fname;
    this->file_len = flen;
}

void Reed_Sol::SetFile(const string& fname){
    FILE *fp;
    struct stat file_info;
    char filename_f[128];

    this->filename = fname;
    sprintf(filename_f , "%s%s" , (en_from_filepath).c_str(),(fname).c_str());
    if((fp = fopen(filename_f,"r"))){
        fclose(fp);
        stat(filename_f, &file_info);
        this->file_len =  file_info.st_size;
    }else{
        throw File_not_found((en_from_filepath+fname).c_str());
    }
}



void Reed_Sol::meta_print()const{
    cout<<"n >>"<<n<<"<<"<<endl;
    cout<<"k  >>"<<k<<"<<"<<endl;
    mat.Print();
    cout<<"filename >>"<<filename<<"<<"<<endl;
    cout<<"file length >>"<<file_len<<"<<"<<endl;
    cout<<"en_from_filepath >>"<<en_from_filepath<<"<<"<<endl;
    cout<<"en_to_filepath >>"<<en_to_filepath<<"<<"<<endl;
    cout<<"de_from_filepath >>"<<de_from_filepath<<"<<"<<endl;
    cout<<"de_to_filepath >>"<<de_to_filepath<<"<<"<<endl;
}

void fread_buff(const string& filename, unsigned char* p_buff, const unsigned long& len){
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
