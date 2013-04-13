#include "nc.h"
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

