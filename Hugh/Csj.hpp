/*
 *Author by Hugh
 *Version 3.0
 */
#ifndef CSMAN_JSONCPP_DEAL_HPP
#define CSMAN_JSONCPP_DEAL_HPP
#include<iostream>
#include<json/json.h>
#include<fstream>
#include<string>
#include<exception>
namespace Csj{
    #define UnDefinedFunction
    #define CsmanJsonCpp
    #define SomethingWrong
    /**
     * designed for Package
     * for throwing wrong path
    */

    class LogicError:public std::exception{
        std::string Error;
    public:
        LogicError()=default;
        LogicError(const std::string&pr):Error(pr){}
        LogicError(const char*pr):Error(pr){}
        ~LogicError()=default;
        LogicError& operator=(const LogicError& pr);
        const char*what();
    };
	class CsmanJsonCpp Package{
		private:
            Json::Value Pack;//storage the info of the whole json
            //std::unique_ptr<Json::Value>revise=nullptr;
            Json::Value* revise=nullptr;// to change the value for the Pack
            bool inner=0;
            /**
             * \brief locate the upper directory of the path
             * \param AbsoluteMethod if AbsoluteMethod is on search as the absoluteMethod
             *  deal as relative otherwise
             *
             */
            Json::Value& UpperFind(const std::string&path,bool AbsoluteMethod=false){
                auto pos=revise;//keep oriented pointer
                revise=AbsoluteMethod?&Pack:revise;//change the pointer depends on the Method
                if(revise==nullptr){//protected way to avoid nullptr->operator*
                    revise=&Pack;//avoid nullptr
                    inner=true;//if revise isn't nullptr, becomes inner
                }
                if(path.size()==1){if(AbsoluteMethod)revise=&Pack;return *revise;}//root path Safe return
                if(path.find('/')==std::string::npos){//a simple path
                    if(!revise->isMember(path))//Not Exists
                        throw LogicError("The Key "+path+" Not Exists");//throw an exception
                    return AbsoluteMethod?Pack:(*revise);//return the higher dir
                }
                std::string p;//to keep every path
                for(auto point=path.begin();point!=path.end();++point){//deal every path but the last
                    if(*point=='/'){//a path ends
                        if(!revise->isMember(p)){revise=(pos==nullptr?&Pack:pos);throw LogicError("The Inner Key "+p+" Not Exists");}//throw error path
                        revise=&(*revise)[p];//Enter deeper path
                        p.clear();//clear the passing path
                        continue;//end this loop
                    }
                    p+=*point;//get every char of a path
                }
                auto temp=revise;//keep changed REVISE or the target
                if(pos!=nullptr)revise=pos;//if not nullptr keep the oriented value
                else revise=&Pack;//avoid nullptr while inner is on
                return *temp;//return the target
            }
            /** \param AbsoluteMethod if AbsoluteMethod is on search as the absoluteMethod
             *  deal as relative otherwise
             *  \param Revise if Revise is on ,it will change the private pointer
             *  default as no change
             */
            bool find(const std::string&path,bool Revise=false,bool AbsoluteMethod=false){//path is a directory
                auto Beg=revise;//keep oriented pointer
                Json::Value& pos=UpperFind(path,AbsoluteMethod);//get the upper dir
                //revise=&pos;//change to search the deepest path
                if(path.find('/')==std::string::npos){//raw path
                    if(Revise){//will change pointer
                        if(pos.isMember(path)){//path Exists
                            inner=true;
                            revise=&pos[path];
                            return true;
                        }//change REVISE
                        else throw LogicError("The Inner Key "+path+" Not Exists");}//throw NOT EXISTS path
                    revise=(Beg==nullptr?&Pack:Beg);//keep the revise unchanged if not nullptr
                    return pos.isMember(path);
                }
                const std::string& p=path.substr(path.rfind('/',path.size()-1)+1,path.size()-1);//get the deepest path
                if(!pos.isMember(p)){//path Not Exists
                    revise=(Beg==nullptr?&Pack:Beg);//keep the revise unchanged if not nullptr
                    throw LogicError("The Inner Key "+p+" Not Exists");
                }
                //if((*revise)[p].isObject())revise=&pos[p];
                // else if((*revise)[p].isArray())throw LogicError(p+" is an Array, can't get the value of an Array");
                //else throw LogicError("The Inner Key "+p+" isn't Object or Array!");
                if(Revise/* &(*revise!=Pack) */){
                    inner=true;//make sure inner is on
                    revise=&pos[p];//change the pointer;
                }
                if(!Revise)revise=(Beg==nullptr?&Pack:Beg);//keep the revise unchanged if not nullptr
                return true;
           }
		public:
            Package(){}
            Package(const Package&p):Pack(p.Pack),revise(p.revise),inner(p.inner){}
            Package(const Json::Value&JV):Pack(JV){}
            ~Package()=default;
            bool ShowInner(){return inner;}
            bool changePoint(const std::string&path);
            /**
             * \brief check the str is the member or not
             * \param AbsoluteMethod if AbsoluteMethod is on search as the absoluteMethod
             *  default as relative otherwise
            */
            bool isMember(const std::string&,bool AbsoluteMethod=false)noexcept;
            /**
             * \brief get the Names of Array in current directory
            */
            const std::vector<std::string> GetArrayNames();
            /**
             * \brief get the Names of Member in current directory
            */
            auto GetMemberNames()->decltype(Pack.getMemberNames());
            /**
             * \brief if current pointer or Pack is an array
             * return the size
             * return -1 otherwise
            */
            [[deprecated]]int arraySize();

