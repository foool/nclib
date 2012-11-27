#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../galois.h"
#include "../nc.h"

#define CMD_NUM 16

char *command[CMD_NUM]={\
    (char*)"print", (char*)"rank", (char*)"is_full", (char*)"transpose",
    (char*)"del_row", (char*)"del_col", (char*)"wipe_matrix", 
    (char*)"swap_rows", (char*)"swap_cols", (char*)"del_rows", 
    (char*)"del_cols", (char*)"row_plus_row", (char*)"row_to_irow", 
    (char*)"make_random",(char*)"row_plus_irow", (char*)"wipe_range"};

void usage(){
    printf(">>>Usage:\n");
    printf(">>>print/rank/is_fulli/transpose\n");
    printf(">>>del_row/del_col/wipe_matrix argument1 \n");
    printf(">>>swap_rows/swap_cols/del_rows/del_cols/row_plus_row/row_to_irow argument1 argument2\n");
    printf(">>>make_random/row_plus_irow/wipe_range argument1 argument2 argument3\n");
}

int main(void){
    GMatrix m1;
    char *str;
    char *cmd;
    int i, j;
    int argu_num;
    int r[3] = {0,0,0};
    
    str = new char(40);
    cmd = new char(10);
    m1.Make_random(4, 4, 8);

    while(1){
        argu_num = 0;   /* numbers of arguments */
        printf(">>>");
        fgets(str, 40, stdin); 

        /* Get cmd from str */
        for(i = 0; (str[i]!=' ')&&(str[i]!='\0')&&(str[i]!='\n'); i++){
            cmd[i] = str[i];
            if(i > 15){
                printf("tooo looong command\n");
                exit(0);
            }
        }
        cmd[i]= '\0';

        /* Get r1, r2, r3, if have*/
        for(j = 0; j < 3; j++){
            while((str[i] < '0')||(str[i] > '9')){
                if('\0' == str[i]){
                    break;
                }
                ++i;
            }
            if(str[i] == '\0'){
                break;
            }
            if((str[i] >= '0')&&(str[i] <= '9')){
                sscanf(&str[i],"%d", &r[j]);
                ++argu_num;
            }
            while((str[i] >= '0')&&(str[i] <= '9')){
                if('\0' == str[i]){
                    break;
                }
                ++i;
            }
        }

        printf("r[3] = { %d %d %d }\n",r[0], r[1], r[2]);
        printf("Command is %s , argu_num = %d\n", cmd, argu_num);
        
        for(j = 0; j < CMD_NUM; j++){
            if(0 == strcmp(cmd, command[j])){
                switch(j){
                    case 0:
                        /*  print 0 */
                        NOTE(" Matrix ");
                        m1.Print();
                        break;
                    case 1:
                        /*  rank 0 */
                        NOTE(" Matrix ");
                        m1.Print();
                        printf("Rank: %d\n", Rank(m1));
                        break;
                    case 2:
                        /* is_full 0 */
                        NOTE(" Matrix ");
                        m1.Print();
                        if(1 == Is_full(m1)){
                            NOTE(" full rank matrix ");
                        }else{
                            NOTE(" Non-full rank matrix ");
                        }
                        break;
                    case 3:
                        /* transpose 0 */
                        NOTE(" Matrix ");
                        m1.Print();
                        m1 = Transpose(m1);
                        NOTE(" After execution ");
                        m1.Print();
                        break;
                    case 4:
                        /* del_row 1 */
                        if(argu_num < 1){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Del_row(r[0]);
                        NOTE(" After execution ");
                        m1.Print();
                        break;
                    case 5:
                        /* del_col 1 */
                        if(argu_num < 1){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Del_col(r[0]);
                        NOTE(" After execution ");
                        m1.Print();
                        break;
                    case 6:
                        /* wipe_matrix 1 */
                        if(argu_num < 1){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Wipe_matrix(r[0]);
                        NOTE(" After execution ");
                        m1.Print();
                        break;
                    case 7:
                        /* swap_rows 2 */
                        if(argu_num < 2){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Swap_rows(r[0], r[1]);
                        NOTE(" After execution ");
                        m1.Print();
                        break;
                    case 8:
                        /* swap_cols 2 */
                        if(argu_num < 2){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Swap_cols(r[0], r[1]);
                        NOTE(" After execution ");
                        m1.Print();
                        break;
                    case 9:
                        /* del_rows 2 */
                        if(argu_num < 2){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Del_rows(r[0], r[1]);
                        NOTE(" After execution ");
                        m1.Print();
                        break;
                    case 10:
                        /* del_cols 2 */
                        if(argu_num < 2){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Del_cols(r[0], r[1]);
                        NOTE(" After execution ");
                        m1.Print();
                        break;
                    case 11:
                        /* row_plus_row 2 */
                        if(argu_num < 2){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Row_plus_row(r[0], r[1]);
                        NOTE(" After execution ");
                        m1.Print(); 
                        break;
                    case 12:
                        /* row_to_irow 2 */
                        if(argu_num < 2){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Row_to_irow(r[0], r[1]);
                        NOTE(" After execution ");
                        m1.Print(); 
                        break;
                    case 13:
                        /* make_random 3 */
                        if(argu_num < 3){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Make_random(r[0], r[1], r[2]);
                        NOTE(" After execution ");
                        m1.Print(); 
                        break;
                    case 14:
                        /* row_plus_irow 3 */
                        if(argu_num < 3){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Row_plus_irow(r[0], r[1], r[2]);
                        NOTE(" After execution ");
                        m1.Print(); 
                        break;
                    case 15:
                        /* wipe_range 3 */
                        if(argu_num < 3){
                            usage();
                            break;
                        }
                        NOTE(" Matrix ");
                        m1.Print();
                        m1.Wipe_matrix(r[0], r[1], r[2]);
                        NOTE(" After execution ");
                        m1.Print(); 
                        break;
                    default:
                        break;
                }
                break;
            }
        } /* for */
        if(CMD_NUM == j){
            usage();
        }
    } /* while */
} /* main */
