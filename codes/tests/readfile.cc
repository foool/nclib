#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int len;
    string filename(argv[1]);
    FILE* fp;
    char cfile[123];
    char *p_buff;
    char *pf;

    if(argc <= 2){
        printf("usage: ./a.out filename length");
        return -1;
    }

    len = atoi(argv[2]);
    cout<<"filename:"<<filename<<endl;
    cout<<"filelen:"<<len<<endl;

    p_buff = (char *)malloc(len);
    memset(p_buff, 9, len+1);
    sprintf(cfile , "%s" , filename.c_str());
    if((fp = fopen(cfile,"r")) == NULL){
        cout<<"Can't open file:"<<filename<<endl;
    }
    fread(p_buff, len, 1, fp);
    pf = p_buff;

    for(int i = 0; i < len ; ++i){
        printf("%d",*(pf+i));
    }
    printf("\n");
    fclose(fp);
    free(p_buff);
}
