#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include "../galois.h"
#include "../nc.h"

using namespace std;

/* Compute rank of a matrix and select lines */
int main(void){
    char ss[100];
    char *ps;
    int count;
    int linenum = 0;
    int val[100];
    string css;
    GMatrix mat, mat_add, mat_;

    while(fgets(ss, 100, stdin)!=NULL){
        //printf("\n new line\n");
        //puts(ss);
        
        ps = &ss[0];
        count = 0;
        
        while(*ps == ' ' || (*ps >= '0' && *ps <= '9')){
            if(*ps == ' '){
                ++ps;
                if(*ps < '0' || *ps > '9'){
                    break;
                }
            }

            sscanf(ps, "%d", &val[count]);
            //cout<<val[count]<<endl;
            ++count;
            while(*ps >= '0' || *ps >= '9'){
                ++ps;
            }
        }
        if(count == 0){
            break;
        }

        mat_add.Make_zero( 1 , count, 8);
        for(int i =0 ; i < count ; ++i){
            mat_add.Set(0, i, val[i]);    
        }

        if(linenum == 0){
            mat = mat_add;
        }else{
            if(mat_add.cc != mat.cc){
                printf("column\n");
                break;
            }
            mat.Append_matrix(mat_add);
        }
        ++linenum;
            
    }

    NOTE("mat");
    mat.Print();
    printf("The rank of the matrix is %d\n", Rank(mat));

    while(1){
        printf("which lines ？\n");
        std::getline(cin, css);
        //cout<<_Trim(css)<<endl;
        count = 0;

        stringstream scs(_Trim(css));
        int tmp;
        while(!scs.eof()){
            scs >> tmp;
            ++count;
        }
        mat_ = Draw_rows(mat, css, count);
        printf("count = %d\n",count);
        NOTE("mat_");
        mat_.Print();
        printf("The rank of the matrix is %d\n", Rank(mat_));
    }

    return 1;
}
