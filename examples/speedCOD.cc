#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

void usage(){
    printf("./speedCOD w mat_types row col size iteration\n");
    printf("w : 4 8 16 32 64\n");
    printf("mat_types: \n0 : random matrix\n1 : vandermonde matrix\n");
    printf("methods:\n");
    printf("0 : dot product\n");
    printf("1 : dot product sheduled\n");
    printf("1 : dot product sheduled,gf-\n");
    printf("2 : shift coding\n");
    printf("3 : shift coding sheduled\n");
    printf("4 : shift coding(gf-)\n");
}

/* Speed of different coding method */
int main(int argc, char*argv[]){
    ntime t_begin, t_end;
    int row, col;
    unsigned char * in_buff;
    unsigned char * out_buff;
    unsigned long size_in_buff;
    unsigned long size_ot_buff;
    int count;
    int mat_types;
    int w;
    int meth;
    int iter;
    int val;
    gf_t gfm;

    if(argc != 7){
        usage();
        exit(1);
    }else{
        w = atoi(argv[1]);
        mat_types = atoi(argv[2]);
        row = atoi(argv[3]);
        col = atoi(argv[4]);
        size_in_buff = atoi(argv[5]);
        iter = atoi(argv[6]);
        if(w != 4 && w != 8 && w != 16 && w != 32 && w != 64){
            printf("bad w = %d!\n",w);
            exit(1);
        }
        if(mat_types != 0 || mat_types != 1){
            printf("bat mat_types =%d\n", mat_types);
        }
        if(row <= 0 || col <= 0){
            printf("row(col) must bigger than 0\n");
            exit(1);
        }
        printf("matrix with w = %d, %d rows, %d cols\n",w,row,col);
        if(iter <= 0){
            printf("Bad iterations!\n");
            exit(1);
        }
        if(size_in_buff <= 0){
            printf("bad size!\n");
            exit(1);
        }else{
            if(size_in_buff < 1024){
                printf("buffer size: %3f Bytes\n", size_in_buff*1.0);
            }else{
                if(size_in_buff < 1024*1024){
                    printf("buffer size: %3.2f KB\n", size_in_buff/1024.0);
                }else{
                    printf("buffer size: %04.2f MB\n", size_in_buff/(1024.0*1024));
                }
            }
        }
        printf("iterations : %d times\n",iter);
    }

    printf("===================================\n");
    size_ot_buff = size_in_buff*col/row;
    in_buff = (unsigned char *)malloc(size_in_buff);
    out_buff = (unsigned char *)malloc(size_ot_buff);
    
    //printf(" random set \n");
    unsigned char *pl = in_buff;
    unsigned char *pend;
    pend = pl + size_in_buff;
    srand(time(NULL));
    while(pl < pend){
        *pl = rand()%256;
        pl++;
    }

    if(!gf_init_easy(&gfm, w)){
        printf("Bad gf spec\n");
        exit(1);
    }

    switch(w){
        case 4:
        {
            GMatrixU8 mat4;
            if(mat_types == 0){
                mat4.Make_random(row, col, 4);
            }else{
                mat4.Make_vandermonde(row, col, 4);
            }
            NOTE("coding mat");
            mat4.Print();
                    
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w4coding(mat4, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding):\t\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w4coding_(mat4, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding sheduled):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w4coding_opt(mat4, out_buff, in_buff, size_in_buff, gfm);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            break;       
        }

        case 8:
        {
            GMatrix mat8;
            if(mat_types == 0){
                mat8.Make_random(row, col, 8);
            }else{
                mat8.Make_vandermonde(row, col, 8);
            }
            NOTE("coding mat");
            mat8.Print();
                    
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    matrix_coding(mat8, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(dot product):\t\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    matrix_coding_(mat8, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(dot product sheduled):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    matrix_coding_opt(mat8, out_buff, in_buff, size_in_buff,gfm);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(dot product she..,gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w8coding(mat8, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding):\t\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w8coding_(mat8, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding sheduled):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w8coding_opt(mat8, out_buff, in_buff, size_in_buff, gfm);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            break;       
        }

        case 16:
        {
            GMatrix mat16;
            if(mat_types == 0){
                mat16.Make_random(row, col, 16);
            }else{
                mat16.Make_vandermonde(row, col, 16);
            }
            NOTE("coding mat");
            mat16.Print();
                    
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    matrix_coding(mat16, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(dot product):\t\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    matrix_coding_(mat16, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(dot product sheduled):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    matrix_coding_opt(mat16, out_buff, in_buff, size_in_buff, gfm);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(dot product she..,gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w16coding(mat16, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding):\t\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w16coding_(mat16, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding sheduled):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w16coding_opt(mat16, out_buff, in_buff, size_in_buff, gfm);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            break;       
        }
    

        case 32:
        {
            GMatrix mat32;
            if(mat_types == 0){
                mat32.Make_random(row, col, 32);
            }else{
                mat32.Make_vandermonde(row, col, 32);
            }
            NOTE("coding mat");
            mat32.Print();
                    
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    matrix_coding(mat32, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(dot product):\t\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    matrix_coding_(mat32, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(dot product sheduled):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    matrix_coding_opt(mat32, out_buff, in_buff, size_in_buff, gfm);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(dot product she..,gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w32coding(mat32, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding):\t\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w32coding_(mat32, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding sheduled):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w32coding_opt(mat32, out_buff, in_buff, size_in_buff, gfm);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            break;       
        }

        case 64:
        {
            GMatrix64 mat64;
            if(mat_types == 0){
                mat64.Make_random(row, col);
            }else{
                mat64.Make_vandermonde(row, col);
            }
            NOTE("coding mat");
            mat64.Print();

            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w64coding(mat64, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding):\t\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w64coding_(mat64, out_buff, in_buff, size_in_buff);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding sheduled):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            
            count = iter;
            gettimeofday(&t_begin , NULL);
                while(--count >= 0){
                    w64coding_opt(mat64, out_buff, in_buff, size_in_buff, gfm);
                }
            gettimeofday(&t_end, NULL);
            printf("Speed(shift coding gf-):\t%4.3f MB/S\n", (size_in_buff/(1024.0*1024/iter))/((t_end.tv_sec-t_begin.tv_sec) + (t_end.tv_usec - t_begin.tv_usec)/(1000.0*1000.0)));
            break;       
        }



    }
    free(in_buff);
    free(out_buff);
}
