#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"


/* mulitply {02} */
int main(void){
    int size = 128;
    int i=0;
    unsigned char *ibuff;
    unsigned char *obuff;
    unsigned char *obuff2;

    ibuff = (unsigned char *)malloc(size*sizeof(char));
    obuff = (unsigned char *)malloc(size*sizeof(char));
    obuff2 = (unsigned char *)malloc(size*sizeof(char));
    memset(ibuff, 166, size);
    memset(obuff, 166, size);

    galois_w16_region_multby_2(ibuff, size);
    galois_w16_region_multby_2_64(obuff, size);

    galois_w32_region_multby_2(ibuff, size);
    galois_w32_region_multby_2_64(obuff, size);

    if(0 == memcmp(obuff, ibuff, size)){
        printf("The same\n");
    }else{
        printf("The difference\n");
    }

    do{
        printf("%d ",ibuff[i]);
        ++i;
    }while(i < 128);
    i = 0;
    printf("\n");
    do{
        printf("%d ",obuff[i]);
        ++i;
    }while(i < 128);
    printf("\n");



    free(ibuff);
    free(obuff);
    free(obuff2);
}
