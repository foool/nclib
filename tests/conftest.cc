#include "../config.h"

int main(void){
    int port;
    string ip, user, section;
    const char filename[] = "conf.txt";

    Config newconf;
    newconf.SetSection("section");
    newconf.SetDelimiter("=");
    newconf.Add("ip","0.0.0.0");
    newconf.Add("port",25);
    newconf.Add("user","liuqing");
    newconf.Write(filename);
    
    Config conf(filename);
    port = conf.Read("port", 0);
    ip = conf.Read("ip", ip);
    user = conf.Read("user", user);
    section = conf.GetSection();
    cout<<"Section is "<<section<<endl;
    cout<<"Port is "<<port<<endl;
    cout<<"IP is "<<ip<<endl;
    cout<<"user is "<<user<<endl;

    conf.Write("conf.txt");
}
