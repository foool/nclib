#include "../galois.h"
#include "../nc.h"
#include "../config.h"
#include "../utils.h"
#include "../property.h"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//next_permutation and CL_property
int main(int argc, char *argv[]){
    vector<int> vv;
    GMatrix mat;

    vv.push_back(0);
    vv.push_back(1);
    vv.push_back(2);
    vv.push_back(3);

    for(int i=0; i < vv.size(); i++){
        printf("%d ", vv.at(i));
    }
    printf("\n");

    NOTE("permutation");
    while(next_permutation(vv)){
        for(int i=0; i < vv.size(); i++){
            printf("%d ", vv.at(i));
        }
        printf("\n");
    }
   
    sort_v_i(vv);
    vv.pop_back();
    NOTE("conbination");
    while(next_combination(vv, 5)){
        for(int i=0; i < vv.size(); i++){
            printf("%d ", vv.at(i));
        }
        printf("\n");
    }

    NOTE("CL_property");
    mat.Make_random(15, 8, 8);
    if(CL_property(mat, 5, 3)){
        printf("True!\n");
    }else{
        printf("False!\n");
    }

}

