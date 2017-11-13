#ifndef TBA_CURL_H
#define TBA_CURL_H

#include<string>
#include<map>
#include<vector>

namespace frc_api{

using URL=std::string;

struct Get_result{
	std::map<std::string,std::string> headers;
	std::string data;
};

std::ostream& operator<<(std::ostream&,Get_result const&);

Get_result get_url(URL const&,std::vector<std::string> const& request_headers={});

}

#endif
