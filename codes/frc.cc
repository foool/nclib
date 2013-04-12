#include "frc.h"

//constructor
FRC::FRC(int rows, int cols, int n, int k, int beta, int d, int w, int ws)
{
    if(false == TestParameters(rows, cols, n, k, beta, d, w, ws))
    {
        printf("invalid parameters\n");
        exit(-1);
    }
    this->rows = rows;
    this->cols = cols;
    this->n = n;
    this->k = k;
    this->beta = beta;
    this->d = d;
    this->w = w;
    this->w_s = ws;
    this->alpha = rows/n;
    this->bw = GetBW(alpha, k, beta, d);
    this->gamma = d * beta;
    this->i = alpha + k - d - 1;
    GetEncodeMatrix();

}

FRC::FRC(int rows, int n, int k, int beta, int d, int w, int ws)
{
    this->rows = rows;
    this->alpha = rows/n;
    this->bw = GetBW(alpha, k, beta, d);
    this->cols = bw;
    this->n = n;
    this->k = k;
    this->beta = beta;
    this->d = d;
    this->w = w;
    this->w_s = ws;
    this->gamma = d * beta;
    this->i = alpha + k - d - 1;
    if(false == TestParameters(rows, cols, n, k, beta, d, w, ws))
    {
        printf("invalid parameters\n");
        exit(-1);
    }
    GetEncodeMatrix();

}

FRC::FRC(int n, int k, int d, int i, int w, int ws)
{
    this->n = n;
    this->k = k;
    this->w = w;
    this->w_s = ws;
    this->rows = (d + 1 + i - k) * n;
    this->cols = k*(d+1+i-k)-(i+1)*i/2;
    this->alpha = rows/n;
    this->d = d;
    this->i = i;
    if((d < k) || (d >= n))
    {
        printf("invalid parameters\n");
        exit(-1);
    }
    if(0 == i)
    {
        this->gamma = cols*d/(k*(d-k+1));
    }
    else
    {
        if((i > 0) && (i <= (k - 1)))
        {
            this->gamma = (2*d*(cols-(k-i)*alpha))/((2*d-2*k+i+1)*i);
        }
        else
        {
            printf("invalid parameters!\n");
            exit(-1);
        }
    }
    GetEncodeMatrix();
    //printf("gamma:%d i:%d d:%d \n", gamma, i, d);
    //getchar();
}



FRC::FRC(string confFile){
    Config conf(confFile.c_str());
    conf.SetSection("filemetadata");
    conf.SetDelimiter("=");
    this->n = conf.Read("n" , 0);
    this->k = conf.Read("k" , 0);
    this->d = conf.Read("d", 0);
    this->i = conf.Read("i", 0);
    this->w_s = conf.Read("w_s" , 0);
    this->w = conf.Read("w" , 0);
    this->mat = conf.Read("mat" , mat);
    this->file_name = conf.Read("filename" , string(""));
    this->file_len = conf.Read("filesize" , 0);
    this->rows = (d + 1 + i - k) * n;
    this->cols = k*(d+1+i-k)-(i+1)*i/2;
    this->alpha = rows/n;
    if((d < k) || (d >= n))
    {
        printf("invalid parameters!\n");
        exit(-1);
    }
    if(0 == i)
    {
        this->gamma = cols*d/(k*(d-k+1));
    }
    else
    {
        if((i > 0) && (i <= (k - 1)))
        {
            this->gamma = (2*d*(cols-(k-i)*alpha))/((2*d-2*k+i+1)*i);
        }
        else
        {
            printf("invalid parameters!\n");
            exit(-1);
        }
    }
    //GetEncodeMatrix();

}
		
//methodes

