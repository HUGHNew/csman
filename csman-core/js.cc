#include<iostream>
#include<json/json.h>
#include<fstream>
#include<string>
#include"op.hpp"
int main(int argc,char**argv){
	using String=std::string;
	Json::Reader reader;
	Json::Value root;
	std::ifstream ifs(argv[1]);
	if(!reader.parse(ifs,root)){
	std::cout<<"Error,fail to open file:"<<argv[1]<<std::endl;
	ifs.close();	
	return -1;
	}
	ifs.close();	
	std::ofstream ofs;
	Json::StyledWriter sw;
	ofs.open(argv[1]);
	if(ofs){
	ofs<<sw.write(root)<<std::endl;
	}else{std::cout<<"Error,fail to open file:"<<argv[1]<<std::endl;}
	ofs.close();
	return 0;
}
