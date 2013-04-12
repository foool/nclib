#include "nc.h"
#include "galois.h"

GMatrix64::GMatrix64(){
    this->rr = 0;
    this->cc = 0;
}

GMatrix64::GMatrix64(int row, int col){
    this->rr = row;
    this->cc = col;
    Resize_matrix();
}

int GMatrix64::Resize_matrix(){
    try{
        ele.resize(this->rr * this->cc, 0);
    }catch(...){
        return -1;
    }
    return 1;
}

uint64_t GMatrix64::Get(int row, int col)const{
    return this->ele[row*(this->cc) + col];
}

void GMatrix64::Set(int row, int col, uint64_t val){
    this->ele[row*(this->cc) + col] = val;
}

int GMatrix64::Make_zero(int row, int col){

    this->ele.clear();
    this->rr = row;
    this->cc = col;
    Resize_matrix();

    return row;
}

int GMatrix64::Make_identity(int row, int col){
    int i;

    assert(row == col);

    this->rr = row;
    this->cc = col;
    Make_zero(row, col);

    for(i = 0; i < row; i++){
        Set(i, i, 1);
    }

    return row;
}

int GMatrix64::Make_random(int row, int col){
    int i, j;
    uint64_t val;

    this->rr = row;
    this->cc = col;
    Make_zero(row, col);

    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            val = rand();
            val = (val << 32)|rand();
            Set(i, j, val);
        }
    }

    return row;
}

int GMatrix64::Make_from_string(string str, int rows, int cols, int w){
    int i, j;
    uint64_t ele;
    istringstream eles(str, istringstream::in);

    rr = rows;
    cc = cols;
    ww = w;
    Resize_matrix();

    for(i = 0; i < rows; i++){   
        for(j = 0; j < cols; j++){   
            eles>>ele;
            Set(i, j, ele);
        }   
    }   
    return rows*cols;
}

/* Make a vandermonde matrix */
int GMatrix64::Make_vandermonde(int r, int c){
    int i, j;
    uint64_t val;

    this->rr = r;
    this->cc = c;
    Make_zero(r, c);

    for(j = 0; j < c; j++){
        Set(0, j, 1);
    }
    for(i = 1; i < r; i++){
        val = 1;
        for(j = 0; j < c; j++){
            Set(i, j, val);
            val = galois_shift_multiply_64(val, i+1);
        }
    }

    return r;
}

/* Make a vandermonde matrix with systematic part */
int GMatrix64::Make_sys_vandermonde(int r, int c){
    int i, j;
    uint64_t val;
    int ins;

    assert(r > c);
    Make_identity(c, c);
    this->rr = r;
    this->cc = c;
    Resize_matrix();

    for(j = 0; j < c; j++){
        Set(c, j, 1);
    }
    ins = 1;
    for(i = c+1; i < r; i++){
        val = 1;
        ++ins;
        for(j = 0; j < c; j++){
            Set(i, j, val);
            val = galois_shift_multiply_64(val, ins);
        }
    }

    return r;
}

void GMatrix64::Print()const{
    for(int i=0; i<(this->rr); i++){
        for(int j=0; j<(this->cc); j++){
            printf("%.16" PRIx64 " ", Get(i, j));
        }
        printf("\n");
    }
}

void GMatrix64::Swap_rows(int row1, int row2){
    uint64_t tmp;

    for(int i = 0; i < this->cc; i++){
        tmp = Get(row1, i);
        Set(row1, i, Get(row2, i));
        Set(row2, i, tmp);
    }
}

void GMatrix64::Del_row(int row){
    int i,j;
    
    assert(row < rr);
    for(i = row; i < (this->rr-1); ++i){
        for(j = 0; j < this->cc; ++j){
            ele[i*this->cc + j] = ele[(i+1)*this->cc +j];
        }
    }
    --this->rr;
    Resize_matrix();
}

void GMatrix64::Del_rows(int begin, int len){
    int i;
    
    assert(begin+len <= this->rr);

    for(i = 0 ; i < len ; i++){
        Del_row(begin);
    }
}

void GMatrix64::Row_to_irow(int row, uint64_t prod){
    for(int i = 0; i < this->cc; i++){
        Set(row, i, galois_shift_multiply_64(Get(row, i), prod));
    }
}

void GMatrix64::Row_plus_irow(int row1, int row2, uint64_t prod){
    for(int i = 0; i < this->cc; i++){
        Set(row1, i, Get(row1, i)^(galois_shift_multiply_64(Get(row2, i), prod)));
    }
}

void GMatrix64::Replace_matrix(GMatrix64 mat, int begin){
    int i, j;
    assert(cc == mat.cc);
    assert((begin + mat.rr) <= rr);

    for(i = 0; i < mat.rr; i++){
        for(j = 0; j < mat.cc; j++){
            Set(begin + i, j, mat.Get(i, j));
        }
    }
}

void GMatrix64::Append_matrix(GMatrix64 mat_app){
    int i, j;
    int end;

    assert(mat_app.cc == cc);
    end = rr;
    rr = rr + mat_app.rr;
    Resize_matrix();
    
    for(i = 0; i < mat_app.rr; i++){
        for(j = 0; j < cc; j++){
            Set(end + i, j, mat_app.Get(i, j));
        }
    }
}