int FRC::GetEncodeMatrix()
{
    int w64 = 64, w32 = 32, w16 = 16, w8 = 8, w4 = 4;
    GMatrixU8 matrix_u8;
    if(w == w64)
    {
        matrix_u8.Make_random(rows, cols, w_s);
        GMatrix64 matrix_64 = U8to64(matrix_u8);
        while(0 == NK_property(matrix_64, alpha, k))
        {
            NOTE("NK_Property not satisfied!");
            matrix_u8.Make_random(rows, cols, w_s);
            matrix_64 = U8to64(matrix_u8);
        }
    }
    else
    {
        if((w == w8) || (w == w16) || (w == w32))
        {
            matrix_u8.Make_random(rows, cols, w_s);
            GMatrix matrix = U8tomat(matrix_u8, w);
            while(0 == NK_property(matrix, alpha, k))
            {
                NOTE("NK_Property not satisfied!");
                matrix_u8.Make_random(rows, cols, w_s);
                matrix = U8tomat(matrix_u8, w);
            }
        }
        else
        {
            if((w == w4) && (w_s == w4))
            {
                matrix_u8.Make_random(rows, cols, w_s);
                while(0 == NK_property(matrix_u8, alpha, k))
                {
                    NOTE("NK_Property not satisfied!");
                    matrix_u8.Make_random(rows, cols, w_s);
                }
            }
        }
    }
    string encode_matrix = "";
    for(int i = 0; i < matrix_u8.rr; i++)
    {
        for(int j = 0; j < matrix_u8.cc; j++)
        {
            ostringstream oss;
            oss<<matrix_u8.Get(i, j);
            encode_matrix = encode_matrix + oss.str() +' ';
        }
    }
    mat = encode_matrix;
    return 1;
}

int FRC::MemCoding(GMatrix & gm, unsigned char *psrc, unsigned char *pdes, int size, int type)const{
    switch(type)
    {
        case 32:
            w32coding_(gm, pdes, psrc, size);
            break;
        case 16:
            w16coding_(gm, pdes, psrc, size);
            break;
        case 8:
            w8coding_(gm, pdes, psrc, size);
            break;
        default:
            printf("no such coding type!\n");
            return -1;
    }
    return 1;
    
}

int FRC::MemCoding(GMatrix64 & gm, unsigned char *psrc, unsigned char *pdes, int size, int type)const{

    switch(type)
    {
        case 64:
            w64coding_(gm, pdes, psrc, size);
            break;
        default:
            printf("no such coding type!\n");
            return -1;
    }
    return 1;

}

int FRC::MemCoding(GMatrixU8 & gm, unsigned char *psrc, unsigned char *pdes, int size, int type)const{
    switch(type)
    {
        case 4:
            w4coding_(gm, pdes, psrc, size);
            break;
        default:
            printf("no such coding type!\n");
            return -1;
    }
    return 1;

}


int FRC::Encode(string filepath, string filename){
    string file_path = filepath + filename;
    struct stat info;
    stat(file_path.c_str(), &info);
    unsigned long filesize = info.st_size;
    unsigned long size_in_buff, size_out_buff;
    int w64 = 64, w32 = 32, w16 = 16, w8 = 8, w4 = 4;
    unsigned char *in_buff, *out_buff;
    int r = 64 * cols;
    int result = 0;
    GMatrixU8 matrix_u8;
    this->file_len = filesize;
    if(0 == (filesize % r))
    {
        //printf("aaaaaaaaaaaaa\n");
        size_in_buff = filesize;
    }
    else
    {
        size_in_buff = (filesize/r + 1)*r;
    }
    size_out_buff = size_in_buff*rows/cols;
    in_buff = (unsigned char *)calloc(size_in_buff, sizeof(unsigned char));
    out_buff = (unsigned char *)calloc(size_out_buff, sizeof(unsigned char));
    //printf("%s\n",file_path.c_str());

    fsread_buff(file_path , in_buff , this->file_len);
    //mem_print_(in_buff, size_in_buff, 64);
    printf("k1\n");
    matrix_u8.Make_from_string(mat, rows, cols, w_s);
    printf("k2\n");
    if(w == w64)
    {
        GMatrix64 matrix_64 = U8to64(matrix_u8);
        //matrix_64.Print();
        result = MemCoding(matrix_64, in_buff, out_buff, size_in_buff, w);
    }
    else
    {
        if((w == w8) || (w == w16) || (w == w32))
        {
            GMatrix matrix = U8tomat(matrix_u8, w);
            //matrix.Print();
            result = MemCoding(matrix, in_buff, out_buff, size_in_buff, w);
        }
        if((w == w4) && (w_s == w4))
        {
            result = MemCoding(matrix_u8, in_buff, out_buff, size_in_buff, w);
        }
    }
    if(1 == result)
    {
        //mem_print_(in_buff, size_in_buff, 64);
        //printf("aaaaaaaaaaaaa\n");
        //mem_print_(out_buff, size_out_buff, 64);

        Bat_Write(filename , rows , out_buff , size_in_buff/cols , filepath);

        string metadata_path = filepath + filename + ".meta";
        Config newconf;
        newconf.SetSection("filemetadata");
        newconf.SetDelimiter("=");
        newconf.Add("filename", filename);
        newconf.Add("filesize", filesize);
        newconf.Add("n", this->n);
        newconf.Add("k", this->k);
        newconf.Add("d", this->d);
        newconf.Add("i", this->i);
        newconf.Add("w_s", this->w_s);
        newconf.Add("w", this->w);
        newconf.Add("mat", this->mat);
        newconf.Write(metadata_path);

    }
    else
    {
        NOTE("w type error!");
    }
    //matrix_u8.Print();
    //mem_print_(in_buff, size_in_buff, 64);
    //mem_print_(out_buff, size_out_buff, 64);
    free(in_buff);
    free(out_buff);

    
    
    return result;
}

        

