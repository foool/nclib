#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

/* Test w4coding_opt */
int main(int argc, char*argv[]){
    ntime t_begin, t_end;
    int cm;
    int k;
    int m;
    unsigned char * in_buff;
    unsigned char * ot_buff, *ot_buff2;
    unsigned char *pend, *pl;
    unsigned long size_buff = 256;
    gf_t gfm, gfm8;
    GMatrixU8 mat4, mat4_i, mat4_r;
    GMatrix mat8, mat8_i, mat8_r;
    GMatrix mat16, mat16_i, mat16_r;
    GMatrix mat32, mat32_i, mat32_r;
    GMatrix64 mat64, mat64_i, mat64_r;

    in_buff = (unsigned char *)malloc(size_buff);
    ot_buff = (unsigned char *)malloc(size_buff);
    ot_buff2 = (unsigned char *)malloc(size_buff);

    pend = in_buff + size_buff;
    pl = in_buff;
    srand(time(NULL));
    while(pl < pend){
        *pl = rand()%256;
        pl++;
    }

        printf("------------------------------------------\n");
        printf(" in_buff:\n");
        mem_print(in_buff, 64, 32);
    
    if(!gf_init_easy(&gfm, 4)){
        printf("Bad gf spec\n");
        exit(1);
    }

    mat4.Make_random(2, 2, 4);
    mat4_i = mat4;
    mat4_i.Inverse();
    mat4_r = Prod(mat4, mat4_i);
    NOTE("mat4");
    mat4.Print();
    NOTE("mat4_i");
    mat4_i.Print();
    NOTE("mat4_r");
    mat4_r.Print();

    w4coding_(mat4, ot_buff, in_buff, size_buff);
    //w4coding(mat4, ot_buff2, in_buff, size_buff);
    w4coding_opt(mat4, ot_buff2, in_buff, size_buff, gfm);

    if(0 == memcmp(ot_buff2, ot_buff, size_buff)){
        printf("The same\n");
    }else{
        printf("The different\n");
        printf("------------------------------------------\n");
        printf("ot_buff:\n");
        mem_print(ot_buff, 64, 32);
        printf("ot_buff2:\n");
        mem_print(ot_buff2, 64, 32);
    }

    if(!gf_init_easy(&gfm, 8)){
        printf("Bad gf spec\n");
        exit(1);
    }

    getchar();
    mat8.Make_random(2, 2, 8);
    mat8_i = mat8;
    mat8_i.Inverse();
    mat8_r = Prod(mat8, mat8_i);
    NOTE("mat8");
    mat8.Print();
    NOTE("mat8_i");
    mat8_i.Print();
    NOTE("mat8_r");
    mat8_r.Print();

    mem_print(in_buff, size_buff, 64);
    w8coding_(mat8, ot_buff, in_buff, size_buff);
    mem_print(ot_buff, size_buff, 64);
    //w8coding(mat8, ot_buff2, in_buff, size_buff);
    w8coding_opt(mat8, ot_buff2, in_buff, size_buff, gfm);
    mem_print(ot_buff2, size_buff, 64);

    if(0 == memcmp(ot_buff2, ot_buff, size_buff)){
        printf("The same\n");
    }else{
        printf("The different\n");
        printf("------------------------------------------\n");
        printf("ot_buff:\n");
        mem_print(ot_buff, 64, 32);
        printf("ot_buff2:\n");
        mem_print(ot_buff2, 64, 32);
    }

    if(!gf_init_easy(&gfm, 16)){
        printf("Bad gf spec\n");
        exit(1);
    }
    getchar();
    mat16.Make_random(2, 2, 16);
    mat16_i = mat16;
    mat16_i.Inverse();
    mat16_r = Prod(mat16, mat16_i);
    NOTE("mat16");
    mat16.Print();
    NOTE("mat16_i");
    mat16_i.Print();
    NOTE("mat16_r");
    mat16_r.Print();

    w16coding_(mat16, ot_buff, in_buff, size_buff);
    //w16coding(mat16, 16, ot_buff2, in_buff, size_buff);
    w16coding_opt(mat16, ot_buff2, in_buff, size_buff, gfm);

    if(0 == memcmp(ot_buff2, ot_buff, size_buff)){
        printf("The same\n");
    }else{
        printf("The different\n");
        printf("------------------------------------------\n");
        printf("ot_buff:\n");
        mem_print(ot_buff, 64, 32);
        printf("ot_buff2:\n");
        mem_print(ot_buff2, 64, 32);
    }


    if(!gf_init_easy(&gfm, 32)){
        printf("Bad gf spec\n");
        exit(1);
    }
    getchar();
    mat32.Make_random(2, 2, 32);
    mat32_i = mat32;
    mat32_i.Inverse();
    mat32_r = Prod(mat32, mat32_i);
    NOTE("mat32");
    mat32.Print();
    NOTE("mat32_i");
    mat32_i.Print();
    NOTE("mat32_r");
    mat32_r.Print();

    mem_print_(in_buff, size_buff, 64);
    w32coding_(mat32, ot_buff, in_buff, size_buff);
    //mem_print_(ot_buff, size_buff, 64);
    //w8coding(mat32, ot_buff2, in_buff, size_buff);
    w32coding_opt(mat32, ot_buff2, in_buff, size_buff, gfm);
    //mem_print_(ot_buff2, size_buff, 64);

    if(0 == memcmp(ot_buff2, ot_buff, size_buff)){
        printf("The same\n");
    }else{
        printf("The different\n");
        printf("------------------------------------------\n");
        printf("ot_buff:\n");
        mem_print(ot_buff, 64, 32);
        printf("ot_buff2:\n");
        mem_print(ot_buff2, 64, 32);
    }


    if(!gf_init_easy(&gfm, 64)){
        printf("Bad gf spec\n");
        exit(1);
    }
    getchar();
    mat64.Make_random(2, 2);
    mat64_i = mat64;
    mat64_i.Inverse();
    mat64_r = Prod(mat64, mat64_i);
    NOTE("mat64");
    mat64.Print();
    NOTE("mat64_i");
    mat64_i.Print();
    NOTE("mat64_r");
    mat64_r.Print();

    w64coding_(mat64, ot_buff, in_buff, size_buff);
    //w8coding(mat64, ot_buff2, in_buff, size_buff);
    w64coding_opt(mat64, ot_buff2, in_buff, size_buff, gfm);

    if(0 == memcmp(ot_buff2, ot_buff, size_buff)){
        printf("The same\n");
    }else{
        printf("The different\n");
        printf("------------------------------------------\n");
        printf("ot_buff:\n");
        mem_print(ot_buff, 64, 32);
        printf("ot_buff2:\n");
        mem_print(ot_buff2, 64, 32);
    }



    free(in_buff);
    free(ot_buff);
    free(ot_buff2);
}
