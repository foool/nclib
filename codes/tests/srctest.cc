#include "../../nc.h"
#include "../src.h"

int main(void){
    FILE* fp;
    unsigned char* con;
    string conf_file("src.conf");
    string en_file("test.txt");
    string en_path("./");
    SRC src(conf_file);
    int i, j;

    con = (unsigned char*)malloc(256+1);
    for(i = 0; i < 4; i++){
        for(j = 0; j < 64; j++){
            *(con+i*64+j) = i;
        }
    }
    NOTE("SRC coding method's metadata");
    src.SRCmeta_print();      //Output SRC meta

    NOTE("Propering the test file!");
    if(NULL == (fp = fopen("test.txt","w"))){
       printf("Cann't create this file\n"); 
    }
    fwrite(con, 256, 1, fp);
    fclose(fp);
    free(con);

    NOTE("Begin to encode file !"); 
    src.Encode(en_file, en_path);
 
    // repair
    NOTE("Invalid a node to repair!");
    int del_file_idx = 1;
    char del_filename[128];
    string de_path = en_path;
    sprintf(del_filename, "%s%c%02d", (en_path+en_file).c_str(), '_', del_file_idx);

    if(remove(del_filename)!=0){
        printf("Error when deleting file %s\n", del_filename);
    }
    cout<<"file "<<del_filename<<" deleted!"<<endl;
    src.Repair(de_path+en_file, del_file_idx);

    // decode
    NOTE("Decode");
    del_file_idx = 1;
    sprintf(del_filename, "%s%c%02d", (en_path+en_file).c_str(), '_', del_file_idx);

    if(remove(del_filename)!=0){
        printf("Error when deleting file %s\n", del_filename);
    }
    cout<<"file "<<del_filename<<" deleted!"<<endl;
    string files_idx;
    for(int i = 0; i < src.n; i++){
        if(i == del_file_idx){
            continue;
        }
        stringstream ss;
        ss << i;
        files_idx = files_idx+' '+ss.str();
    }
    cout<<"rest files indexes:"<<files_idx<<endl;
    src.Decode(de_path, en_file, files_idx);

}
