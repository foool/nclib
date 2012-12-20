#include "config.h"

Config::Config(){
    delimiter = string(1,'=');
}
Config::Config(string sec, string delim){
    section = sec;
    delimiter = delim;
}
Config::Config(string filename){
    size_t pos = -1;
    string key, value, line;
    int lbracket, rbracket;
    ifstream in(filename.c_str());
    
    delimiter = string(1,'=');
    if(!in){
        throw File_not_found(filename);
    }

    //get section and key-values from in
    //get section
    getline(in, line);
    lbracket = (int)(line.find_first_of("["));
    rbracket = (int)(line.find_last_of("]"));
    section = line.substr(lbracket+1,(rbracket-lbracket-1));

    //get key-value
    getline(in, line);
    while(in||(line.length()>0)){
        pos = line.find(delimiter);
        if(pos == string::npos){
            getline(in, line);
            continue;
        }
        key = line.substr(0, (int)pos);
        value = line.substr((int)pos+1, line.length()-1);

        Trim(key);
        Trim(value);
        consMap[key] = value;
        getline(in, line);
    }
    in.close();
}
int Config::Write(const string& filename){
    int ret = 0;
    mapi p;
    ofstream out(filename.c_str());

    //section
    out<<"["<<section<<"]"<<endl;

    //key-value
    for(p = consMap.begin(); p != consMap.end(); ++p){
        ret++;
        out<<p->first<<delimiter<<p->second<<endl;
    }
    out.close();

    return ret;
}

void Config::SetSection(const string& sec){
    section = sec;
}

string Config::GetSection(){
    return section;
}

void Config::SetDelimiter(const string& delim){
    delimiter = delim;
}

ToStringHelper Config::string_to_T(string const& s)const{
    return ToStringHelper(s);
}

//static
void Config::Trim( string& str ){  
    static const char whitespace[] = " \n\t\v\r\f";  

    str.erase(0, str.find_first_not_of(whitespace));
    str.erase(str.find_last_not_of(whitespace)+1);  
}

bool Config::KeyExist( const string& key ) const  {   
    mapci p = consMap.find( key );  
    
    return ( p != consMap.end() );  
}

bool Config::FileExist(string filename){
    bool exist = false;

    ifstream in(filename.c_str());
    if(in){
        exist = true;
    }
    return exist;
}
