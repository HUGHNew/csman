#include"Csj.hpp"
namespace csman_config{
    int mul(int num,int time){
        int n=num;
        for(int i=0;i<time-1;++i){
            num*=n;
        }return num;
    }
    int VerSize(const std::string&str){
        int size=0,scale;
        std::vector<int> list;
        char _c='.';
        for(size_t i=0;i<str.size();++i){
            if(_c==str[i])
                list.push_back(i);
        }
        scale=list.size();
        //std::cout<<scale<<std::endl;
        size+=mul(100,scale)*std::stoi(str.substr(0,list[0]));
        size+=std::stoi(str.substr(list[scale-1]+1));
        if(scale>1){
            for(int i=0;i<scale-1;++i){
                size+=mul(100,scale-i-1)*std::stoi(str.substr(list[i]+1,list[i+1]-list[i]-1));
            }
        }return size;
    }
    std::string MaxVer(const Json::Value::Members&VS){
        std::string f=VS[0];
        for(auto&&i:VS){
            if(VerSize(f)<VerSize(i)){
                f=i;
            }
        }return f;
    }
    JsonStorage::JsonStorage(std::ifstream&ifs)noexcept{
        try{
            if(ifs)
                ifs>>Cache;
        }catch(Json::RuntimeError&l){
            std::cout<<l.what()<<std::endl;
        }
    }
    // JsonStorage::JsonStorage(const std::string&file)noexcept{
    //     std::ifstream ifs(file);
    //     try{
    //         if(ifs)
    //             ifs>>JS.Cache;
    //     }catch(Json::RuntimeError&l){
    //         std::cout<<l.what()<<std::endl;
    //     }ifs.close();
    // }



    std::string JsonStorage::Url()noexcept{
        std::string Shell=Cache.getMemberNames().begin()->c_str();
        std::string name=Cache[Shell].getMemberNames().begin()->c_str();
        std::string version=Cache[Shell][name]["Version"].getMemberNames().begin()->c_str();
        return Cache[Shell][name]["Version"][version]["ContentUrl"].asString();
    }
    std::vector<std::string> JsonStorage::Denpendencies()noexcept{
        std::string Shell=Cache.getMemberNames().begin()->c_str();
        std::string name=Cache[Shell].getMemberNames().begin()->c_str();
        std::string version=Cache[Shell][name]["Version"].getMemberNames().begin()->c_str();
        return Cache[Shell][name]["Version"][version]["Dependencies"].getMemberNames();
    }
    bool JsonDeal::operator==(const JsonDeal&JsDl)noexcept{
        return JS.Cache==JsDl.JS.Cache;
    }
    void JsonDeal::swap(JsonDeal&JDRv)noexcept{
        JDRv.JS.Cache.swap(this->JS.Cache);
    }
    JsonDeal::JsonDeal(const JsonDeal&JDLv)noexcept:JS(JDLv.JS.Cache){}
    JsonDeal::JsonDeal(JsonDeal&&JDRv)noexcept{
        JDRv.swap(*this);
    }
    JsonDeal& JsonDeal::operator=(const JsonDeal&JD)noexcept{
        JS.Cache=JD.JS.Cache;
        return *this;
    }
    JsonDeal& JsonDeal::operator=(JsonDeal&&JDRv)noexcept{
        JDRv.swap(*this);
    }
    bool JsonDeal::operator()(const std::string&name,MODE mod,const std::string&version){
        if(!this->operator bool())
            return false;
        bool find=JS.Cache.isMember(name);
        if(find){if(mod==MODE::DEL)JS.Cache.removeMember(name);return find;}
        for(auto&& H:JS.Cache.getMemberNames()){
            find=JS.Cache[H].isMember(name);
            if(find){
                switch(mod){
                    case MODE::SEARCH:
                        if(version!="")find=JS.Cache[H][name]["Version"].isMember(version);
                        goto end;
                    case MODE::ADD:
                        return false;
                    case MODE::DEL:
                        if(version==""){ JS.Cache[H].removeMember(name);find=true;}
                        else{
                            auto&&Mover=JS.Cache[H][name]["Version"];
                            if(Mover.isMember(version))
                            Mover.removeMember(version);
                            find=true;
                        }
                        goto end;
                }
            }
        }end: return find;
    }
    bool JsonDeal::Search(const std::string&name)noexcept{
        return this->operator()(name);
    }
    bool JsonDeal::Search(const std::string&name,const std::string&version)noexcept{
        return this->operator()(name,MODE::SEARCH,version);
    }
    bool JsonDeal::Del(const std::string&name)noexcept{
        return this->operator()(name,MODE::DEL);
    }
    bool JsonDeal::Del(const std::string&name,const std::string&version)noexcept{
        return this->operator()(name,MODE::DEL,version);
    }
    void JsonDeal::Add(JsonStorage&js)noexcept{
        if(!this->operator bool())
            return;
        const std::string outer=js.Cache.getMemberNames().begin()->c_str();
        const std::string name=js.Cache[outer].getMemberNames().begin()->c_str();
        if(!JS.Cache.isMember(outer)){
            JS.Cache[outer]=js.Cache[outer];
        }else{
            if(JS.Cache[outer].isMember(name)){

        std::cout<<js.Cache[outer][name]<<std::endl;
                try{
                    auto&& InJs=JS.Cache[outer][name]["Version"];
                    auto&& JsVerJs=js.Cache[*(js.Cache.getMemberNames().begin())]["Version"];
                    std::string JsVerName=*(JsVerJs.getMemberNames().begin());
                    if(!InJs.isMember(JsVerName)){
                        InJs[JsVerName]=JsVerJs[JsVerName];
                    }
                }catch(Json::LogicError&l){
                    std::cout<<l.what()<<std::endl;
                }
            }
            else{
                auto&&name=js.Cache[outer].getMemberNames().begin()->c_str();
                auto&&Content=js.Cache[outer][name];
                JS.Cache[outer][name]=Content;
            }
        }
    }
    std::vector<std::string> JsonDeal::InnerOp(const std::string&name,VerInner mod,const std::string&version)noexcept{
        if(empty())
            return {};
        for(auto&& H:JS.Cache.getMemberNames()){
            if(JS.Cache[H].isMember(name)){
                switch(mod){
                    case VerInner::Url:
                    if(version!=""){
                        if(JS.Cache[H][name]["Version"].isMember(version)){
                            auto&&str=JS.Cache[H][name]["Version"][version]["ContentUrl"].asString();
                            return {str};
                        }
                    }
                    else{
                        auto&& inner=JS.Cache[H][name]["Version"];
                        auto&& VecStr=inner.getMemberNames();
                        return {inner[MaxVer(VecStr)]["ContentUrl"].asString()};
                    }break;
                case VerInner::Dependencies:
                    std::vector<std::string>depens;
                    if(version!=""){
                        if(JS.Cache[H][name]["Version"].isMember(version)){
                            depens=JS.Cache[H][name]["Version"][version]["Dependencies"].getMemberNames();
                            return depens;
                        }
                    }
                    else{
                        auto&& inner=JS.Cache[H][name]["Version"];
                        auto&& VecStr=inner.getMemberNames();
                        return inner[MaxVer(VecStr)]["Dependencies"].getMemberNames();
                    }
                }
            }
        }
        return {};
    }
    std::string JsonDeal::Url(const std::string&name)noexcept{
        return *(InnerOp(name,VerInner::Url).begin());
        /* std::string null="";
        if(!this->operator bool())
            return null;
        for(auto&& H:JS.Cache.getMemberNames()){
            if(JS.Cache[H].isMember(name)){
                auto&& inner=JS.Cache[H][name]["Version"];
                auto&& VecStr=inner.getMemberNames();
                char MaxNumber=(*VecStr.rbegin())[0];
                for(auto it=VecStr.begin();it!=VecStr.end();++it){
                    if((*it)[0]!=MaxNumber)
                        continue;
                    if((*it)<*(it+1)&&it->size()>(it+1)->size()){
                        auto&&str=inner[*it]["ContentUrl"].asString();
                        return str.substr(1,str.size()-2);
                    }
                }
            }
        }
        return null;*/
    }
    std::string JsonDeal::Url(const std::string&name,const std::string&version)noexcept{
        return *(InnerOp(name,VerInner::Url,version).begin());/*
        std::string null="";
        if(!this->operator bool())
            return null;
        for(auto&& H:JS.Cache.getMemberNames()){
            if(JS.Cache[H].isMember(name)){
                if(JS.Cache[H][name]["Version"].isMember(version)){
                    auto&&str=JS.Cache[H][name]["Version"][version]["ContentUrl"].asString();
                    return str.substr(1,str.size()-2);
                }
            }
        }
        return null; */
    }

