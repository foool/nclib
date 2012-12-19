#include "embr.h"

E_MBR::E_MBR(int n_nodes, int k_nodes){
    GMatrix mat_t;
    int row, col;

    this->n = n_nodes;
    this->k = k_nodes;
    this->w = 8;
    row = (this->n)*(this->n -1)/2;
    col = row - (this->n - this->k)*(this->n - this->k - 1)/2;
    mat_t.Make_sys_vandermonde(row, col, 8);
    this->mat = mat_t;
}

E_MBR::E_MBR(int n_nodes, int k_nodes, int word){
    GMatrix mat_t;
    int row, col;

    this->n = n_nodes;
    this->k = k_nodes;
    this->w = word;
    row = (this->n)*(this->n -1)/2;
    col = row - (this->n - this->k)*(this->n - this->k - 1)/2;
    this->mat.Make_sys_vandermonde(row, col, this->w);
}

E_MBR::E_MBR(int n_nodes, int k_nodes, int word, GMatrix mat_t){
    GMatrix mat_t;
    int row, col;

    this->n = n_nodes;
    this->k = k_nodes;
    this->w = word;
    this->mat = mat_t;
}

E_MBR::E_MBR(string conf_file){
    string mat_str;
    int row, col;
    Config conf(conf_file);
    
    // n , k 
    this->n = conf.Read("n" , 0);
    this->k = conf.Read("k" , 0);
    row = (this->n)*(this->n -1)/2;
    col = row - (this->n - this->k)*(this->n - this->k - 1)/2;

    // filename
    this->filename = conf.Read("filename" , string(""));
    
    // file_len
    if(conf.KeyExist("file_len")){
        this->file_len = conf.Read("file_len", -1);
    }else{
        this->file_len = -1;
    }

    // path
    this->en_from_filepath = conf.Read("en_from_path", string("./"))
    this->en_to_filepath = conf.Read("en_to_path", string("./"))
    this->de_from_filepath = conf.Read("de_from_path", string("./"))
    this->de_to_filepath = conf.Read("de_to_path", string("./"))

    // mat
    if(conf.KeyExist("mat")){
        if(conf.KeyExist("w")){
            this->w = conf.Read("w" , 8);
        }else{
            this->w = 8;
        }
        mat_str = conf.Read("mat", mat_str);
        this->mat.Make_from_string(mat_str, row, col, word);
    }else{
        this->mat.Make_sys_vandermonde(row, col, this->w);
    }

}
    
int E_MBR::Encode()const{
    string en_file = this->en_from_filepath + this->filename;
    string en_files = this->en_to_filepath + this->filename;
    unsigned char *in_buff, *out_buff, *i_b, *o_b;
    int size_in_buff = this->file_len;
    int size_out_buff = size_in_buff*(this->mat.rr)/(this->mat.cc);

    i_b = (unsigned char *)malloc(size_in_buff);
    in_buff = i_b;
    o_b = (unsigned char *)malloc(size_out_buff);
    out_buff = o_b;
    

    free(i_b);
    free(o_b);
    

}

int E_MBR::Decode()const{

}

int E_MBR::Repair()const{

}

int GetN()const{
    return this->n;
}

int GetK()const{
    return this->k;
}

string GetFilename()const{
    return filename;
}

int GetFilelen()const{
    return file_len;
}

GMatrix GetMat()const{
    return mat;
}

int SetN(const int& n_set){
    this->n = n_set;

    return this->n;
}

int SetK(const int& k_set){
    this->k = k_set;

    return this->k;
}

void SetMat(const GMatrix& mat_set){
    this->mat = mat_set;
}

void SetEnpath(const string& from_filepath, const string& to_filepath){
    this->en_from_filepath = from_filepath;
    this->en_to_filepath = to_filepath;
}

void SetDepath(const string& from_filepath, const string& to_filepath){
    this->de_from_filepath = from_filepath;
    this->de_to_filepath = to_filepath;
}

void SetFile(const string& fname, const int& flen){
    this->filename = fname;
    this->file_len = flen;
}

void SetFile(const string& fname){
    FILE *fp;
    struct stat file_info;
    char filename_f[128];

    this->filename = fname;
    if(fp = fopen(file_,"r")){
        fclose(fp);
        sprintf(filename_f , "%s%s" , (en_from_filepath).c_str(),(fname).c_str());
        stat(filename_f, &file_info);
        this->file_len =  file_info.st_size;
    }else{
        throw File_not_found();
    }
}



void E_MBR::meta_print()const{
    cout<<"n = "<<n<<endl;
    cout<<"k = "<<k<<endl;
    mat.Print();
    cout<<"filename:"<<filename<<endl;
    cout<<"file length:"<<file_len<<endl;
    cout<<"en_from_filepath : "<<en_from_filepath<<endl;
    cout<<"en_to_filepath : "<<en_to_filepath<<endl;
    cout<<"de_from_filepath : "<<de_from_filepath<<endl;
    cout<<"de_to_filepath : "<<de_to_filepath<<endl;
}
