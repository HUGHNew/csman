#pragma once
#include<string>
#include<unordered_map>
#include<iostream>
#include<json/json.h>
#include<fstream>
namespace Bri{//Some subsidiary functuions
    static void out(const std::string& str,std::ostream&os=std::cout,const char*end="\n"){
        os<<str;//shorten for output
        if(end=="\n")std::cout<<std::endl;
        else os<<end<<std::flush;
    }
    template<typename T>
    static void out(const T& str,std::ostream&os=std::cout,const char*end="\n"){
        os<<str;//shorten for output
        if(end=="\n")std::cout<<std::endl;
        else os<<end<<std::flush;
    }
    static void out(const std::initializer_list<std::string>&strlis,std::ostream&os=std::cout){
        for(auto str:strlis){//shorten for varible parameters output
            out(str,os," ");
        }out("",os);
    }
    template<typename Key,typename Value>
    static std::pair<Key,Value>Mp(Key&& key,Value&& val){
        /*to quickly make a pair, shorten the codes
         *the crutal instead of std::make_pair<Key,Value>
         */
        return std::make_pair<Key,Value>(std::move(key),std::move(val));
    }
    template<typename Key,typename Value>
    static std::pair<Key,Value>Mp(Key& key,Value& val){
        /*to quickly make a pair, shorten the codes
         *the crutal instead of std::make_pair<Key,Value>
         */
        return std::make_pair<Key,Value>(std::move(key),std::move(val));
    }
    template<typename Key,typename Value>
    static std::unordered_map<Key,Value>&Um(const std::initializer_list<std::pair<Key,Value>>&ini_pair){
        /*to quickly make an unordered_map
         *just the shorten for the unordered_map create
         */
        std::unordered_map<Key,Value> temp;
        for(auto const &t:ini_pair)
            temp.insert(t);
        return temp;
    }
    template<typename Key,typename Value>using mp=std::make_pair<Key,Value>;
    template<typename Key,typename Value>using um=std::unordered_map<Key,Value>;
}
class CsmanJson{
private:
    Json::Value Csman;
public:
    friend std::ostream& operator<<(std::ostream&,const CsmanJson&);
    friend std::ofstream& operator<<(std::ofstream&,const CsmanJson&);
    friend std::ifstream& operator>>(std::ifstream&,CsmanJson&);
    CsmanJson()=default;
    CsmanJson(const Json::Value&root):Csman(root){}
    void upgrade(const std::unordered_map<std::string,std::string>&ver){
        for(auto v:ver){
            if(Csman.isMember(v.first))Csman[v.first]=v.second;//has this member
            else Bri::out("Wrong Verion Infomation Input");
        }
    }
    bool Null(){
        return Csman.isNull();
    }
    bool read(std::ifstream&in){
        Json::Reader reader;
        if(in.is_open()&&reader.parse(in,Csman)){
            return true;
        }else return false;
    }
    bool write(std::ofstream&out)const{
        Json::StyledWriter Sw;
        if(out.is_open()){out<<Sw.write(Csman);return true;}
        else return false;
    }
    bool file_change(const std::string &path,const std::pair<std::string,std::string>&ver){
        std::ifstream ifs;
        ifs.open(path);
        if(!read(ifs)){ifs.close();return false;}
        if(Csman.isMember(ver.first))Csman[ver.first]=ver.second;//has this member
        else Bri::out("Wrong Verion Infomation Input");
        std::ofstream ofs;
        ofs.open(path);
        if(!write(ofs)){ofs.close();return false;}
        return true;
    }
};
std::ostream& operator<<(std::ostream&os,const CsmanJson&CJ){
    Json::StyledWriter sw;
    os<<sw.write(CJ.Csman);
    return os;
}
std::ofstream& operator<<(std::ofstream&ofs,const CsmanJson&CJ){
    CJ.write(ofs);
    return ofs;
}
std::ifstream& operator>>(std::ifstream&ifs,CsmanJson&CJ){
    CJ.read(ifs);
    return ifs;
}
