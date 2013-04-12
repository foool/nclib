#include "nc.h"
#include "galois.h"

GMatrixU8::GMatrixU8(){
    this->rr = 0;
    this->cc = 0;
    this->ww = 0;
}

GMatrixU8::GMatrixU8(int row, int col, int word){
    assert((word<8)&&(word>0));
    this->rr = row;
    this->cc = col;
    this->ww = word;
}

int GMatrixU8::Resize_matrix(){
    try{
        ele.resize(this->rr * this->cc, 0);
    }catch(...){
        return -1;
    }
    return 1;
}

int GMatrixU8::Get(int row, int col)const{
    return this->ele[row*(this->cc) + col];
}

void GMatrixU8::Set(int row, int col, int val){
    assert(val < (1<<this->ww));
    this->ele[row*(this->cc) + col] = val;
}

int GMatrixU8::Make_zero(int row, int col, int word){

    this->ele.clear();
    this->rr = row;
    this->cc = col;
    this->ww = word;
    Resize_matrix();

    return row;
}

int GMatrixU8::Make_identity(int row, int col, int word){
    int i;

    assert(row == col);

    this->rr = row;
    this->cc = col;
    this->ww = word;
    Make_zero(row, col, word);

    for(i = 0; i < row; i++){
        Set(i, i, 1);
    }

    return row;
}

int GMatrixU8::Make_from_string(string str, int rows, int cols, int w){
    int i, j;
    uint32_t ele;
    
    istringstream eles(str);
    rr = rows;
    cc = cols;
    ww = w;
    
    Resize_matrix();
    for(i = 0; i < rows; i++){   
        for(j = 0; j < cols; j++){   
            eles>>ele;
            Set(i, j, (uint8_t)ele);
        }   
    }   
    return rows*cols;                                                     
}

int GMatrixU8::Make_vandermonde(int r, int c, int word){
    int i, j;
    unsigned int val;

    this->rr = r;
    this->cc = c;
    this->ww = word;
    Make_zero(r, c, word);

    for(j = 0; j < c; j++){
        Set(0, j, 1);
    }
    for(i = 1; i < r; i++){
        val = 1;
        for(j = 0; j < c; j++){
            Set(i, j, val);
            val = galois_single_multiply(val, i+1, word);
        }
    }

    return r;
}

/* Make a vandermonde matrix with systematic part */
int GMatrixU8::Make_sys_vandermonde(int r, int c, int word){
    int i, j;
    unsigned int val;

    assert(r > c);
    Make_identity(c, c, word);
    this->rr = r;
    this->cc = c;
    this->ww = word;
    Resize_matrix();

    for(j = 0; j < c; j++){
        Set(c, j, 1);
    }
    
    for(i = c+1; i < r; i++){
        val = 1;
        for(j = 0; j < c; j++){
            Set(i, j, val);
            val = galois_single_multiply(val, i+1, word);
        }
    }

    return r;
}

int GMatrixU8::Make_random(int row, int col, int word){
    int i, j;
    int val;
    int mod = 1<<word;

    this->rr = row;
    this->cc = col;
    this->ww = word;
    Resize_matrix();

    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            val = rand()%mod;
            Set(i, j, val);
        }
    }

    return row;
}

void GMatrixU8::Print()const{
    for(int i=0; i<(this->rr); i++){
        for(int j=0; j<(this->cc); j++){
            printf("%03d ", Get(i, j));
        }
        printf("\n");
    }
}

void GMatrixU8::Swap_rows(int row1, int row2){
    int tmp;

    for(int i = 0; i < this->cc; i++){
        tmp = Get(row1, i);
        Set(row1, i, Get(row2, i));
        Set(row2, i, tmp);
    }
}

void GMatrixU8::Del_row(int row){
    int i,j;
    
    assert(row < this->rr);
    for(i = row; i < (this->rr-1); ++i){
        for(j = 0; j < this->cc; ++j){
            ele[i*this->cc + j] = ele[(i+1)*this->cc +j];
        }
    }
    --this->rr;
    Resize_matrix();
}

void GMatrixU8::Del_rows(int begin, int len){
    int i;

    assert(begin+len <= this->rr);

    for(i = 0 ; i < len ; i++){
        Del_row(begin);
    }
}

void GMatrixU8::Row_to_irow(int row, int prod){
    int w = this->ww;

    for(int i = 0; i < this->cc; i++){
        Set(row, i, galois_single_multiply(Get(row, i), prod, w));
    }
}

void GMatrixU8::Row_plus_irow(int row1, int row2, int prod){
    int w = this->ww;
    
    for(int i = 0; i < this->cc; i++){
        Set(row1, i, Get(row1, i)^(galois_single_multiply(Get(row2, i), prod, w)));
    }
}

