#ifndef FRC_API_UTIL_H
#define FRC_API_UTIL_H

#include<iostream>
#include<numeric>
#include<set>
#include<variant>
#include<map>
#include<vector>
#include<iterator>

namespace frc_api{

#define nyi { std::cout<<"nyi "<<__FILE__<<":"<<__LINE__<<"\n"; exit(44); }
#define FRC_API_NYI nyi
#define PRINT(X) std::cout<<""#X<<":"<<(X)<<"\n";

template<typename A,typename B>
std::ostream& operator<<(std::ostream& o,std::pair<A,B> const& p){
    return o<<"("<<p.first<<","<<p.second<<")";
}

template<typename A,typename B>
std::ostream& operator<<(std::ostream& o,std::map<A,B> const& a){
    o<<"{ ";
	for(auto const& elem:a) o<<elem<<" ";
    return o<<"}";
}

template<typename... A>
std::ostream& operator<<(std::ostream& o,std::variant<A...> const& a){
    std::visit([&](auto &&elem){ o<<elem; },a);
    return o;
}

template<typename T>
std::ostream& operator<<(std::ostream& o,std::vector<T> const& a){
    o<<"[ ";
	copy(begin(a),end(a),std::ostream_iterator<T>(o," "));
    return o<<"]";
}

std::vector<size_t> range(size_t lim);
std::vector<size_t> range(size_t,size_t lim);

template<typename T>
std::ostream& operator<<(std::ostream& o,std::optional<T> const& a){
    if(a) return o<<*a;
    return o<<"NULL";
}

template<typename T>
std::set<T>& operator|=(std::set<T> &a,T t){
	a.insert(t);
	return a;
}

template<typename A,typename B>
std::ostream& operator<<(std::ostream& o,std::tuple<A,B> const& a){
	return o<<"("<<std::get<0>(a)<<","<<std::get<1>(a)<<")";
}

template<typename A,typename B,typename C,typename D>
std::ostream& operator<<(std::ostream& o,std::tuple<A,B,C,D> const& a){
	o<<"(";
	o<<std::get<0>(a)<<",";
	o<<std::get<1>(a)<<",";
	o<<std::get<2>(a)<<",";
	o<<std::get<3>(a);
	return o<<")";
}

}

#endif
