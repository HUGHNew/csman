<h1>[[Deprecated]]</h1>
<p>头文件"Csj.hpp"</p>


|类||
|---|---|
|LogicError|指示所有的路径错误的异常类(本Hpp唯一的异常抛出可能)
|Package|保存JSON信息并且提供修改工具的类

常用参数:路径(path)
```json
{
    A:{
        B:{
            C:{
                D:1
            }
        }
    }
}
```
C 的绝对路径:`"A/B/C"`,相对于A路径(即当前在A，进入C `["B/C"]`)

|函数||
|---|---|
|changePoint|改变指针所指对象|
|GetArrayNames|返回数组成员的名字(`vector<string>`)
|GetMemberNames|返回成员名字(返回类型同上)
|reset|类内部指针置为空
|Inner|返回当前JSON对象,若无则返回整个JSON
|Enter|进入JSON对象
|read|从Package对象中读取JSON或者从 ifstream 中读取JSON
|write|将 JSON 输出到 os 对象,第二个参数可使用Inner()显示当前的JSON对象
|isMember|成员判断
|empty|为空判断
|ArrayMake(const std::string&ValueName,int index,const type&,const T&...tup)|添加数组从index开始插入元素
|erase|删除对象
|operator=|改值或进入内部对象
|operator[]|进入对象和制造对象
|friend|友元部分|
|operator<<|输出
|operator>>|输入

概要
```cpp
#include<iostream>
#include<json/json.h>
#include<fstream>
#include<string>
#include<exception>
namespace Csj{
    class Package;
    class LogicError;
}
```

类 Package
```cpp
namespace Csj{
	class Package{
		private:
            Json::Value Pack;//storage the info of the whole json
            Json::Value* revise=nullptr;// to change the value for the Pack
            bool inner=0;
            Json::Value& UpperFind(const std::string&path,bool AbsoluteMethod=false);
            /**
             * \param AbsoluteMethod if AbsoluteMethod is on search as the absoluteMethod
             *  deal as relative otherwise
             *  \param Revise if Revise is on ,it will change the private pointer
             *  default as no change
             */
            bool find(const std::string&path,bool Revise=false,bool AbsoluteMethod=false);
		public:
            Package(){}
            Package(const Package&p):Pack(p.Pack),revise(p.revise),inner(p.inner){}
            Package(const Json::Value&JV):Pack(JV){}
            ~Package()=default;
            bool ShowInner(){return inner;}
            bool changePoint(const std::string&path);
            bool isMember(const std::string&,bool AbsoluteMethod=false)noexcept;
            const std::vector<std::string> GetArrayNames();
            auto GetMemberNames()->decltype(Pack.getMemberNames());//返回值类型同上
            [[deprecated]]int arraySize();//useless function
            [[deprecated]]Json::Value* LoopPos();
            void reset(){inner=0;revise=nullptr;}//back every time if you want to change an object
            const Json::Value& Inner(){
                return inner?*revise:Pack;
            }
            Package& Enter(const std::string&path);
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
            template<typename type>
            bool ArrayMake(const std::string&ValueName,int index,const type&);
            template<typename type,typename... T>
            bool ArrayMake(const std::string&ValueName,int index,const type&,const T&...tup);
            [[deprecated]]Package& UnDefinedFunction erase (int index);//useless function
            Package& erase(const std::string&Key,bool AbsoluteMethod=false);//AbsoluteMethod is not settled. Don't turn AbsoluteMethod on
    };
}
std::istream& operator>>(std::istream&is,Package &pack);
std::ostream& operator<<(std::ostream&os,Package &pack);
```
类 LogicError
```cpp
namespace Csj{
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
}
```