void GMatrix64::Append_matrix(GMatrix64 mat_app, int begin, int len){
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

GMatrix64 Draw_rows(const GMatrix64& mat, const string& row_list, const int& len)
{
    GMatrix64 ret = mat;
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

GMatrix64 Draw_rows(const GMatrix64& mat, const vector<int>& row_list, const int& len)
{
    GMatrix64 ret = mat;
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

void GMatrix64::Clear_zero_rows(){
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

void GMatrix64::Inverse(){
    GMatrix64 mat_inv;
    int dim;
    int i, nzero_row, j;
    uint64_t temp, jtimes;

    assert(this->rr == this->cc);
    dim = this->rr;
    mat_inv.Make_identity(this->rr, this->cc);

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
                jtimes = galois_single_divide_64(Get(j,i), Get(i,i));
                Row_plus_irow(j, i, jtimes);
                mat_inv.Row_plus_irow(j, i, jtimes);
            }else{
                jtimes = galois_shift_inverse_64(Get(i, i));
                Row_to_irow(i , jtimes);
                mat_inv.Row_to_irow(i , jtimes);
            }
        }
    }
    this->ele = mat_inv.ele;
}

GMatrix64 Slice_matrix(const GMatrix64& mat, const int& begin, const int& len){
    GMatrix64 ret;
    int i;
    int col;
    int tmp;

    assert(begin >= 0);
    assert(len > 0);
    assert(begin + len <= mat.rr);
    
    ret.rr = len;
    ret.cc = mat.cc;
    col = mat.cc;
    tmp = col*len;
    ret.ele.resize(tmp, 0);
    
    for(i = 0; i < tmp; i++){
        ret.ele[i] = mat.ele[i+tmp];
    }
    
    return ret;
}

GMatrix64 Prod(GMatrix64 m1, GMatrix64 m2){
    GMatrix64 ret;
    int i, j, k;
    int r, c;
    uint64_t val;

    r = m1.rr;
    c = m2.cc;
    ret.Make_zero(r, c);
    for(i = 0; i < r; i++){
        for(j = 0; j < c; j++){
            val = 0;
            for(k = 0; k < m1.cc; k++){
                val = val^galois_shift_multiply_64(m1.Get(i, k), m2.Get(k, j));
                ret.Set(i, j, val);
            }
        }
    }

    return ret;
}

int Rank(GMatrix64 mat){
    GMatrix64 mat_cpy;
    int nz_row, nz_col;
    int i, j;
    uint64_t temp, jtimes;

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
            jtimes = galois_single_divide_64(mat_cpy.Get(j,nz_col), mat_cpy.Get(i,nz_col));
            mat_cpy.Row_plus_irow(j, i, jtimes);
        }
        mat_cpy.Clear_zero_rows();
    }

    return mat.rr;
}

bool Is_full(GMatrix64 mat){
    GMatrix64 mat_cpy;
    int nz_row, nz_col;     /* equal nzero_row in Inverse() */
    int i, j;
    uint64_t temp, jtimes;

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
            jtimes = galois_single_divide_64(mat_cpy.Get(j,nz_col), mat_cpy.Get(i,nz_col));
            mat_cpy.Row_plus_irow(j, i, jtimes);
        }
    }
    return true;
}

GMatrix64 Mat8to64(GMatrix& mat){
    GMatrix64 mat_ret;
    int r = mat.rr;
    int c = mat.cc;
    mat_ret.rr = mat.rr;
    mat_ret.cc = mat.cc;
    
    assert(mat.ww == 8);
    mat_ret.ele.resize(mat.rr*mat.cc, 0);

    for(int i = 0; i < r; ++i){
        for(int j = 0; j < c; ++j){
            mat_ret.ele[i*c+j] = (uint8_t)mat.ele8[i*c+j];
        }
    }

    return mat_ret;
}


GMatrix64 Mat16to64(GMatrix& mat){
    GMatrix64 mat_ret;
    int r = mat.rr;
    int c = mat.cc;
    mat_ret.rr = mat.rr;
    mat_ret.cc = mat.cc;
    
    assert(mat.ww == 16);
    mat_ret.ele.resize(mat.rr*mat.cc, 0);

    for(int i = 0; i < r; ++i){
        for(int j = 0; j < c; ++j){
            mat_ret.ele[i*c+j] = (uint16_t)mat.ele16[i*c+j];
        }
    }

    return mat_ret;
}

GMatrix64 Mat32to64(GMatrix& mat){
    GMatrix64 mat_ret;
    int r = mat.rr;
    int c = mat.cc;
    mat_ret.rr = mat.rr;
    mat_ret.cc = mat.cc;
    
    assert(mat.ww == 32);
    mat_ret.ele.resize(mat.rr*mat.cc, 0);

    for(int i = 0; i < r; ++i){
        for(int j = 0; j < c; ++j){
            mat_ret.ele[i*c+j] = (uint32_t)mat.ele32[i*c+j];
        }
    }

    return mat_ret;
}
