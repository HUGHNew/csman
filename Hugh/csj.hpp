#pragma once
/*
 *Author by HUGHNew
 *Version 2.0
 */
#include<string>
#include<unordered_map>
#include<map>
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
    //template<typename Key,typename Value>using mp=std::make_pair<Key&&,Value&&>;
    template<typename Key,typename Value>using um=std::unordered_map<Key,Value>;
}
class CsmanJson{
private:
    Json::Value Csman;
public:
    friend std::ostream& operator<<(std::ostream&,const CsmanJson&);
    friend std::ofstream& operator<<(std::ofstream&,const CsmanJson&);
    friend std::ifstream& operator>>(std::ifstream&,CsmanJson&);
    bool operator==(const Json::Value&);
    bool operator==(const CsmanJson&CJ);
    CsmanJson& operator=(const Json::Value&);
    CsmanJson& operator=(const CsmanJson&);
    CsmanJson()=default;
    CsmanJson(const Json::Value&root):Csman(root){}
    CsmanJson(const std::string&path);
    CsmanJson(const char*);
    void upgrade(const std::unordered_map<std::string,std::string>&ver);
    void upgrade(const std::map<std::string,std::string>&ver);
    bool reboot(std::ifstream&);//reload the Csman
    bool reboot(const CsmanJson&);
    bool reboot(const std::string&);
    bool reboot(const char*);
    bool reboot(const Json::Value&);
    bool reboot(const std::string&,const std::unordered_map<std::string,std::string>&);
    bool reboot(const std::string&,const std::map<std::string,std::string>&);
    bool reboot(std::ifstream&,const std::map<std::string,std::string>&);
    bool reboot(std::ifstream&,const std::unordered_map<std::string,std::string>&);
    void clear(){Csman.clear();}//remove the objects from Csman
    bool empty()const{//check whether the Csman is empty
        return Csman.isNull()||Csman.empty();
    }
    bool read(std::ifstream&in){//fetch the json from file
        Json::Reader reader;
        if(in.is_open()&&reader.parse(in,Csman)){
            return true;
        }else return false;
    }
    bool write(std::ofstream&out)const{//output the json-file
        Json::StyledWriter Sw;
        if(out.is_open()&&(!empty())){out<<Sw.write(Csman);return true;}
        else return false;
    }
    bool file_change(const std::string &path,const std::pair<std::string,std::string>&ver);//straightly change the file
private:
    bool PairChange(const std::pair<std::string,std::string>&p){//the function for change the Version Pair
        if(Csman.isMember(p.first)){Csman[p.first]=p.second;return true;}
        return false;
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
bool CsmanJson::operator==(const Json::Value&JV){
    return JV==Csman;
}
bool CsmanJson::operator==(const CsmanJson&CJ){
    return CJ.Csman==Csman;
}
CsmanJson& CsmanJson::operator=(const Json::Value&JV){
    Csman=JV;
    return *this;
}
CsmanJson& CsmanJson::operator=(const CsmanJson&CJ){
    Csman=CJ.Csman;
    return *this;
}
CsmanJson::CsmanJson(const std::string&path){
    std::ifstream ifs;
    ifs.open(path);
    if(ifs)read(ifs);
    ifs.close();
}
CsmanJson::CsmanJson(const char*path){
    std::string Path=path;
    std::ifstream ifs(Path);
    if(ifs)read(ifs);
    ifs.close();
}
bool CsmanJson::reboot(const CsmanJson&CJ){
    Csman=CJ.Csman;
    return 1;
}
bool CsmanJson::reboot(const Json::Value&JV){
    Csman=JV;
    return 1;
}
bool CsmanJson::reboot(const std::string& file,const std::map<std::string,std::string>&ver){
    Csman.clear();
    std::ifstream ifs;
    ifs.open(file);
    if(!read(ifs)){ifs.close();return false;}
    for(auto v:ver){
        if(!PairChange(v))Bri::out("Wrong pair exists");
    }
    ifs.close();
    return true;
}
bool CsmanJson::reboot(const std::string& file,const std::unordered_map<std::string,std::string>&ver){
    Csman.clear();
    std::ifstream ifs;
    ifs.open(file);
    if(!read(ifs)){ifs.close();return false;}
    for(auto v:ver){
        if(!PairChange(v))Bri::out("Wrong pair exists");
    }
    ifs.close();
    return true;
}
bool CsmanJson::reboot(const char*c){
    return reboot(std::string(c));
}
bool CsmanJson::reboot(const std::string& file){
    Csman.clear();
    std::ifstream ifs;
    ifs.open(file);
    if(!read(ifs)){ifs.close();return false;}
    ifs.close();
    return true;
}
bool CsmanJson::reboot(std::ifstream&ifs){
    Csman.clear();
    if(!read(ifs))return false;
    return true;
}
bool CsmanJson::reboot(std::ifstream&ifs,const std::map<std::string,std::string>&ver){
    Csman.clear();
    if(!read(ifs))return false;
    for(auto v:ver){
        if(!PairChange(v))Bri::out("Wrong pair exists");
    }
    return true;
}
bool CsmanJson::reboot(std::ifstream&ifs,const std::unordered_map<std::string,std::string>&ver){
    Csman.clear();
    if(!read(ifs))return false;
    for(auto v:ver){
        if(!PairChange(v))Bri::out("Wrong pair exists");
    }
    return true;
}
bool CsmanJson::file_change(const std::string &path,const std::pair<std::string,std::string>&ver){
        CsmanJson C_CJ(path);
        if(C_CJ.empty())return false;
        if(!PairChange(ver))Bri::out("Wrong pair exists");
        std::ofstream ofs;
        ofs.open(path);
        if(!write(ofs)){ofs.close();return false;}
        ofs.close();
        return true;
}
void CsmanJson::upgrade(const std::unordered_map<std::string,std::string>&ver){
    for(auto v:ver){
        if(!PairChange(v))Bri::out("Wrong pair exists");
    }
}
void CsmanJson::upgrade(const std::map<std::string,std::string>&ver){
    for(auto v:ver){
        if(!PairChange(v))Bri::out("Wrong pair exists");
    }
}
