#include "inttypes.h"
#include "stdlib.h"
#include "stdio.h"
#include <sys/time.h>

#define  ntime struct timeval
int gettimeofday(struct timeval *tv,struct timezone *tz);


int main(void){
        uint32_t a, b, c;
            uint64_t x, y, z;
                ntime t_begin, t_end;

                    gettimeofday(&t_begin , NULL);
                        for(int i = 0; i < 10000; i++){
                                    for(int j = 0; j < 10000; j++){
                                                    c = a^b;   
                                                            }
                                        }
                            gettimeofday(&t_end, NULL);
                                printf("%d s %d us\n", (t_end.tv_sec-t_begin.tv_sec) , (t_end.tv_usec-t_begin.tv_usec));
                                    
                                    
                                    gettimeofday(&t_begin , NULL);
                                        for(int i = 0; i < 10000; i++){
                                                    for(int j = 0; j < 10000; j++){
                                                                    z = x^y;   
                                                                            }
                                                        }
                                            gettimeofday(&t_end, NULL);
                                                printf("%d s %d us\n", (t_end.tv_sec-t_begin.tv_sec) , (t_end.tv_usec-t_begin.tv_usec));
}
