#include "data.h"
#include<iostream>
#include<cassert>
#include "util.h"
#include "rapidjson.h"

#define nyi FRC_API_NYI

namespace frc_api{

std::ostream& operator<<(std::ostream&,R0_2)nyi

R0_2::R0_2(int i1):i(i1){
	if(i<0 || i>2){
		throw std::invalid_argument("R0_2");
	}
}

R0_2 example(const R0_2*)nyi
R0_2 rand(const R0_2*)nyi

R0_2 decode(JSON const& in,const R0_2*){
	return R0_2{decode(in,(int*)0)};
}

bool alphanumeric(char c){
	return isalpha(c) || isdigit(c);
}

String2::String2(std::string a):s(move(a)){
	if(s.size()<2) throw std::invalid_argument("String2");
	if(!all_of(begin(a),end(a),alphanumeric)){
		throw std::invalid_argument{"String2"};
	}
}

std::string const& String2::get()const{ return s; }

bool operator==(String2 const& a,const char *b){
	return a.get()==b;
}

bool operator!=(String2 const& a,const char *b){
	return a.get()!=b;
}

String2 example(const String2*){
	nyi
}

char rand_char(){
	return 'a'+(::rand()%26);
}

String2 rand(const String2*){
	std::ostringstream ss;
	ss<<rand_char()<<rand_char();
	return String2{ss.str()};
}

std::ostream& operator<<(std::ostream& o,String2 const& a){
	return o<<a.get();
}

String2 decode(JSON const& in,const String2*){
	return String2{decode(in,(std::string*)nullptr)};
}

bool operator<(Team_number const& a,Team_number const& b){
	return a.get()<b.get();
}

Team_number decode(JSON const& in,const Team_number*){
	return Team_number{decode(in,(int*)nullptr)};
}

#define PRINT_ITEM(A) if(a==decltype(a)::A) return o<<""#A;

#define ITEM_LIST(A) NAME::A,

#define DECODE(A) if(s==""#A) return NAME::A;

#define STR(A) (""#A)

#define IMPL_ENUM(ITEMS)\
	std::ostream& operator<<(std::ostream& o,NAME a){\
		ITEMS(PRINT_ITEM)\
		assert(0);\
	}\
	NAME example(const NAME*){\
		std::vector<NAME> options{ITEMS(ITEM_LIST)};\
		return options.at(0);\
	}\
	NAME rand(const NAME*){\
		std::vector<NAME> options{ITEMS(ITEM_LIST)};\
		return options[::rand()%options.size()];\
	}\
	NAME decode(JSON const& in,const NAME* x){\
		auto s=decode(in,(std::string*)nullptr);\
		ITEMS(DECODE)\
		throw Decode_error{typeid(*x).name(),in,"unexpected value"};\
	}

#define NAME Tournament_level
IMPL_ENUM(FRC_API_TOURNAMENT_LEVEL)
#undef NAME

#define NAME Award_Event_type
IMPL_ENUM(FRC_API_AWARD_EVENT_TYPES)
#undef NAME

#define NAME Station
IMPL_ENUM(FRC_API_STATION)
#undef NAME

#define NAME Field
IMPL_ENUM(FRC_API_FIELD)
#undef NAME

#define NAME Event_Event_type
IMPL_ENUM(FRC_API_EVENT_EVENT_TYPES)
#undef NAME

#define NAME Alliance_color
IMPL_ENUM(FRC_API_ALLIANCE_COLOR)
#undef NAME

#define NAME Auto_2017
IMPL_ENUM(FRC_API_AUTO_2017)
#undef NAME

#define NAME Touchpad
IMPL_ENUM(FRC_API_TOUCHPAD)
#undef NAME

#define NAME Auto_2016
IMPL_ENUM(FRC_API_AUTO_2016)
#undef NAME

#define NAME Tower_face
IMPL_ENUM(FRC_API_TOWER_FACE)
#undef NAME

}
