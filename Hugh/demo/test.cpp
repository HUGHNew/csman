#include <json/json.h>
#include<iostream>
#include"Csj.hpp"
#include"csman_config_file.hpp"
template<typename T>
void o(T&&out){
    std::cout<<out<<std::endl;
}
void v(){
    /* csman_config::JsonCombination com(ifs2,ifs);
    ifs.close();ifs2.close();
    o(com()); */
    /* Json::Value t1;
    if(ifs)
        ifs>>t1;o(t1); */


    //csman_config::JsonCombination com(ifs2,ifs);;
    /* std::ofstream ofs(dest);
    ofs<<com();
    ofs.close(); */
    // ifs.open(dest);
    // csman_config::JsonDeal Tes(ifs);
    // ifs.close();
    // o(Tes.Search("cics.csbuild"));
}
void test_storage(){
    std::string src="test.json",dest="dest.json";
    std::ifstream ifs(src);
    std::ofstream ofs(dest,std::ios::app);
    /* Json::Value t;
    ifs>>t; */
    csman_config::JsonStorage pack1(ifs);
    pack1.output(ofs);
    //ofs<<pack1;
    ifs.close();ofs.close();
}
void test_combination(){
    std::string src="test.json",dest="dest.json";
    std::ifstream ifs(src),ifs2(dest);
    csman_config::JsonCombination com(ifs2,ifs);
    ifs.close();ifs2.close();
    std::ofstream ofs(dest);
    ofs<<com();
    ofs.close();
}
void test(){
    std::string src="test.json",dest="Generic.json";
    std::ifstream ifs(dest),is(src);
    csman_config::JsonDeal test_deal(is);
    //csman_config::JsonStorage ad(ifs);
    //csman_config::JsonStorage St_a(ifs);
    //test_deal.Del("cics.cs");
    //std::cout<<test_deal.Search("cs.runtime","3.3.3.8")<<std::endl;
    //test_deal.Add(ad);
    for(auto&& it:test_deal.Dependencies("cics.cs")){
        std::cout<<it<<std::endl;
    }
    for(auto&& it:test_deal.GetRuntime()){
        std::cout<<it<<std::endl;
    }
    for(auto&& it:test_deal.GetVersion("cs.runtime")){
        std::cout<<it<<std::endl;
    }
    for(auto&& it:csman_config::JsonDeal(ifs).GetNames()){
        std::cout<<it<<std::endl;
    }
    csman_config::JsonDeal b(test_deal);
    std::cout<<b.Url("cs.runtime")<<std::endl<<b("cs.runtime",csman_config::JsonDeal::MODE::SEARCH,"3.3.3.8")<<std::endl;
    ifs.close();is.close();
    std::ofstream ofs(src);
    test_deal.output(ofs);
}
void Segment(){
    std::string src="test.json",dest="dest.json";
    std::ifstream is(src);
    Json::Value test_deal;
    is>>test_deal;
    for(auto a:test_deal["Generic"]["cics.csbuild"]["Version"].getMemberNames())
    std::cout<<a<<std::endl;
    is.close();
}
void JV(){
    std::string src="test.json";
    std::ifstream ifs(src);
    Json::Value a;
    ifs>>a;
    //std::cout<<a;
    Json::Value b=a;
    std::cout<<b;
    /* ifs>>a;
    std::cout<<a["Generic"]["cics.csbuild"]["Version"]["1.1.5"]["ContentUrl"].asString()<<std::endl;
    ifs.close(); */
    ifs.close();
}
// #include<string>
// #include<vector>
// typedef std::vector<size_t> Pos;
// Pos StrPos(const std::string&str,char _c){
//     Pos list;
//     for(size_t i=0;i<str.size();++i){
//         if(_c==str[i])
//             list.push_back(i);
//     }
//     return list;
// }
// int mul(int num,int time){
//     int n=num;
//     for(int i=0;i<time-1;++i){
//         num*=n;
//     }return num;
// }
// int VerSize(const std::string&str){
//     int size=0,scale;
//     Pos list;
//     char _c='.';
//     for(size_t i=0;i<str.size();++i){
//         if(_c==str[i])
//             list.push_back(i);
//     }
//     scale=list.size();
//     //std::cout<<scale<<std::endl;
//     size+=mul(100,scale)*std::stoi(str.substr(0,list[0]));
//     size+=std::stoi(str.substr(list[scale-1]+1));
//     if(scale>1){
//         for(int i=0;i<scale-1;++i){
//             size+=mul(100,scale-i-1)*std::stoi(str.substr(list[i]+1,list[i+1]-list[i]-1));
//         }
//     }return size;
// }
int main(){
    //JV();
    test();
    //Segment();
    return 0;
}
