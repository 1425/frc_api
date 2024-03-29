#ifndef FRC_API_CURL_H
#define FRC_API_CURL_H

#include<string>
#include<map>
#include<vector>

namespace frc_api{

using URL=std::string;

struct HTTP_error{
	std::string url;
	long response_code;
	std::string msg;
};

std::ostream& operator<<(std::ostream&,HTTP_error const&);

struct Get_result{
	std::map<std::string,std::string> headers;
	std::string data;
};

std::ostream& operator<<(std::ostream&,Get_result const&);

Get_result get_url(URL const&,std::vector<std::string> const& request_headers={});

}

#endif
