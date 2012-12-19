#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* test Draw_rows */
int main(int argc, char *argv[]){
    GMatrix mat(4, 4, 8);
    GMatrix mat_new(4, 4, 8);
    
    NOTE("Identity");
    mat.Make_identity(4,4,8);
    mat.Print();
    NOTE("New matrix");
    mat_new.Make_zero(4,4,8);
    mat_new.Print();
    NOTE("Append");
    mat.Append_matrix(mat_new);
    mat.Print();

    return 1;
}
