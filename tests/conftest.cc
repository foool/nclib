#include "../config.h"

int main(void){
    int port;
    string ip, user, section, mat;
    const char filename[] = "conf.txt";

    Config newconf;
    newconf.SetSection("section");
    newconf.SetDelimiter("=");
    newconf.Add("ip","0.0.0.0");
    newconf.Add("port",25);
    newconf.Add("user","liuqing");
    newconf.Add("mat","1 1 1 2");
    newconf.Write(filename);
    
    Config conf(filename);
    port = conf.Read("port", 0);
    ip = conf.Read("ip", ip);
    user = conf.Read("user", user);
    mat = conf.Read("mat", mat);
    section = conf.GetSection();

    cout<<"Section is "<<section<<endl;
    cout<<"Port is "<<port<<endl;
    cout<<"IP is "<<ip<<endl;
    cout<<"user is "<<user<<endl;
    cout<<"mat is "<<mat<<endl;

    conf.Write("conf.txt");
}
