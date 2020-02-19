#pragma once
#include"op.cpp"
#include<string>
#include<json/json.h>
#include<map>
//bool install(CsJson::Value&,const std::string&);
//bool upgrade(CsJson::Value&,const std::string&);
class CsJs{
protected:
	std::string Latest;
	std::string Nightly;
public:
	void LUpgrade(std::string NewLVer){Latest=NewLVer;}
	void NUpgrade(std::string NewNVer){Nightly=NewNVer;}
	CsJs(std::string Lat,std::string Night):Latest(Lat),Nightly(Night){}
	CsJs() =default;
	std::string Late(){return Latest;}
	std::string Night(){return Nightly;}
};
class CsRoot{
private:
	std::string Version;
	std::string BaseUrl;
	std::string Platform;
public:
	CsRoot(std::string Ver,std::string BUl,std::string Plat):Version(Ver),BaseUrl(BUl),Platform(Plat){}
	std::string Ver(){return Version;}
	std::string Url(){return BaseUrl;}
	std::string Plat(){return Platform;}
};
class CsPlat:public CsJs{
private:
	std::map<std::string,std::string>Version;
public:
	CsPlat(const std::string &VER,const std::string &STD,const std::string &ABI,std::string Lat,std::string Night):CsJs(Lat,Night){
	Version.insert(std::make_pair<std::string,std::string>(std::string("VER"),std::string(VER)));
	Version.insert(std::make_pair<std::string,std::string>(std::string("STD"),std::string(STD)));
	Version.insert(std::make_pair<std::string,std::string>(std::string("ABI"),std::string(ABI)));
	}
	CsPlat(std::string Lat,std::string Nig,const std::map<std::string,std::string>&v):CsJs(Lat,Nig),Version(v){}
	void upgrade(const std::map<std::string,std::string>&Ver){
	for(auto v:Ver){
		if(v.first==std::string("Latest")){LUpgrade(v.second);}
		else if(v.first==std::string("Nightly")){NUpgrade(v.second);}
		else{
			try{Version.at(v.first);
			Version[v.first]=v.second;}
			catch(std::out_of_range)
				{std::cout<<"Version Error:Some null version info was added"<<std::endl;}
			}
		}
	}
	std::string Ver(const std::string&Ver){
	try{Version.at(Ver);}
	catch(std::out_of_range){return "";}
	return Version[Ver];
	}
};
class CsPack:public CsJs{
private:
	std::string Description;
	std::string Author;
	std::string Version;
public:
	CsPack(std::string Lat,std::string Night,std::string Desc,std::string Auth,std::string Ver):CsJs(Lat,Night),Description(Desc),Author(Auth),Version(Ver){}
	std::string Desc(){return Description;}
	std::string Auth(){return Author;}
	std::string Ver(){return Version;}
	void upgrade(std::map<std::string,std::string>&Info){
		if(!Info[std::string("Description")].empty())
			Description=Info[std::string("Description")];
		if(!Info[std::string("Author")].empty())
			Author=Info[std::string("Author")];
		if(!Info[std::string("Version")].empty())
			Version=Info[std::string("Version")];
		if(!Info[std::string("Latest")].empty())
			Latest=Info[std::string("Latest")];
		if(!Info[std::string("Nightly")].empty())
			Nightly=Info[std::string("Nightly")];
	}
};
class CsRT{
private:
	std::map<std::string,std::string>Ver;
public:
	CsRT(std::map<std::string,std::string>v):Ver(v){}
	void upgrade(const std::map<std::string,std::string>&v){
		for(auto vi:v){
			try{Ver.at(vi.first);Ver[vi.first]=vi.second;}
			catch(std::out_of_range){std::cout<<"ERROR!Some needless infomation was passed!"<<std::endl;}
		}
	}
};
