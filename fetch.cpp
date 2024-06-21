#include<fstream>
#include "query.h"
#include "curl.h"
#include "util.h"
#include "db.h"

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
	//using Item=decltype(*begin(t));
	//std::copy(begin(t),end(t),std::ostream_iterator<Item>(std::cout,"\n"));

	//Using this second implementation because the first one uses different rules to look for operator<<, which is a problem for things in std that have operator<< defined locally.
	for(auto const& elem:t){
		std::cout<<elem<<"\n";
	}
}

#define MAP(F,IN) mapf([&](auto a){ return F(a); },IN)

template<typename Fetcher,typename T>
auto run(Fetcher &fetcher,URL url,const T*){
	auto g=fetcher.fetch(url);
	//PRINT(g);
	//rapidjson::Document a;
	//a.Parse(g.second.c_str());
	simdjson::dom::parser parser;
	simdjson::padded_string str(g.second);
	auto doc=parser.parse(str);
	try{
		switch(doc.type()){
			case simdjson::dom::element_type::ARRAY:
				return decode(doc.get_array(),(T*)nullptr);
			case simdjson::dom::element_type::OBJECT:
				return decode(doc.get_object(),(T*)nullptr);
			case simdjson::dom::element_type::NULL_VALUE:
				return decode(nullptr,(T*)nullptr);
			default:
				throw Decode_error{typeid(T).name(),"","unexpected type"};
		}
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

int demo(){
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
		for(auto event_code:take(5,event_codes)){
			//PRINT(event_code);
			try{
				run(f,Alliance_selection{year,event_code});
			}catch(Decode_error){
				std::cout<<"Failed on "<<event_code<<"\n";
				//FRC_API_PRINT(event_code);
				continue;
			}catch(HTTP_error const&){
				std::cout<<"Failed on fetch: "<<event_code<<"\n";
				continue;
			}

			//auto d=run(f,Event_awards{year,event_code});
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

struct Args{
	bool demo=0;
	bool show_cache=0;
	bool help=0;
};

Args parse_args(int argc,char **argv){
	Args r;
	for(int i=1;i<argc;i++){
		std::string s=argv[i];
		if(s=="--help"){
			r.help=1;
		}else if(s=="--demo"){
			r.demo=1;
		}else if(s=="--show_cache"){
			r.show_cache=1;
		}else{
			std::cerr<<"Unrecognized argument.\n";
			exit(1);
		}
	}
	return r;
}

void help(){
	std::cout<<"--help\n";
	std::cout<<"\tShow this message.\n";
	std::cout<<"--demo\n";
	std::cout<<"\tRun some queries and see that everything is working.\n";
	std::cout<<"--show_cache\n";
	std::cout<<"\tPrint what's already cached.\n";
}

int show_cache(){
	Sqlite db("cache.db");
	auto x=db.query("SELECT url,date FROM cache");
	auto m=mapf(
		[](auto row){
			assert(row.size()==2);
			assert(row[0].first=="url");
			assert(row[0].second);
			assert(row[1].first=="date");
			assert(row[1].second);
			return make_pair(*row[0].second,*row[1].second);
		},
		x
	);
	print_lines(m);
	return 0;
}

int main1(int argc,char **argv){
	auto args=parse_args(argc,argv);
	if(args.show_cache){
		return show_cache();
	}else if(args.help){
		help();
		return 0;
	}else if(args.demo){
		return demo();
	}

	help();
	return 0;
}

int main(int argc,char **argv){
	try{
		return main1(argc,argv);
	}catch(Decode_error e){
		FRC_API_PRINT(e);
		return 1;
	}catch(std::runtime_error const& e){
		FRC_API_PRINT(e);
		return 1;
	}catch(std::invalid_argument const& e){
		std::cout<<"invalid_argument:"<<e.what()<<"\n";
		return 1;
	}catch(frc_api::HTTP_error const& e){
		std::cout<<e<<"\n";
		return 1;
	}catch(std::string const& s){
		std::cout<<"s:"<<s<<"\n";
		return 1;
	}
}
