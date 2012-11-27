#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* Test GMatrix::Inverse()/Inverse()/Prod()/Is_full function */
int main(void){
    GMatrix m1;
    GMatrix m2;
    GMatrix *m3;
    int w;

    for(w = 8; w <= 32; w = w<<1){
        printf("        w = %d\n",w);
        NOTE(" Random Matrix m1");
        m1.Make_random(5, 5, w);
        while((1 == m1.Empty())||(-1 == Is_full(m1))){
            if(1 == m1.Empty()){
                printf("Empty!\n");
            }
            if(!Is_full(m1)){
                printf("Not full\n");
            }
            m1.Make_random(5, 5, w);
        }
        m1.Print();

        NOTE(" m2 = inversion of m1");
        m2 = Inverse(m1);
        m2.Print();
    
        NOTE("m3 = m1*m2");
        m3 = new GMatrix(5, 5, w);
        Prod(m3, &m1, &m2);
        m3->Print();
       
        getchar();
    }
}