int FRC::Decode(string path_decode, string nodes_idx){
    
    unsigned char *in_buff, *out_buff;
    unsigned long size_in_buff, size_out_buff;
    unsigned long filesize = this->file_len, size_each_file;
    int w64 = 64, w32 = 32, w16 = 16, w8 = 8, w4 = 4;
    vector <int> nodes_idx_list;
    int node_num;
    int r = 64 * cols;
    int result = 0;
    string encoded_filename = this->file_name;
    nodes_idx = _Trim(nodes_idx);
    stringstream ss(nodes_idx);
    while(!ss.eof())
    {
        ss >> node_num;
        nodes_idx_list.push_back(node_num);
    }
    SortIds(nodes_idx_list, 0, nodes_idx_list.size() - 1);
    vector <int>::size_type nodes_amount = nodes_idx_list.size();
    if(nodes_amount < (unsigned int)this->k)
    {
        printf("nodes amount is not enough!\n");
        return -1;
    }
    if(this->file_len > 0)
    {
        if(0 == filesize%r)
        {
            size_out_buff = filesize;
        }
        else
        {
            size_out_buff = (filesize/r + 1)*r;
        }
        size_in_buff = size_out_buff;
        size_each_file = size_in_buff/cols;
    }
    else
    {
        return -1;
    }
    printf("NOTEA\n");
    /*
    string files_idx_cols;
    int count = 0;
    for(int i = 0 ; i < k ; i++)
    {
        for(int j = 0; j < alpha; j++)
        {
            stringstream ss_idx;
            ss_idx << (nodes_idx_list.at(i) * alpha + j);
            files_idx_cols = files_idx_cols + ss_idx.str() + " ";
            count++;
            if(count == cols)
            {
                break;
            }
        }

    }
    files_idx_cols = _Trim(files_idx_cols);
    printf("%s\n", files_idx_cols.c_str());
    */
    in_buff = (unsigned char *)calloc(size_in_buff, sizeof(unsigned char));
    out_buff = (unsigned char *)calloc(size_out_buff, sizeof(unsigned char));
    //Bat_Read(primary_path + encoded_filename , files_idx_cols , in_buff , size_each_file);

    //string metadata_path = meta_path + encoded_filename + ".meta";
    //Config conf(metadata_path);
    //conf.SetSection("filemetadata");
    //string mat;
    //GMatrixU8 matrix_u8;
    //mat = conf.Read("mat", mat);
    //printf("aaaaaaaaaaaaaaaaaa\n");
    //matrix_u8.Make_from_string(mat, rows, cols, w_s);
    //matrix_u8.Print();
    //printf("bbbbbbbbbbbbbbbbbb\n");
    //GMatrixU8 mat_de;
    //mat_de = Draw_rows(matrix_u8 , files_idx_cols , cols);

    //mat_de.Print();

    if(w == w64)
    {
        GMatrix64 matrix_64;
        matrix_64.Make_from_string(this->mat, rows, cols, w);
        vector<int> files_idx_cols;
        GMatrix64 mat_de;
        GetFullRankRows(matrix_64, mat_de, nodes_idx_list, files_idx_cols, k, alpha);
        SortIds(files_idx_cols, 0, files_idx_cols.size() - 1);
        mat_de.Inverse();
        Bat_Read(path_decode + encoded_filename , files_idx_cols , in_buff , size_each_file);
        result = MemCoding(mat_de, in_buff, out_buff, size_in_buff, w);
        
    }
    else
    {
        if((w == w8) || (w == w16) || (w == w32))
        {
            GMatrix matrix;
            matrix.Make_from_string(this->mat, rows, cols, w);
            vector<int> files_idx_cols;
            GMatrix mat_de;
            GetFullRankRows(matrix, mat_de, nodes_idx_list, files_idx_cols, k, alpha);
            SortIds(files_idx_cols, 0, files_idx_cols.size() - 1);
            //mat_de.Print();
            mat_de.Inverse();
            Bat_Read(path_decode + encoded_filename , files_idx_cols , in_buff , size_each_file);

            result = MemCoding(mat_de, in_buff, out_buff, size_in_buff, w);
        }
        if((w == w4) && (w_s == w4))
        {
            GMatrixU8 matrix_u8;
            matrix_u8.Make_from_string(mat, rows, cols, w);
            vector<int> files_idx_cols;
            GMatrixU8 mat_de;
            GetFullRankRows(matrix_u8, mat_de, nodes_idx_list, files_idx_cols, k, alpha);
            SortIds(files_idx_cols, 0, files_idx_cols.size() - 1);
            mat_de.Inverse();
            Bat_Read(path_decode + encoded_filename , files_idx_cols , in_buff , size_each_file);
            result = MemCoding(mat_de, in_buff, out_buff, size_in_buff, w);
        }
    }
    if(1 == result)
    {
        fswrite_buff(path_decode + encoded_filename, out_buff, this->file_len);
    }
    else
    {
        NOTE("w type error!");
    }
    //mem_print_(in_buff, size_in_buff, 64);
    //mem_print_(out_buff, size_out_buff, 64);
    free(in_buff);
    free(out_buff);
    

    return result;
}

