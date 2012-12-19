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

    Bat_Write(file_str, 6, out_buff, 6, string("./"));
    string files_index("0 1 2 3");
    file_num = Bat_Read(file_str, files_index, in_buff, 6);
    for(i = 0; i < file_num*6; i++){
        printf("%c ",*(in_buff+i));
    }
    printf("\n");
    files_index = files_index+" 4 5";
    cout<<"file_str:"<<file_str<<endl;
    cout<<"files_index:"<<files_index<<endl;
    
    Bat_Delete(file_str, files_index);


    vector<int> files_list;
    for(int i = 0; i < 6; ++i){
        files_list.push_back(i);
    }
    Bat_Write(file_str, 6, out_buff, 6, string("./"));
    memset(in_buff , 0, 36);
    Bat_Read(file_str, files_list, in_buff, 6);
    if(0 == memcmp(in_buff , out_buff , 36)){
        printf("in_buff = out_buff\n");
    }else{
        printf("in_buff != out_buff\n"); 
    }
    Bat_Delete(file_str, files_list);

    free(out_buff);
    free(in_buff);
    return 1;
}