void GMatrixU8::Replace_matrix(GMatrixU8 mat, int begin){
    int i, j;
    assert(cc == mat.cc);
    assert((begin + mat.rr) <= rr);

    for(i = 0; i < mat.rr; i++){   
        for(j = 0; j < mat.cc; j++){   
            Set(begin + i, j, mat.Get(i, j));
        }   
    }   
}

void GMatrixU8::Append_matrix(GMatrixU8 mat_app, int begin, int len){
    int i, j;
    int end;

    assert(mat_app.cc == cc);
    assert(begin >= 0);
    assert(mat_app.rr >= (begin+len));
    
    end = rr;
    rr = rr + len;
    Resize_matrix();

    for(i = 0; i < len; i++){
        for(j = 0; j < cc; j++){
            Set(end + i, j, mat_app.Get(begin+i,j));
        }
    }
}

GMatrixU8 Draw_rows(const GMatrixU8& mat, const string& row_list, const int& len)
{
    GMatrixU8 ret = mat;
    int i, j;
    int * lst;
    int ic = 0;
    assert(mat.rr > len);
    lst = (int *)malloc(100*sizeof(int));
    stringstream ss(row_list);
    while(!ss.eof())
    {
        ss >> lst[ic];
        ++ic;
    }
    for(i = mat.rr-1; i >= 0; --i)
    {
        bool isin = false;
        for(j = len-1; j >= 0; --j)
        {
            if(lst[j] == i)
            {
                isin = true;
            }
        }
        if(isin)
        {
            continue;
        }
        else
        {
            ret.Del_row(i);
        }
    }
    free(lst);
    return ret;
}

GMatrixU8 Draw_rows(const GMatrixU8& mat, const vector<int>& row_list, const int& len){
    GMatrixU8 ret = mat;

    int i, j;
    assert(mat.rr > len);
    for(i = mat.rr-1; i >= 0; --i)
    {
        bool isin = false;
        for(j = len-1; j >= 0; --j)
        {
            if(row_list[j] == i)
            {
                isin = true;
            }
        }
        if(isin)
        {
            continue;
        }
        else
        {
            ret.Del_row(i);
        }
    }
    return ret;
}

void GMatrixU8::Inverse(){
    GMatrixU8 mat_inv;
    int w = this->ww;
    int dim;
    int i, nzero_row, j;
    int temp, jtimes;

    assert(this->rr == this->cc);
    dim = this->rr;
    mat_inv.Make_identity(this->rr, this->cc, this->ww);

    for(i = 0; i < dim; i++){
        nzero_row = i;
        if(0 == Get(i,i)){
            do{
                ++nzero_row;
                if(nzero_row >= dim){
                    ERROR("Non-full rank matrix!");
                }
                temp = Get(nzero_row, i);
            }while((0 == temp)&&(nzero_row < dim));
            Swap_rows(i, nzero_row);
            mat_inv.Swap_rows(i, nzero_row);
        }
    
        for(j = 0; j < dim; j++){
            if(0 == Get(j,i))
                continue;
            if(j != i){
                jtimes = galois_single_divide(Get(j,i), Get(i,i), w);
                Row_plus_irow(j, i, jtimes);
                mat_inv.Row_plus_irow(j, i, jtimes);
            }else{
                jtimes = galois_inverse(Get(i, i), w);
                Row_to_irow(i , jtimes);
                mat_inv.Row_to_irow(i , jtimes);
            }
        }
    }
    this->ele = mat_inv.ele;
}

void GMatrixU8::Clear_zero_rows(){
    int ncur = 0;
    bool az = true;
    int i;

    for(ncur = 0; ncur < this->rr; ++ncur){
        for(i = 0; i < this->cc; ++i){
            if(Get(ncur, i) != 0){
                az = false;
                break;
            }
        }
        if(az){
            this->Del_row(ncur);
            --ncur;
        }
        az = true;
    }

    return;
}