    std::vector<std::string> JsonDeal::GetNames()noexcept{
        std::vector<std::string>all=JS.Cache[JS.Cache.getMemberNames()[0]].getMemberNames();
        for(auto&&it:JS.Cache.getMemberNames()){
            if(all!=JS.Cache[it].getMemberNames()){
                for(auto&&str:JS.Cache[it].getMemberNames())
                    all.push_back(str);
            }
        }return all;
    }
    std::vector<std::string> JsonDeal::GetVersion(const std::string&name)noexcept{
        if(empty())
            return {};
        for(auto&& H:JS.Cache.getMemberNames()){
            if(JS.Cache[H].isMember(name)){
                return JS.Cache[H][name]["Version"].getMemberNames();
            }
        }return {};
    }
    std::vector<std::string> JsonDeal::GetRuntime()noexcept{
        if(empty())
            return {};
        return JS.Cache["Generic"]["cs.runtime"]["Version"].getMemberNames();
    }
    std::ostream& operator<<(std::ostream&os,const JsonStorage&JSON){
        os<<JSON.Cache;
        return os;
    }
    std::ostream& operator<<(std::ostream&os,const JsonDeal&JDJson){
        os<<JDJson;
        return os;
    }
    const JsonStorage& JsonCombination::operator()()noexcept{
        const std::string shell=src.Cache.getMemberNames().begin()->c_str();
        if(!dest.Cache.isMember(shell)){
            dest.Cache[shell].swap(src.Cache[shell]);
        }else{
            std::string name=src.Cache[shell].getMemberNames().begin()->c_str();
            if(dest.Cache[shell].isMember(name)){
                std::string Ver=src.Cache[shell][name]["Version"].getMemberNames().begin()->c_str();
                if(!dest.Cache[shell][name]["Version"].isMember(Ver)){
                    dest.Cache[shell][name]["Version"][Ver]=src.Cache[shell][name]["Version"][Ver];
                }
            }
        }return dest;
    }
}
