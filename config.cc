#include "config.h"

Config::Config(){
    delimiter = string(1,'=');
}
Config::Config(string sec, string delim){
    section = sec;
    delimiter = delim;
}
Config::Config(string filename){
    size_t pos;
    string key, value, line;
    size_t lbracket, rbracket;
    ifstream in(filename.c_str());
    typedef pair <string, string> Int_Pair;
    delimiter = string(1,'=');
    if(!in){
        throw File_not_found(filename);
    }


    //get key-value
    getline(in, line);
    while(in||(line.length()>0)){
        lbracket = (int)(line.find_first_of("["));
        rbracket = (int)(line.find_last_of("]"));
        if((lbracket != string::npos) && (rbracket != string::npos) && (lbracket != rbracket))
        {
            section = line.substr(lbracket+1,(rbracket-lbracket-1));
            getline(in, line);
            continue;
        }
        pos = line.find(delimiter);
        if(pos == string::npos){
            getline(in, line);
            continue;
        }
        key = line.substr(0, (int)pos);
        value = line.substr((int)pos+1, line.length()-1);
        if(true == section.empty())
        {
            throw Format_wrong("conf file format error");
        }
        Trim(key);
        Trim(value);
        consMap[section].insert(Int_Pair(key, value));
        getline(in, line);
    }
    in.close();
}



int Config::Write(const string& filename){
    int ret = 0;
    mapi_f p_f;
    mapi_s p_s;
    ofstream out(filename.c_str());
    for(p_f = consMap.begin(); p_f != consMap.end(); ++p_f)
    {
        out<<"["<<p_f->first<<"]"<<endl;
        for(p_s = (p_f->second).begin(); p_s != (p_f->second).end(); ++p_s)
        {
            ret++;
            out<<p_s->first<<delimiter<<p_s->second<<endl;
        }
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
    mapci_f iter1 = consMap.find(section);
    if(iter1 == consMap.end())
    {
        return false;
    }
    else
    {
        mapci_s iter2 = (iter1->second).find(key);
        if(iter2 == (iter1->second).end())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    //mapci p = consMap.find( key );  
    
    //return ( p != consMap.end() );  
}

bool Config::FileExist(string filename){
    bool exist = false;

    ifstream in(filename.c_str());
    if(in){
        exist = true;
    }
    return exist;
}
