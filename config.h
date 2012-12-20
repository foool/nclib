#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class ToStringHelper{
    std::string myValue;
    public:
        ToStringHelper(std::string const& value): myValue(value){
        
        }
        operator std::string() const{

            return myValue;
        }
        template <typename T>operator T() const{
            std::istringstream cvt(myValue);
            T results;
            
            cvt >> results;
            
            return results;
        }
};

class Config{
    public:
        string delimiter;
        string section;
        map<string,string> consMap;
        typedef map<string, string>::iterator mapi;
        typedef map<string, string>::const_iterator mapci;

        Config();
        Config(string section, string delimiter);
        Config(string filename);
        

        template<class T> T Read(const string& key)const;
        template<class T> T Read(const string& key, const T& value)const;
        
        int Write(const string& filename);
        template<class T> void Add(const string& key, const T& value);
        void SetSection(const string& sec);
        string GetSection();
        void SetDelimiter(const string& delim);

        bool FileExist(string filename);
        bool KeyExist(const string& key) const;

        template<class T> static string T_to_string(const T& t);
        //template<class T> static T string_to_T(const string& s);
        ToStringHelper string_to_T(string const& s)const;

        static void Trim(string& rstr);
        
        //Exceptions
        struct File_not_found{
            string filename;
            File_not_found(const string& filename_ = string()):filename(filename_){}
        };
        struct Key_not_found{
            string key;
            Key_not_found(const string& key_ = string()):key(key_){}
        };
        struct Section_not_found{
            string section;
            Section_not_found(const string& section_ = string()):section(section_){}
        };
        struct Char_forbidden{
            string character;
            Char_forbidden(const string& character_ = string()):character(character_){}
        };
};    



    template<class T> string Config::T_to_string(const T& t){
        ostringstream ost;

        ost << t;
        
        return ost.str();
    }
/*
    template<class T> T Config::string_to_T(const string& s){
        T t;
        bool at_in = false;
        
        if(s.find('@') != string::npos){
            //at_in= true;
            
        }
        istringstream ist(s);
        ist >> t;

        return t;
    }
*/
    template<class T> T Config::Read(const string& key)const{
        mapci p = consMap.find(key);
        
        if(p == consMap.end()){
            cout<<"Cann't find key: "<<key<<endl;
            throw Key_not_found(key);
        }
        return string_to_T( p->second );
    }
    
    template<class T> T Config::Read(const string& key, const T& value)const{
        mapci p = consMap.find(key);

        if(p == consMap.end()){
            cout<<"Cann't find key: "<<key<<endl;
            throw Key_not_found(key);
        }
        
        return string_to_T( p->second );
    }


    template<class T> void Config::Add(const string& key, const T& value){
        string ckey = key;
        string cvalue = T_to_string(value);
        
        Trim(ckey);
        Trim(cvalue);

        /*check for @, which we used it to replace blank space
        if(cvalue.find('@') != string::npos){
            throw Char_forbidden("@ ");
        }
        for(int i = 0; i < cvalue.length(); i++){
            if(cvalue[i] == ' '){
                cvalue[i] = '@';
            }
        }*/
        consMap[ckey] = cvalue;
    }
