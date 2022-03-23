#include<fstream>
#include "query.h"
#include "curl.h"
#include "util.h"
#include "db.h"
#include "rapidjson.h"

using namespace frc_api;
//using namespace std;

template<typename T>
std::vector<T> take(size_t n,std::vector<T> const& v){
	return std::vector<T>{v.begin(),v.begin()+std::min(n,v.size())};
}

template<typename T>
std::set<T>& operator|=(std::set<T> &a,std::vector<T> const& in){
	copy(begin(in),end(in),inserter(a,a.end()));
	return a;
}

template<typename Func,typename T>
auto mapf(Func f,std::vector<T> in){
	std::vector<decltype(f(in[0]))> r;
	transform(begin(in),end(in),back_inserter(r),f);
	return r;
}

template<typename Func,typename T>
std::vector<T> filter(Func f,std::vector<T> t){
	std::vector<T> out;
	copy_if(begin(t),end(t),back_inserter(out),f);
	return out;
}

template<typename T>
void print_lines(T const& t){
	using Item=decltype(*begin(t));
	copy(begin(t),end(t),std::ostream_iterator<Item>(std::cout,"\n"));
}

#define MAP(F,IN) mapf([&](auto a){ return F(a); },IN)

template<typename Fetcher,typename T>
auto run(Fetcher &fetcher,URL url,const T*){
	auto g=fetcher.fetch(url);
	//PRINT(g);
	rapidjson::Document a;
	a.Parse(g.second.c_str());
	try{
		return decode(a,(T*)nullptr);
	}catch(...){
		std::cout<<url<<"\n";
		throw;
	}
}

#define X(A,B) \
	template<typename Fetcher>\
	B run(Fetcher &fetcher,A a){\
		return run(fetcher,url(a),(B*)nullptr);\
	}
FRC_API_QUERY_TYPES(X)
#undef X

template<typename Fetcher>
auto run1(Fetcher &fetcher,Query const& q){
	return visit([&](auto a){
		auto g=run(fetcher,a);
		PRINT(g);
	},q);
}

struct Query_generator{
	std::set<Query> seen;

	struct iterator{
		Query_generator *parent;
		std::optional<Query> q;

		iterator(Query_generator *p):parent(p){
			if(parent){
				++*this;
			}
		}

		Query operator*(){
			assert(q);
			return *q;
		}

		bool operator!=(iterator const& a){
			return parent!=a.parent;
		}

		iterator& operator++(){
			assert(parent);
			for(auto _:range(20)){
				(void)_;
				auto r=rand((Query*)nullptr);
				if(parent->seen.count(r)==0){
					parent->seen.insert(r);
					q=r;
					return *this;
				}
			}
			parent=nullptr;
			return *this;
		}
	};

	iterator begin(){ return iterator{this}; }
	iterator end(){ return iterator{NULL}; }
};

template<typename T>
auto code(T t){ return t.code; }

int main1(){
	std::string key;
	{
		std::ifstream f("api_key");
		getline(f,key);
	}

	Cached_fetcher f{Fetcher{Nonempty_string{key}},Cache{}};
	auto u=url(Alliance_selection{Season{2022},Event_code{"orwil"}});
	run(f,u,(Alliances*)nullptr);
	//run1(f,Alliance_selection{Season{2015},Event_code{"orwil"}});

	/*for(auto q:Query_generator()){
		PRINT(q);
		run1(f,q);
	}*/

	{
		auto g=run(f,API_index{});
		FRC_API_PRINT(g);
	}

	std::vector<Season> years{
		Season{2015},
		Season{2016},
		Season{2017},
		Season{2018},
		Season{2022},
	};
	std::set<Team_number> teams;

	for(auto year:years){
		FRC_API_PRINT(year);
		auto g=run(f,Event_listings{year,Event_criteria{}});
		/*auto g=run(
			f,
			Event_listings{
				year,
				Event_criteria{Team_number{1425},{}}
			}
		);*/
		//PRINT(g);
		auto event_codes=MAP(code,g.Events);
		FRC_API_PRINT(event_codes);
		FRC_API_PRINT(event_codes.size());
		for(auto event_code:take(2000,event_codes)){
			//PRINT(event_code);
			try{
				run(f,Alliance_selection{year,event_code});
			}catch(Decode_error){
				std::cout<<"Failed on "<<event_code<<"\n";
				//FRC_API_PRINT(event_code);
				continue;
			}

			auto d=run(f,Event_awards{year,event_code});
			//PRINT(d);

			auto g=run(
				f,
				Match_results{
					year,
					event_code,
					Match_results_qualifier{M2{Tournament_level::Qualification,{},{},{}}}
				}
			);
			//PRINT(g);

			{
				auto g=run(f,Score_details{year,event_code,Tournament_level::Qualification,{},{},{},{}});
				//PRINT(g);
			}

			try{
				auto g2=run(f,Event_schedule{year,event_code,Tournament_level::Qualification,{},{}});
				//PRINT(g2);
			}catch(Decode_error const& a){
				FRC_API_PRINT(a);
			}

			auto g3=run(f,Hybrid_schedule{year,event_code,Tournament_level::Qualification,{},{}});
			//PRINT(g3);

			auto g4=run(f,Registrations_query{year,event_code});
			//PRINT(g4);

			auto g5=run(f,Event_rankings{year,event_code,{}});
			//PRINT(g5);
		}
		{
			auto g=run(f,Award_listings{year});
			//PRINT(g);
		}
		{
			auto g=run(f,Season_summary{year});
			//PRINT(g);
		}
		{
			auto g=run(f,District_listings{year});
			//PRINT(g);
		}

		unsigned lim=1;
		for(unsigned i=1;i<=lim;i++){
			auto g=run(
				f,
				Team_listings{
					year,
					make_tuple(std::optional<Event_code>{},std::optional<District_code>{},std::optional<State>{},i)
				}
			);
			lim=g.pageTotal;
			teams|=mapf([](auto a){ return a.teamNumber; },g.teams);
			//PRINT(g);
		}
	}

	print_lines(teams);

	return 0;
}

std::ostream& operator<<(std::ostream& o,std::runtime_error const& e){
	return o<<"runtime_error("<<e.what()<<")";
}

int main(){
	try{
		return main1();
	}catch(Decode_error e){
		FRC_API_PRINT(e);
		return 1;
	}catch(std::runtime_error const& e){
		FRC_API_PRINT(e);
		return 1;
	}catch(std::invalid_argument const& e){
		std::cout<<"invalid_argument:"<<e.what()<<"\n";
		return 1;
	}
}
