#include "curl.h"
#include <curl/curl.h> 
#include<iostream>
#include<cassert>
#include<sstream>
#include "util.h"

namespace frc_api{

std::ostream& operator<<(std::ostream& o,Get_result const& a){
	o<<"Get_result(";
	o<<a.headers<<a.data;
	return o<<")";
}

size_t write_callback1(char *ptr,size_t size,size_t nmemb,void *userdata){
	std::ostringstream& ss=*(std::ostringstream*)userdata;
	size_t len=size*nmemb;
	ss.write(ptr,len);
	return len;
}

size_t header_callback(char *ptr,size_t size,size_t nmemb,void *userdata){
	std::map<std::string,std::string>& out=*(std::map<std::string,std::string>*)userdata;
	const size_t len=size*nmemb;

	if(out.empty()){
		out["main"]=std::string(ptr,len-2);//-2 to cut off \r\n
		return len;
	}

	if(len==2 && ptr[0]=='\r' && ptr[1]=='\n') return len;

	size_t at=0;
	while(at<len && ptr[at]!=':') at++;
	assert(at<len);
	std::string name{ptr,at};
	at++;//skip space
	assert(at<len);
	std::string value{ptr+at+1,len-at-3}; //-3 rather than -1 to remove \r\n
	out[name]=value;

	return len;
}

struct Curl{
	CURL *data;

	Curl():data(curl_easy_init()){
		if(!data) throw "libcurl: failed to initialize";
	}

	Curl(Curl const&)=delete;
	Curl& operator=(Curl const&)=delete;

	~Curl(){
		curl_easy_cleanup(data);
	}

	operator CURL *()const{ return data; }	
};

Get_result get_url_inner(std::string const& url,std::vector<std::string> const& request_headers){
	Curl curl;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	/* example.com is redirected, so we tell libcurl to follow redirection */ 
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	std::ostringstream ss;
	std::map<std::string,std::string> headers;

	auto set=[&](auto item,auto value){
		auto r=curl_easy_setopt(curl,item,value);
		if(r) throw "libcurl: failed to set option";
	};

	set(CURLOPT_WRITEDATA,&ss);
	set(CURLOPT_WRITEFUNCTION,write_callback1); 

	set(CURLOPT_HEADERDATA,&headers);
	set(CURLOPT_HEADERFUNCTION,header_callback);

	struct curl_slist *chunk=NULL;

	if(request_headers.size()){
		for(auto const& a:request_headers){
			chunk=curl_slist_append(chunk,a.c_str());
		}
		set(CURLOPT_HTTPHEADER,chunk);
	}

	CURLcode res = curl_easy_perform(curl);
	if(res != CURLE_OK){
		std::ostringstream ss;
		ss<<"URL:"<<url<<"\n";
		ss<<"curl_easy_perform() failed: "<<curl_easy_strerror(res);
		throw ss.str();
	}

	if(chunk){
		curl_slist_free_all(chunk);
	}

	return Get_result{headers,ss.str()};
}

Get_result get_url(std::string const& url,std::vector<std::string> const& request_headers){
	//make two attempts.
	try{
		return get_url_inner(url,request_headers);
	}catch(std::string const& s){
		std::cout<<"Failure:"<<s<<" Going to retry.\n";
		return get_url_inner(url,request_headers);
	}
}

}

#ifdef CURL_TEST
int main(){
	cout<<get_url(string{"https://example.com"});
}
#endif
