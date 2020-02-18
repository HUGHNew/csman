#include<json/json.h>
bool install(Json::Value&js,const std::string&str){
	if(str<js["Version"].asString())return 0;
	js["Version"]=str;
	return 1;
}
bool upgrade(Json::Value&js,const std::string&str){
	
	if(str<js["Version"].asString())return 0;
	js["Version"]=str;
	return 1;
}
