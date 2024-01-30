#include "rapidjson.h"
#include<iostream>
#include<optional>
#include<vector>
#include<sstream>
#include "rapidjson/document.h"
#include "util.h"

namespace frc_api{

Decode_error::Decode_error(const char *s,const char *value1,std::string what):
	path({s}),value(value1),description(what)
{}

Decode_error::Decode_error(const char *s,JSON const& json,std::string what):
	path({s}),value(to_string(json)),description(move(what))
{}

Decode_error::Decode_error(std::vector<std::string> path1,JSON const& json,std::string what):
	path(move(path1)),value(to_string(json)),description(move(what))
{
	//could check that path is not empty...or make it so that the how path variable can't be empty.
}

std::ostream& operator<<(std::ostream& o,Decode_error const& a){
	/*o<<"Decode_error(";
	o<<"from:"<<a.value<<" ";
	o<<"path:"<<a.path<<" ";
	o<<"description:"<<a.description;
	return o<<")";*/
	o<<"Decode_error(\n\t";
	o<<"from:"<<a.value<<"\n\t";
	o<<"path:"<<a.path<<"\n\t";
	o<<"description:"<<a.description;
	return o<<"\n)";
}

std::ostream& operator<<(std::ostream& o,rapidjson::GenericValue<rapidjson::UTF8<>> const& a){
	if(a.IsInt()) return o<<a.GetInt();

	if(a.IsString()) return o<<"str("<<a.GetString()<<")";
	if(a.IsBool()) return o<<"bool("<<a.GetBool()<<")";
	if(a.IsNull()) return o<<"null";

	if(a.IsArray()){
		o<<"[ ";
		std::for_each(a.Begin(),a.End(),[&](auto const& x){ o<<x<<" "; });
		return o<<"]";
	}

	if(a.IsObject()){
		o<<"{ ";
		std::for_each(a.MemberBegin(),a.MemberEnd(),[&](auto const& x){
			o<<x.name<<":"<<x.value<<" ";
		});
		return o<<"}";
	}

	if(a.IsUint()) return o<<"uint("<<a.GetUint()<<")";
	if(a.IsInt64()) return o<<"int64";
	if(a.IsDouble()) return o<<a.GetDouble();

	if(a.IsNumber()){
		return o<<"(num)";
	}

	return o<<"(Unexpected type)";
}

std::string decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::string*){
	if(!in.IsString()){
		throw Decode_error{"string",in,"wrong type"};
	}
	return in.GetString();
}

bool decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const bool*){
	if(!in.IsBool()) throw Decode_error{"bool",in,"wrong type"};
	return in.GetBool();
}

std::nullptr_t decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::nullptr_t*){
	if(in.IsNull()) return nullptr;
	throw Decode_error{"null",in,"wrong type"};
}

int decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const int*){
	if(!in.IsInt()){
		std::ostringstream ss;
		ss<<"Expected int, got:"<<in<<" isnum:"<<in.IsNumber()<<" isint:"<<in.IsInt()<<" isunsigned:"<<in.IsUint()<<" IsDouble:"<<in.IsDouble();
		throw Decode_error{"int",in,ss.str()};
	}
	return in.GetInt();
}

unsigned decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const unsigned int*){
	if(!in.IsUint()){
		throw Decode_error{"unsigned",in,"wrong type"};
	}
	return in.GetUint();
}

int64_t decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const int64_t*){
	if(!in.IsInt64()){
		throw Decode_error{"int64_t",in,"wrong type"};
	}
	return in.GetInt64();
}

double decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const double*){
	if(in.IsDouble()){
		return in.GetDouble();
	}
	if(in.IsInt()){
		//implicitly do the conversion.
		return in.GetInt();
	}
	std::ostringstream ss;
	ss<<"num:"<<in.IsNumber()<<" int:"<<in.IsInt();
	throw Decode_error{"double",in,ss.str()};
}

}