            [[deprecated]]Json::Value* LoopPos();
            void reset(){inner=0;revise=nullptr;}//back every time if you want to change an object
            /**
             *  \brief
             * return object the inner pointer points
             * main utility is to show the pointer's content
            */
            const Json::Value& Inner(){
                return inner?*revise:Pack;
            }
            /**
             * \brief access the inner JSON
             * \param path the successive Key to get
            */
            Package& Enter(const std::string&path);
            /*
            *IO part
            * get json and output json
            *
            */
            friend std::istream& operator>>(std::istream&,Package &);
            friend std::ostream& operator<<(std::ostream&,Package &);
            Csj::Package& read(const Csj::Package&);
            bool read(std::ifstream&);//read from ifstream to get a json value
			bool write(std::ostream&ofs=std::cout,const Json::Value&out=Json::Value::null)noexcept;//output a ofstream to make a json
            Json::Value& operator[](const std::string&);
            Json::Value& operator[](const char*);
            Package& operator=(const Json::Value&);
			Package& operator=(const Package&);
            Package& operator=(Json::Value*);
            operator bool()noexcept;
            bool empty()noexcept;

            /**
             * add part
             * array make
             * \brief to make an array DON'T USE CONTAINER which is not included in JSON
             * it can make an array or overwrite the EXISTING array
             * \param ValueName the relative route
             * \param index the first position for the function to add
            */
            template<typename type>
            bool ArrayMake(const std::string&ValueName,int index,const type&);
            template<typename type,typename... T>
            bool ArrayMake(const std::string&ValueName,int index,const type&,const T&...tup);
            /**
             * \brief for array erase
             * delete the index
             * \param index remove the specific index
            */
            [[deprecated]]Package& UnDefinedFunction erase (int index);
            /**
             * \brief for other type
             * \param key remove the key and value
            */
            Package& erase(const std::string&Key,bool AbsoluteMethod=false);
    };
std::istream& operator>>(std::istream&is,Package &pack);
std::ostream& operator<<(std::ostream&os,Package &pack);
	template<typename type>
    std::ostream& out(std::ostream&os,type f);
    template<typename type,typename... params>
    std::ostream& out(std::ostream&os,type f,params... param);
    template<typename type>
    std::ostream& out(type f);
    template<typename type,typename... params>
    std::ostream& out(type f,params... param);
}
const char* Csj::LogicError::what(){
    return Error.c_str();
}
Csj::LogicError& Csj::LogicError::operator=(const LogicError& pr){
    Error=pr.Error;
    return *this;
}
std::istream& Csj::operator>>(std::istream&is,Csj::Package &pack){
    is>>pack.Pack;
    return is;
}
std::ostream& Csj::operator<<(std::ostream&os,Csj::Package &pack){
    os<<pack.Pack;
    return os;
}
Json::Value& Csj::Package::operator[](const std::string&Key){
    try{
            if(inner)return (*revise)[Key];
        else{
            if(Pack.isMember(Key)){
                inner=true;
                revise=&Pack[Key];
            }
            return Pack[Key];
        }
    }catch(const Json::LogicError&LE){throw LogicError(LE.what());}
}
Json::Value& Csj::Package::operator[](const char*Key){
    return this->operator[](std::string(Key));
}
Csj::Package& Csj::Package::operator=(const Json::Value&value){
    if(inner)(*revise)=value;
    else Pack=value;
    return *this;
}
Csj::Package& Csj::Package::operator=(const Csj::Package&value){
    return *this=value;
}
Csj::Package& Csj::Package::operator=(Json::Value*back){
    revise=back;
    inner=(back==nullptr||back==&Pack?0:1);
    return *this;
}
int Csj::Package::arraySize(){
    if((inner?*revise:Pack).isArray())return (inner?*revise:Pack).size();
    return -1;
}
const std::vector<std::string> Csj::Package::GetArrayNames(){
    std::vector<std::string>Array;
    auto&&cur=(inner?*revise:Pack);
    for(auto&&name:GetMemberNames()){
        if(cur[name].isArray())
            Array.push_back(name);
    }
    return Array;
}
auto Csj::Package::GetMemberNames()->decltype(Pack.getMemberNames()){
    Json::Value::Members Member;
    try{Member=(inner?*revise:Pack).getMemberNames();}
    catch(const Json::LogicError&JLE){throw LogicError(JLE.what());}
    return Member;
}
bool Csj::Package::isMember(const std::string&Key,bool AbsoluteMethod)noexcept{
    return (AbsoluteMethod?Pack:(inner?*revise:Pack)).isMember(Key);
}
Csj::Package::operator bool()noexcept{
    return !(Pack.isNull()||Pack.empty());
}
bool Csj::Package::empty()noexcept{
    return Pack.isNull()||Pack.empty();
}
Csj::Package& Csj::Package::read(const Csj::Package&other){
    Pack=other.Pack;
    revise=other.revise;
    inner=other.inner;
    return *this;
}
bool Csj::Package::read(std::ifstream&ifs){
    if(ifs){
        ifs>>Pack;
        reset();
    }
    else return false;
    return true;
}
bool Csj::Package::write(std::ostream&ofs,const Json::Value&out)noexcept{
    if(ofs)
        ofs<<(out==Json::Value::null?Pack:out);
    else return false;
    return true;
}
template<typename type>
bool Csj::Package::ArrayMake(const std::string&ValueName,int index,const type&f){
    auto& arr=inner?*revise:Pack;
    try{
        if(find(ValueName))
            {if(!arr[ValueName].isArray())
                return false;}
    }
    catch(const LogicError&){}
    arr[ValueName][index]=Json::Value(f);
    return true;
}
template<typename type,typename... T>
bool Csj::Package::ArrayMake(const std::string&ValueName,int index,const type&f,const T&...tup){
    auto& arr=inner?*revise:Pack;
    try{
        if(find(ValueName))
            {if(!arr[ValueName].isArray())
                return false;}
    }
    catch(const LogicError&){}
    arr[ValueName][index]=Json::Value(f);
    ArrayMake(ValueName,index+1,tup...);
    return true;
}
bool Csj::Package::changePoint(const std::string&path){
    find(path,true,true);
    return true;
}
Json::Value* Csj::Package::LoopPos(){
    if(revise!=nullptr)
        return revise;
    else{
        inner=0;
        return &Pack;
    }
}
Csj::Package& Csj::Package::erase(const std::string&Key,bool AbsoluteMethod){
    auto obj=(inner?*revise:Pack);
    std::string p;
    revise=(revise==nullptr?&Pack:revise);
    inner=true;
    //revise=&UpperFind(Key,AbsoluteMethod);
    if(Key.find('/')==std::string::npos)//simple path
        p=Key;//raw path
    else{
        revise=&UpperFind(Key);
        p=Key.substr(Key.rfind('/',Key.size()-1)+1,Key.size());
    }
    if(revise->isMember(p))
        revise->removeMember(p);
    if(AbsoluteMethod)revise=&obj;
    return *this;
}
Csj::Package& Csj::Package::Enter(const std::string&path){
    if(!find(path,true))Csj::out("Not in");
    return *this;
}
namespace Csj{
    template<typename type>
    std::ostream& out(std::ostream&os,type f){
        os<<f<<std::endl;
        return os;
    }
    template<typename type,typename... params>
    std::ostream& out(std::ostream&os,type f,params... param){
        os<<f<<std::ends;
        out(os,param...);
        return os;
    }
    template<typename type>
    std::ostream& out(type f){
        std::cout<<f<<std::endl;
        return std::cout;
    }
    template<typename type,typename... params>
    std::ostream& out(type f,params... param){
        std::cout<<f<<std::ends;
        out(std::cout,param...);
        return std::cout;
    }
}
#endif
