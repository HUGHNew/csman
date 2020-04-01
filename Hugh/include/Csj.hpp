#ifndef CsMAN_JSON_HPP
#define CsMAN_JSON_HPP
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
}
#endif