//meta repair

int FRC::Repair(string filepath, int vnode){

    int w64 = 64, w32 = 32, w16 = 16, w8 = 8, w4 = 4;
    //int randnum;
    GMatrix64 matrix_64;
    string filename = this->file_name;
    string metadata_path = filepath + filename + ".meta";
    Config conf(metadata_path);
    conf.SetSection("filemetadata");
    vector<int> files_idx_list;
    //vector<int> nodes_idx_list;
    //vector<int> nodes_idx_add;
    const int vpnode = gamma/d;
    const int rnode = gamma%d;
    //mat_rand_u8.Make_random(alpha, gamma, w_s);
    /*
    mat_rep = Prod(mat_rand, mat_n);
    matrix_u8.Replace_matrix(mat_rep, vnode*alpha);
    printf("mat_rand\n");
    mat_rand.Print();
    printf("mat_n\n");
    mat_n.Print();
    printf("mat_rep\n");
    mat_rep.Print();
    printf("matrix_u8\n");
    matrix_u8.Print();
    */
    if(w == w64)
    {
        GMatrix64 matrix_64;
        matrix_64.Make_from_string(this->mat, rows, cols, w);
        GMatrixU8 mat_rand_u8;
        for(int num = 0; num < 1000; num++)
        {
            mat_rand_u8.Make_random(alpha, gamma, w_s);
            GMatrix64 mat_rand;
            mat_rand = U8to64(mat_rand_u8);
            GetRandomFileidx(files_idx_list, n, d, alpha, vnode, vpnode, rnode);
            GMatrix64 mat_n;
            mat_n = Draw_rows(matrix_64, files_idx_list, files_idx_list.size());
            GMatrix64 mat_rep;
            mat_rep = Prod(mat_rand, mat_n);
            matrix_64.Replace_matrix(mat_rep, vnode*alpha);
            if(1 == NK_property(matrix_64, alpha, k))
            {
                printf("%d times to repair gamma: %d\n", num, gamma);
                UpdateMeta(matrix_64, mat_rand, files_idx_list, conf);
                this->mat = conf.Read("mat", mat);
                conf.Write(metadata_path);
                return 1;
            }
        }
        
    }
    else
    {
        if((w == w8) || (w == w16) || (w == w32))
        {
            GMatrix matrix;
            matrix.Make_from_string(this->mat, rows, cols, w);
            GMatrixU8 mat_rand_u8;
            for(int num = 0; num < 1000; num++)
            {
                mat_rand_u8.Make_random(alpha, gamma, w_s);
                GMatrix mat_rand;
                mat_rand = U8tomat(mat_rand_u8, w);
                GetRandomFileidx(files_idx_list, n, d, alpha, vnode, vpnode, rnode);
                GMatrix mat_n;
                mat_n = Draw_rows(matrix, files_idx_list, files_idx_list.size());
                GMatrix mat_rep;
                mat_rep = Prod(mat_rand, mat_n);
                matrix.Replace_matrix(mat_rep, vnode*alpha);
                if(1 == NK_property(matrix, alpha, k))
                {
                    printf("NOTE2\n");
                    printf("%d times to repair gamma: %d\n", num, gamma);
                    UpdateMeta(matrix, mat_rand, files_idx_list, conf);
                    //update the encode matrix in the object
                    this->mat = conf.Read("mat", mat);
                    conf.Write(metadata_path);
                    return 1;
                }
            }
        }
        else
        {
            if((w == w4) && (w_s == w4))
            {
                GMatrixU8 matrix_u8;
                matrix_u8.Make_from_string(this->mat, rows, cols, w);
                for(int num = 0; num < 1000; num ++)
                {
                    GMatrixU8 mat_n, mat_rep;
                    GMatrixU8 mat_rand;
                    mat_rand.Make_random(alpha, gamma, w_s);
                    GetRandomFileidx(files_idx_list, n, d, alpha, vnode, vpnode, rnode);
                    mat_n = Draw_rows(matrix_u8, files_idx_list, files_idx_list.size());
                    mat_rep = Prod(mat_rand, mat_n);
                    matrix_u8.Replace_matrix(mat_rep, vnode*alpha);
                    if(1 == NK_property(matrix_u8, alpha, k))
                    {
                        printf("%d times to repair gamma: %d\n", num, gamma);
                        UpdateMeta(matrix_u8, mat_rand, files_idx_list, conf);
                        //update the encode matrix in the object
                        this->mat = conf.Read("mat", mat);
                        conf.Write(metadata_path);
                        return 1;
                    }
                }
            }
            else
            {
                NOTE("w type error!");
                return 0;
            }
        }
    }

        /*
        gamma++;
        printf("nedd ++\n");
        getchar();
        if(gamma > (n-1)*alpha)
        {
            break;
        }
        */
    printf("fail to repair!\n");
    return 0;
}

