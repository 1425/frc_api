#include "util.h"

namespace frc_api{

std::vector<size_t> range(size_t start,size_t lim){
	std::vector<size_t> r(lim);
	std::iota(begin(r),end(r),start);
	return r;
}

std::vector<size_t> range(size_t lim){ return range(0,lim); }

}

