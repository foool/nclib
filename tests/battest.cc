#include "nc.h"

int main(void){
    int i, file_num;
    const char* filename = "test_for_bat.txt";
    string file_str(filename);
    unsigned char* out_buff, * in_buff;

    out_buff = (unsigned char*)calloc(36,1);
    in_buff = (unsigned char*)calloc(36,1);

    for(i = 0; i < 36; i++){
        *(out_buff+i) = i+65;
    }
    memset(in_buff, 255, 36);

    Bat_Write(file_str, 6, out_buff, 6);
    string files_index("0 1 2 3");
    file_num = Bat_Read(file_str, files_index, in_buff, 6);
    for(i = 0; i < file_num*6; i++){
        printf("%c ",*(in_buff+i));
    }
    printf("\n");
    files_index = files_index+" 4 5";
    Bat_Delete(file_str, files_index);

    return 1;
}