int FRC::RepairFile(string filepath, int vnode)const
{
    int r = 64 * cols;
    int w64 = 64, w32 = 32, w16 = 16, w8 = 8, w4 = 4;
    unsigned char *in_buff, *out_buff;
    unsigned long filesize = this->file_len, size_each_file;
    unsigned long size_in_buff, size_out_buff;
    vector<int> files_idx_list;
    string filename = this->file_name;
    string metadata_path = filepath + filename + ".meta";
    Config conf(metadata_path);
    conf.SetSection("filemetadata");
    string mat_repair;
    mat_repair = conf.Read("mat_repair", mat_repair);
    //int gamma;
    //gamma = conf.Read("gamma", gamma);
    GMatrixU8 matrix_repair;
    matrix_repair.Make_from_string(mat_repair, alpha, gamma, w_s);
    string files_idx;
    files_idx = conf.Read("files_idx_repair", files_idx);
    
    //printf("aa\n");
    //matrix_repair.Print();
    //printf("bb\n");
    if(filesize > 0)
    {
        if(0 == filesize%r)
        {
            size_each_file = filesize/cols;
        }
        else
        {
            size_each_file = ((filesize/r + 1)*r)/cols;
        }
    }
    else
    {
        return -1;
    }
    size_in_buff = gamma * size_each_file;
    size_out_buff = alpha * size_each_file;

    stringstream ss(files_idx);
    int file_num;
    while(!ss.eof())
    {
        ss >> file_num;
        files_idx_list.push_back(file_num);
    }
    vector <int>::size_type files_amount = files_idx_list.size();
    if(files_amount < (unsigned int)gamma)
    {
        printf("files amount is not enough!\n");
        return -1;
    }
    in_buff = (unsigned char *)calloc(size_in_buff, sizeof(unsigned char));
    Bat_Read(filepath + filename , files_idx_list , in_buff , size_each_file);
    out_buff = (unsigned char *)calloc(size_out_buff, sizeof(unsigned char));

    int result = 0;
    if(w == w64)
    {
        GMatrix64 matrix_64 = U8to64(matrix_repair);
        result = MemCoding(matrix_64, in_buff, out_buff, size_in_buff, w);
    }
    else
    {
        if((w == w8) || (w == w16) || (w == w32))
        {
            GMatrix matrix = U8tomat(matrix_repair, w);
            result = MemCoding(matrix, in_buff, out_buff, size_in_buff, w);
        }
        if((w == w4) && (w_s == w4))
        {
            result = MemCoding(matrix_repair, in_buff, out_buff, size_in_buff, w);
        }
    }
    if(1 == result)
    {
        vector<int> files_idx_repair;
        for(int i = 0; i < alpha; i++)
        {
            files_idx_repair.push_back(vnode * alpha + i);
        }
        Bat_Write(filename, files_idx_repair, out_buff, size_each_file, filepath);
    }
    else
    {
        NOTE("w type error!");
    }

    //mem_print_(in_buff, size_in_buff, 64);
    //mem_print_(out_buff, size_out_buff, 64);

    free(in_buff);
    free(out_buff);
    return result;
}

