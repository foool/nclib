#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* Test Insert_matrix{2} Append_matrix{2} Replace_matrix{2}
 *      Slice_matrix{1}
 */

int main(void){
    GMatrix m1;
    GMatrix m2;
    int r1, r2;

    NOTE(" m1 Matrix ");
    m1.Make_random(4, 4, 16);
    m1.Print();
    NOTE(" m2 Matrix ");
    m2.Make_random(4, 4, 16);
    m2.Print();

    /* Insert_matrix */
    printf("Input the arguments of Insert_matrix(r1)\n");
    scanf("%d", &r1);
    m1.Insert_matrix(m2, r1);
    NOTE("After Insertion");
    m1.Print();

    /* Insert_matrix */
    printf("Input the arguments of Insert_matrix(r1, r2)\n");
    scanf("%d %d", &r1, &r2);
    m1.Insert_matrix(m2, r1, r2);
    NOTE("After Insertion");
    m1.Print();
    
    /* Append_matrix */
    NOTE(" Append a matrix ");
    m1.Print();
    m1.Append_matrix(m2);
    NOTE("After Insertion");
    m1.Print();

    /* Append_matrix */
    printf("Input the arguments of Append_matrix(r1)\n");
    scanf("%d, %d", &r1, &r2);
    m1.Append_matrix(m2, r1, r2);
    NOTE("After Insertion");
    m1.Print();

    /* Slice_matrix */
    printf("Input the arguments of Slice_matrix(r1, r2)\n");
    scanf("%d %d", &r1, &r2);
    NOTE(" m2 matrix ");
    m2.Print();
    m2 = Slice_matrix(m1, r1, r2);
    NOTE("After Insertion");
    m2.Print();


}
