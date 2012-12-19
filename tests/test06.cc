#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* test Draw_rows */
int main(int argc, char *argv[]){
    GMatrix mat;
    GMatrix mat_new;
    int lst[3] = {0, 4, 9};
    string lst_str("1 2 3");
    
    NOTE("mat");
    mat.Make_sys_vandermonde(10, 6, 8);
    mat.Print();
    NOTE("mat_new");
    mat_new = Draw_rows(mat, lst, 3);
    mat_new.Print();
    printf("lst:%d %d %d\n",lst[0],lst[1],lst[2]);
    
    mat_new = Draw_rows(mat, lst_str, 3);
    mat_new.Print();
    cout<<lst_str<<endl;
    
    return 1;
}