//Get class member values
int FRC::GetN()const{

    return this->n;
}

int FRC::GetK()const{

    return this->k;
}

int FRC::GetW()const{

    return this->w;
}

int FRC::GetAlpha()const{

    return this->alpha;
}

int FRC::GetFilelen()const{
    
    return this->file_len;
}

//Set class member values
void FRC::SetN(const int& n_set){
    
    this->n = n_set;
}

void FRC::SetK(const int& k_set){

    this->k = k_set;
}

void FRC::SetW(const int& w_set){

    this->w = w_set;
}

void FRC::SetAlpha(const int& alpha_set){
    this->alpha = alpha_set;
}


void FRC::SRCmeta_print()const{
    printf("hello");
}

/*
int UpdateMeta(const GMatrixU8 & matrix_u8, const GMatrixU8 & matrix_rand, const vector<int> & files_idx_list, Config & conf)
{
    string mat;
    for(int i = 0; i < matrix_u8.rr; i++)
    {
        for(int j = 0; j < matrix_u8.cc; j++)
        {
            ostringstream oss;
            oss<<matrix_u8.Get(i, j);
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

int UpdateMeta(const GMatrix & matrix, const GMatrix & matrix_rand, const vector<int> & files_idx_list, Config & conf)
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

int GetFullRankRows(const GMatrix & matrix, GMatrix & mat_de, const vector<int> & nodes_idx_list, vector<int> & files_idx_list, int k, int alpha)
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
            printf("the size:%d at i:%d\n", vpn_idx.size(), i);
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


int GetFullRankRows(const GMatrixU8 & matrix, GMatrixU8 & mat_de, const vector<int> & nodes_idx_list, vector<int> & files_idx_list, int k, int alpha)
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
            printf("the size:%d at i:%d\n", vpn_idx.size(), i);
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

int GetFullRankRows(const GMatrix64 & matrix, GMatrix64 & mat_de, const vector<int> & nodes_idx_list, vector<int> & files_idx_list, int k, int alpha)
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
            printf("the size:%d at i:%d\n", vpn_idx.size(), i);
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

*/

