/*
 *Author by Hugh
 *Version 4.2
 */
#ifndef _CsMAN_JSON_HPP_
#define _CsMAN_JSON_HPP_
#include<json/json.h>
//#include"csman_config_file.hpp"
#include<fstream>
#include<iostream>
namespace csman_config
{
    /**
     * @brief to store a temporary JSON
     * You can use ifstream to initialize it
     * You can also get url and dependencies from it
    */
    class JsonStorage{
        Json::Value Cache;
    public:
        /**
         * friend part
        */
        friend class JsonOp;
        friend class JsonCombination;
        friend class JsonDeal;
        friend std::ostream& operator<<(std::ostream&,const JsonStorage&);
        JsonStorage()noexcept:Cache(Json::nullValue){}
        JsonStorage(const Json::Value&jv)noexcept:Cache(jv){}
        JsonStorage(std::ifstream&ifs)noexcept;
        //JsonStorage(const std::string&file)noexcept;
        JsonStorage(const JsonStorage&jsl)noexcept:Cache(jsl.Cache){}
        JsonStorage(JsonStorage&&jsr)noexcept{swap(jsr);}
        JsonStorage& operator=(const JsonStorage&jsl)noexcept{Cache=jsl.Cache;return *this;}
        JsonStorage& operator=(JsonStorage&&jsr)noexcept{jsr.swap(*this);}
        /**
         * @brief get the first object's Url
         * @warning only for new package online!!!
        */
        std::string Url()noexcept;
        /**
         * @brief get the first object's Dependencies
         * @warning only for new package online!!!
        */
        std::vector<std::string> Denpendencies()noexcept;
        bool operator==(const JsonStorage&jse)noexcept{return Cache==jse.Cache;}
        void swap(JsonStorage&js)noexcept{Cache.swap(js.Cache);}
        inline operator bool(){return !(Cache.empty()||Cache.isNull());}
        inline bool empty(){return Cache.empty()||Cache.isNull();}
        void show(){
            std::cout<<Cache<<std::endl;
        }
        /**
         * @brief output the JSON using ostream
        */
        virtual void output(std::ostream&os=std::cout)noexcept{os<<Cache;}
        virtual ~JsonStorage(){}
    };
    std::ostream& operator<<(std::ostream&,const JsonStorage&);

    /**
     * @brief combine two JSON
    */
    class JsonCombination final{
        JsonStorage dest,src;
    public:
        JsonCombination()=delete;
        /**
         * @brief the initializer
         * @param _des the needed JSON
         * @param _src the JSON for some objects
        */
        JsonCombination(const JsonStorage&_des,const JsonStorage&_src)noexcept:dest(_des),src(_src){}
        /**
         * @brief to combine the two file to dest
        */
        const JsonStorage& operator()()noexcept;
        /**
         * @brief output the JSON
        */
        void output(std::ostream&os)noexcept{os<<dest;}
        void output(std::ofstream&ofs)noexcept{ofs<<dest;}
    };
    class JsonOp:virtual public JsonStorage{
    protected:
        std::string Path;
        std::string File;
    public:
        JsonOp()=default;
        JsonOp(const Json::Value&jsJV)noexcept:JsonStorage(jsJV){}
        JsonOp(const JsonOp&JO)noexcept:JsonStorage(JO.Cache),Path(JO.Path),File(JO.File){}
        virtual ~JsonOp(){};
    };
    class JsonBoolOpSign{
    public:
        JsonBoolOpSign()=default;
        virtual bool operator()(const std::string&Name)=0;
    };


    class JsonDeal{
        JsonStorage JS;
    public:
        static constexpr const char* LocalName="info.json";
        enum class MODE:short{DEL=0,ADD=1,SEARCH=4};
        enum class VerInner:short{Url=1,Dependencies=3};
        /**
         * @brief using LocalName for default initialization
        */
        JsonDeal()noexcept{std::ifstream ifs(LocalName);ifs>>JS.Cache;}
        /**
         * @brief using JsonStorage for  initialization
        */
        JsonDeal(const JsonStorage&Js)noexcept:JS(Js){}
        /**
         * @brief copy constructor
        */
        JsonDeal(const JsonDeal&)noexcept;
        /**
         * @brief move constructor
        */
        JsonDeal(JsonDeal&&)noexcept;
        JsonDeal(std::ifstream&ifs)noexcept:JS(ifs){}
        JsonDeal& operator=(const JsonDeal&)noexcept;
        JsonDeal& operator=(JsonDeal&&)noexcept;
        bool operator==(const JsonDeal&)noexcept;
        /**
         * @brief Delete package
         * all existing versions
         * @param name the name for operatored object
        */
        bool Del(const std::string&name)noexcept;
        /**
         * @brief Delete specific version of the package
         * do nothing if not exist
         * @param name the name for operatored object
         * @param version the version of specific one to operator exactly
        */
        bool Del(const std::string&name,const std::string&version)noexcept;
        /**
         * @brief Get the JSON and do some jobs
         * available for MODE::SEARCH and MODE::DEL
         * @param name the name for operatored object
         * @param mod the mode option
         * @param version the version of specific one to operator exactly
        */
        bool operator()(const std::string&name,MODE mod=MODE::SEARCH,const std::string&version="");

        void Add(JsonStorage&)noexcept;
        bool Search(const std::string&name)noexcept;
        bool Search(const std::string&name,const std::string&version)noexcept;
        /**
         * @brief run for Dependencies and Url
         * Don't use!
        */
        std::vector<std::string> InnerOp(const std::string&name,VerInner mod,const std::string&version="")noexcept;
        /**
         * @brief get the newer url of pack (maybe)
        */
        std::string Url(const std::string&name)noexcept;
        std::string Url(const std::string&name,const std::string&version)noexcept;
        /**
         * @brief get the newer Dependencies of pack (maybe) if not identify the exact version
        */
        std::vector<std::string> Dependencies(const std::string&name,const std::string&version="")noexcept{return InnerOp(name,VerInner::Dependencies,version);}
        /**
         * @brief get all version of a name
        */
        std::vector<std::string> GetVersion(const std::string&name)noexcept;
        /**
         * @brief get all version of runtime
        */
        std::vector<std::string> GetRuntime()noexcept;
        /**
         * @brief get all package names of JSON
        */
        std::vector<std::string> GetNames()noexcept;
        inline operator bool()noexcept{return JS.operator bool();}
        inline bool empty()noexcept{return JS.empty();}
        inline void swap(JsonDeal&)noexcept;
        void output(std::ostream&os)noexcept{os<<JS;}
        void show()noexcept{std::cout<<JS.Cache<<std::endl;}
        ~JsonDeal(){}
        friend std::ostream& operator<<(std::ostream&os,const JsonDeal&);
    };
    std::ostream& operator<<(std::ostream&os,const JsonDeal&);









//---------------implement part---------

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
    }
    std::string JsonDeal::Url(const std::string&name,const std::string&version)noexcept{
        return *(InnerOp(name,VerInner::Url,version).begin());
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
#endif