int Rank(const GMatrixU8& mat){
    GMatrixU8 mat_cpy;
    int nz_row, nz_col;
    int i, j;
    int temp, jtimes;

    mat_cpy = mat;
    
    nz_col= 0;
    for(i = 0; i < mat_cpy.rr; i++){
        nz_row = i;
        nz_col = (nz_row > nz_col)?nz_row:nz_col;
        if(i == (mat_cpy.cc-1)){
            if(mat_cpy.Get(nz_row, nz_col) != 0){
                return mat_cpy.cc;
            }
            else{
                return (mat_cpy.cc -1);
            }
        }
        
        while(0 == mat_cpy.Get(i, nz_col)){
            nz_row = i;
            do{
                ++nz_row;
                if(nz_row >= mat_cpy.rr){
                    break;
                }
                temp = mat_cpy.Get(nz_row, nz_col);
            }while((0 == temp)&&(nz_row < mat_cpy.rr));

            if(nz_row < mat_cpy.rr){
                mat_cpy.Swap_rows(i, nz_row);
                break;
            }
            ++nz_col;
            if(nz_col >= mat_cpy.cc){
                return i;
            }
        }
        for(j = i+1; j < mat_cpy.rr; j++){
            if(mat_cpy.Get(j, nz_col) == 0)
                continue;
            jtimes = galois_single_divide(mat_cpy.Get(j,nz_col), mat_cpy.Get(i,nz_col), mat_cpy.ww);
            mat_cpy.Row_plus_irow(j, i, jtimes);
        }
        mat_cpy.Clear_zero_rows();
    }

    return mat.rr;
}

GMatrixU8 Prod(GMatrixU8 m1, GMatrixU8 m2){
    GMatrixU8 ret;
    int i, j, k;
    int r, c;
    int val;
    int w = m1.ww;

    assert(m1.ww == m2.ww);
    assert(m1.cc == m2.rr);
    r = m1.rr;
    c = m2.cc;
    ret.Make_zero(r, c, w);
    for(i = 0; i < r; i++){
        for(j = 0; j < c; j++){
            val = 0;
            for(k = 0; k < m1.cc; k++){
                val = val^galois_single_multiply(m1.Get(i, k), m2.Get(k, j), w);
                ret.Set(i, j, val);
            }
        }
    }

    return ret;
}


bool Is_full(GMatrixU8 mat){
    GMatrixU8 mat_cpy;
    int w = mat.ww;
    int nz_row, nz_col;     /* equal nzero_row in Inverse() */
    int i, j;
    int temp, jtimes;

    if(mat.rr > mat.cc){
        return false;
    }
    if(mat.rr <= 0){
        return false;
    }
    if(mat.cc <= 0){
        return false;
    }

    mat_cpy = mat;
    
    nz_col= 0;
    for(i = 0; i < mat_cpy.rr; i++){
        nz_row = i;
        nz_col = (nz_row > nz_col)?nz_row:nz_col;
        if(i == (mat_cpy.cc-1)){
            if(mat_cpy.Get(nz_row, nz_col) != 0){
                return true;
            }
            else{
                return false;
            }
        }
        
        while(0 == mat_cpy.Get(i, nz_col)){
            nz_row = i;
            do{
                ++nz_row;
                if(nz_row >= mat_cpy.rr){
                    break;
                }
                temp = mat_cpy.Get(nz_row, nz_col);
            }while((0 == temp)&&(nz_row < mat_cpy.rr));

            if(nz_row < mat_cpy.rr){
                mat_cpy.Swap_rows(i, nz_row);
                break;
            }
            ++nz_col;
            if(nz_col >= mat_cpy.cc){
                return false;
            }
        }

        for(j = i+1; j < mat_cpy.rr; j++){
            if(0 == mat_cpy.Get(j, nz_col))
                continue;
            jtimes = galois_single_divide(mat_cpy.Get(j,nz_col), mat_cpy.Get(i,nz_col), w);
            mat_cpy.Row_plus_irow(j, i, jtimes);
        }
    }
    return true;
}

/* Draw a matrix with len rows from begin-th row of the matrix  */
GMatrixU8 Slice_matrix(const GMatrixU8& mat, const int& begin, const int& len){
    GMatrixU8 ret;
    int i;
    int col;
    int tmp;

    assert(begin >= 0);
    assert(len > 0);
    //printf("%d + %d <= %d\n",begin, len, mat.rr);
    assert(begin + len <= mat.rr);
    
    ret.rr = len;
    ret.cc = mat.cc;
    ret.ww = mat.ww;
    col = mat.cc;
    
    ret.ele.resize((ret.rr)*(ret.cc), 0);
    tmp = col*len;
    for(i = 0; i < tmp; i++){
        ret.ele[i] = mat.ele[i+tmp];
    }
    
    return ret;
}

GMatrix U8tomat(GMatrixU8 mat, int w){
    int r = mat.rr;
    int c = mat.cc;
    GMatrix ret(r, c, w);
    int i, j;

    for(i = 0; i < r; i++){
        for(j = 0; j < c; j++){
            ret.Set(i, j, mat.Get(i, j));
        }
    }
    
    return ret;
}


GMatrix64 U8to64(GMatrixU8 mat){
    int r = mat.rr;
    int c = mat.cc;
    GMatrix64 ret(r, c);
    int i, j;

    for(i = 0; i < r; i++){
        for(j = 0; j < c; j++){
            ret.Set(i, j, mat.Get(i, j));
        }
    }
    return ret;
}