int GetRandomFileidx(vector<int> & files_idx_list, int n, int d, int alpha, int vnode, int vpnode, int rnode)
{
    vector<int> nodes_idx_list;
    vector<int> nodes_idx_add;
    files_idx_list.clear();
    int i = 0, j = 0;
    int randnum;
    while(i < d)
    {
        randnum = NC_random(n);
        vector<int>::iterator findit = find(nodes_idx_list.begin(), nodes_idx_list.end(), randnum);
        if((randnum == vnode) || (findit != nodes_idx_list.end()))
        {
            continue;
        }
        else
        {
            nodes_idx_list.push_back(randnum);
            i++;
        }
    }
    for(i = 0; i < d; i++)
    {
        for(j = 0; j < vpnode; j++)
        {
            do
            {
                randnum = nodes_idx_list[i]*alpha + NC_random(alpha);
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
        }
    }
    i = 0;
    while(i < rnode)
    {
        randnum = NC_random(d);
        vector<int>::iterator findit = find(nodes_idx_add.begin(), nodes_idx_add.end(), nodes_idx_list[randnum]);
        if(findit == nodes_idx_add.end())
        {
            nodes_idx_add.push_back(nodes_idx_list[randnum]);
            i++;
        }
    }
    for(vector<int>::iterator iter = nodes_idx_add.begin(); iter != nodes_idx_add.end(); iter++)
    {
        randnum = (*iter)*alpha + NC_random(alpha);
        vector<int>::iterator findit = find(files_idx_list.begin(), files_idx_list.end(), randnum);
        while(findit != files_idx_list.end())
        {
            randnum = (*iter)*alpha + NC_random(alpha);
            findit = find(files_idx_list.begin(), files_idx_list.end(), randnum);
        }
        files_idx_list.push_back(randnum);
    }
    SortIds(files_idx_list, 0, files_idx_list.size() - 1);
    return 1;
}

int SortIds(vector<int> & idx_list, int begin, int end)
{
    if(begin >= end)
    {
        return 1;
    }
    int index1 = begin;
    int index2 = end;
    int key = idx_list[index1];
    //printf("index1:%d index2:%d key:%d\n", index1, index2, key);
    //getchar();
    while(index1 < index2)
    {
        while((index2 > index1) && (idx_list[index2] > key))
        {
            index2--;
        }
        idx_list[index1] = idx_list[index2];
        while((index1 < index2) && (idx_list[index1] < key))
        {
            index1++;
        }
        idx_list[index2] = idx_list[index1];
    }
    idx_list[index1] = key;
    SortIds(idx_list, begin, index1 - 1);
    SortIds(idx_list, index1 + 1, end);
    return 1;
}

int Gcd ( int a, int b )
{
    int c;
    while(a != 0)
    {
        c = a;
        a = b%a;
        b = c;
    }
    return b;
}

int GetBW(int alpha, int k, int beta, int d)
{
    int i;
    int ret = 0;
    for(i = 0; i <= k-1; ++i)
    {
        ret += MIN(alpha, (d - i)*beta);
    }
    return ret;
}

bool TestParameters(int rows, int cols, int n, int k, int beta, int d, int w, int ws)
{
    bool result = true;
    if(((w <= 0) || (w >= 8)) || ((ws != 4) && (ws != 8) && (ws != 16) && (ws != 32) && (ws != 64)))
    {
        result = false;
        return result;
    }
    if((rows%n != 0) || (d*beta >= cols))
    {
        result = false;
        return result;
    }
    int alpha = rows / n;
    if((alpha*(k-1) >= cols) || (Gcd(Gcd(Gcd(Gcd(rows, cols),alpha),beta),d)!=1) || (d < k) || (d > n))
    {
        result = false;
        return result;
    }
    int bw = GetBW(alpha, k, beta, d);
    if(bw != cols)
    {
        result = false;
        return result;
    }
    if(beta*(n-1) == alpha)
    {
        printf("MBR\n");
    }
    else
    {
        if(alpha*k == cols)
        {
            printf("MSR\n");
        }
        else
        {
            printf("intermediate node\n");
        }
    }
    return result;

}


