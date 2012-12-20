#include "../../nc.h"
#include "../reed_sol.h"

int main(void){
    FILE* fp;
    unsigned char* con;
    string conf_file("reed_sol.conf");
    string en_file("test.txt");
    Reed_Sol rs(conf_file);
    int i , j;

    NOTE("SRC coding method's metadata");
    rs.meta_print();      //Output SRC meta

    NOTE("Propering the test file!");
    con = (unsigned char*)malloc(256+1);
    for(i = 0; i < 4; i++){
        for(j = 0; j < 64; j++){
            *(con+i*64+j) = i;
        }
    }
    if(NULL == (fp = fopen("test.txt","w"))){
       printf("Cann't create this file\n"); 
    }
    fwrite(con, 256, 1, fp);
    fclose(fp);
    free(con);

    
    // Encode
    NOTE("Begin to encode file !"); 
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    rs.Encode();


    // Decode
    NOTE("Begin to decode files !");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    string files_idx("0 2");
    rs.SetDepath("./", "./decoded/");
    rs.Decode(files_idx);

    // S repair
    remove("test.txt_00");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    rs.Repair(0);
    
    // P repair
    remove("test.txt_02");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    rs.Repair(2);

    // Q repair
    remove("test.txt_03");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    rs.Repair(3);
    
    // S + Q repair
    remove("test.txt_01");
    remove("test.txt_03");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    cout<<"Repair node 1 and node 3!"<<endl;
    rs.Repair("1 3");

    // S + S repair
    remove("test.txt_00");
    remove("test.txt_01"); 
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    cout<<"Repair node 0 and node 1!"<<endl;
    rs.Repair("0 1");
}
