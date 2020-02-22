#include"json.hpp"
using std::string;
void test(){
    CsmanJson test;
    if(test.file_change("csman.json",Bri::Mp<std::string,std::string>("Version","1.0.0.1")))Bri::out(test,std::cout,"");
    CsmanJson test1(test);
    if(test1.Null())Bri::out("Null");
    CsmanJson test2;
    std::ifstream ifs;
    ifs.open("csman.json");
    ifs>>test2;
    if(!test2.Null())Bri::out(test2);
    ifs.close();
}
int main(){
    test();
    // Json::Value JV;
    // JV["1"]=4;
    // Bri::out(JV.isMember("2"));
/*     Json::Value JV;
    Bri::out(JV.isNull());
    JV["1"]=4;
    //Bri::out(JV[1]);
    Bri::out(JV.isMember("1")); */
}
