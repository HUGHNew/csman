#pragma once
#include"op.cpp"
#include<string>
#include<json/json.h>
bool install(Json::Value&,const std::string&);
bool upgrade(Json::Value&,const std::string&);
