#include "query.h"
#include<iostream>
#include<memory>
#include<numeric>
#include<cassert>
#include<set>
#include<sstream>
#include "util.h"
#include "rapidjson.h"

#define nyi FRC_API_NYI

namespace frc_api{

//generic stuff

//std::ostream& operator<<(std::ostream&,std::any)FRC_API_NYI
std::any example(const std::any*)FRC_API_NYI
std::any rand(const std::any*)FRC_API_NYI
//bool operator<(std::any,std::any)FRC_API_NYI

std::any decode(JSON const& a,const std::any*){
	std::cout<<a<<"\n";
	FRC_API_NYI
}

bool example(const bool*){ return 0; }

unsigned example(const unsigned*){ return 0; }
unsigned rand(const unsigned*)FRC_API_NYI

double example(const double*){ return 0; }
double rand(const double*)FRC_API_NYI

//start normal program logic

static const int MAX_YEAR=2024;

Season::Season(int i1):i(i1){
	if(i<2015 || i>MAX_YEAR){
		throw std::invalid_argument("Season");
	}
}

int Season::get()const{ return i; }

Season& Season::operator++(){
	if(i==MAX_YEAR){
		throw std::invalid_argument("Season inc");
	}
	i++;
	return *this;
}

bool operator==(Season a,int i){
	return a.get()==i;
}

std::ostream& operator<<(std::ostream& o,Season a){
	return o<<a.get();
}

Season example(const Season*){ return Season{2015}; }
Season rand(const Season*){ return Season{2015+::rand()%3}; }

Season decode(JSON const&,const Season*)FRC_API_NYI

bool rand(const bool*){ return ::rand()%2; }

static const int MAX_TEAM=10000;

bool normal_team_number(int i){
	return i>=1 && i<MAX_TEAM;
}

bool tmp_team_number(int i){
	auto base=(2018*100000);
	return i>=base && i<=base+1000;
}

Team_number::Team_number(int i1):i(i1){
	if(!normal_team_number(i) && !tmp_team_number(i)){
		throw std::invalid_argument("Team_number:"+to_string(i));
	}
}

int Team_number::get()const{ return i; }

std::ostream& operator<<(std::ostream& o,Team_number a){
	return o<<a.get();
}

Team_number example(const Team_number*){
	return Team_number{1};
}

Team_number rand(const Team_number*){
	return Team_number{1+(::rand()%MAX_TEAM)};
}

Match_number::Match_number(int i1):i(i1){
	if(i<1 || i>200){
		throw std::invalid_argument("Match_number");
	}
}

int Match_number::get()const{ return i; }

std::ostream& operator<<(std::ostream& o,Match_number a){
	return o<<a.get();
}

Match_number rand(const Match_number*){
	return Match_number{1+(::rand()%200)};
}

Match_number example(const Match_number*){
	return Match_number(1);
}

Match_number decode(JSON const&,const Match_number*){
	FRC_API_NYI
}

int example(const int*){ return 0; }
std::string example(const std::string*){ return "ex"; }

template<typename T>
std::optional<T> example(const std::optional<T>*){
	return {};
}

template<typename A,typename ... B>
std::variant<A,B...> example(const std::variant<A,B...>*){
	return example((A*)0);
}

template<typename T>
std::vector<T> example(const std::vector<T>*){
	return {};
}

int rand(const int*){ return ::rand(); }
std::string rand(const std::string*){ return std::string{(char)('a'+(::rand()%26))}; }

template<typename T>
std::optional<T> rand(const std::optional<T>*);

template<typename T>
std::vector<T> rand(const std::vector<T>*)nyi

template<typename A,typename B>
std::pair<A,B> rand(const std::pair<A,B>*){
	return std::make_pair(rand((A*)0),rand((B*)0));
}


template<typename A,typename B>
std::tuple<A,B> rand(const std::tuple<A,B>*){
	return make_tuple(rand((A*)0),rand((B*)0));
}

#define OUTPUT(A,B) o<<""#B<<":"<<a.B<<" ";
#define EXAMPLE(A,B) example((const A*)nullptr),
#define RAND(A,B) rand((const A*)nullptr),
#define LT(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
//#define DECODE(A,B) decode(in[""#B],(A*)nullptr),
#define DECODE(A,B) [&](){ \
		if(!in.HasMember(""#B)) throw Decode_error{""#B,in,"not found"}; \
		try{\
			return decode(in[""#B],(A*)nullptr); \
		}catch(Decode_error e){\
			e.path.push_back(""#B);\
			throw e;\
		}\
	}(),

#define RETURN_IF_PRESENT(A,B) if(x.name==""#B) return;

#define IMPL(NAME,ITEMS) \
	std::ostream& operator<<(std::ostream& o,NAME const& a){\
		(void)a; \
		o<<""#NAME<<"( ";\
		ITEMS(OUTPUT)\
		return o<<")";\
	}\
	NAME example(const NAME*){\
		return NAME{ITEMS(EXAMPLE)};\
	}\
	NAME rand(const NAME*){\
		return NAME{ITEMS(RAND)};\
	}\
	NAME decode(JSON const& in,const NAME*){\
		if(!in.IsObject()) throw Decode_error{""#NAME,in,"expected object"};\
		std::for_each(in.MemberBegin(),in.MemberEnd(),[&](auto const& x){\
			(void)x; \
			ITEMS(RETURN_IF_PRESENT)\
			throw Decode_error{""#NAME,in,"unexpected:"+to_string(x.name)};\
		});\
		try{\
			return NAME{ITEMS(DECODE)};\
		}catch(Decode_error e){\
			e.path.push_back(""#NAME);\
			throw e; \
		}\
	}


IMPL(API_index,FRC_API_API_INDEX)
IMPL(Alliance_selection,FRC_API_ALLIANCE_SELECTION)
IMPL(Match_results,FRC_API_MATCH_RESULTS)
IMPL(M1,FRC_API_M1)
IMPL(M2,FRC_API_M2)
IMPL(Award_listings,FRC_API_AWARD_LISTINGS)
IMPL(Event_awards,FRC_API_EVENT_AWARDS)

IMPL(Event_listings,FRC_API_EVENT_LISTINGS)
IMPL(Score_details,FRC_API_SCORE_DETAILS)
IMPL(Season_summary,FRC_API_SEASON_SUMMARY)
IMPL(Hybrid_schedule,FRC_API_HYBRID_SCHEDULE)
IMPL(Event_schedule,FRC_API_EVENT_SCHEDULE)
IMPL(Event_rankings,FRC_API_EVENT_RANKINGS)

IMPL(District_listings,FRC_API_DISTRICT_LISTINGS_QUERY)
IMPL(Team_listings,FRC_API_TEAM_LISTINGS)
IMPL(Registrations_query,FRC_API_REGISTRATIONS_QUERY)

//TODO: Move to data.cpp
IMPL(Alliance_t,FRC_API_ALLIANCE)
IMPL(Alliances,FRC_API_ALLIANCES)
IMPL(ApiIndex,FRC_API_APIINDEX)
IMPL(Award,FRC_API_AWARD)
IMPL(Awards,FRC_API_AWARDS)
IMPL(AwardListing,FRC_API_AWARD_LISTING)
IMPL(Match,FRC_API_MATCH)
IMPL(Match_Team,FRC_API_MATCH_TEAM)
//IMPL(MatchScore,FRC_API_MATCHSCORE)
IMPL(TeamRanking,FRC_API_TEAMRANKING)
IMPL(ScheduledMatch,FRC_API_SCHEDULEDMATCH)
IMPL(HybridSchedule,FRC_API_HYBRIDSCHEDULE)
IMPL(ScheduledMatch_Team,FRC_API_SCHEDULEDMATCH_TEAM)
IMPL(HybridSchedule_Team,FRC_API_HYBRIDSCHEDULE_TEAM)
IMPL(Season_info,FRC_API_SEASON)
IMPL(Events,FRC_API_EVENTS)
IMPL(DistrictListings,FRC_API_DISTRICTLISTINGS)
IMPL(FRCChampionship,FRC_API_FRCCHAMPIONSHIP)
IMPL(Event,FRC_API_EVENT)
IMPL(TeamListings,FRC_API_TEAMLISTINGS)
IMPL(Registrations,FRC_API_REGISTRATIONS)
IMPL(RegistrationRecord,FRC_API_REGISTRATIONRECORD)
IMPL(AwardListings,FRC_API_AWARDLISTINGS)
IMPL(Matches,FRC_API_MATCHES)
IMPL(Schedule,FRC_API_SCHEDULE)
IMPL(HybridSchedule_whole,FRC_API_HYBRIDSCHEDULE_WHOLE)
IMPL(District,FRC_API_DISTRICT)
IMPL(TeamListings_Team,FRC_API_TEAMLISTINGS_TEAM)
IMPL(MatchScores,FRC_API_MATCHSCORES)
IMPL(Response_details_2015,FRC_API_RESPONSE_DETAILS_2015)
IMPL(Score_2015,FRC_API_SCORE_2015)
IMPL(Response_details_2016,FRC_API_RESPONSE_DETAILS_2016)
IMPL(Score_2017,FRC_API_SCORE_2017)
IMPL(Score_2016,FRC_API_SCORE_2016)
IMPL(Response_details_2017,FRC_API_RESPONSE_DETAILS_2017)
IMPL(Score_2018,FRC_API_SCORE_2018)
IMPL(Response_details_2018,FRC_API_RESPONSE_DETAILS_2018)
IMPL(Score_2022,FRC_API_SCORE_2022)
IMPL(Response_details_2022,FRC_API_RESPONSE_DETAILS_2022)
IMPL(Rankings,FRC_API_RANKINGS)

URL url(Alliance_selection const& a){
	std::ostringstream ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/alliances/"<<a.eventCode;
	return ss.str();
}

URL url(API_index){
	return "https://frc-api.firstinspires.org/v2.0/";
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

URL url(Event_awards const& a){
	//it appears that this API is broken on the other end.
	std::ostringstream ss;
	ss<<"https://frc-api.firstinspires.org/v3.0/"<<a.season<<"/awards/";
	std::visit(overloaded{
			[&ss](Team_number a){ nyi ss<<"teamNumber="<<a; },
			[&ss](Event_code a){ ss<<"event/"<<a; },
			[&ss](std::pair<Event_code,Team_number> p){
				ss<<"eventCode="<<p.first<<"&teamNumber="<<p.second;
			}
		},
		a.which
	);
	return ss.str();
}

URL url(Award_listings const& a){
	std::ostringstream ss;
	ss<<"https://frc-api.firstinspires.org/v3.0/"<<a.season<<"/awards/list";
	return ss.str();
}

class URL_generator{
	std::ostringstream ss;
	bool start_section=1;

	public:

	template<typename T>
	URL_generator& operator<<(T const& t){
		assert(start_section);
		ss<<t;
		return *this;
	}

	template<typename T>
	void add(std::string_view name,T const& value){
		if(start_section){
			ss<<"?";
			start_section=0;
		}else{
			ss<<"&";
		}
		ss<<name<<"="<<value;
	}

	template<typename T>
	void add(std::string_view name,std::optional<T> const& value){
		if(value) add(name,*value);
	}

	auto str()const{ return ss.str(); }
};

URL url(Match_results const& a){
	URL_generator ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/matches/"<<a.eventCode;

	#define ADD_ITEMS(A,B) ss.add(""#B,a.B);

	visit(
		overloaded{
			[&](M1 const& a){
				FRC_API_M1(ADD_ITEMS)
			},
			[&](M2 const& a){ FRC_API_M2(ADD_ITEMS) },
			[&](Team_number a){ ss.add("teamNumber",a); }
		},
		a.match_results_qualifier
	);
	return ss.str();
}

URL url(Score_details const& a){
	URL_generator ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/scores/"<<a.eventCode<<"/"<<a.tournamentLevel;
	ss.add("teamNumber",a.teamNumber);
	ss.add("matchNumber",a.matchNumber);
	ss.add("start",a.start);
	ss.add("end",a.end);
	return ss.str();
}

URL url(Event_rankings const& a){
	URL_generator ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/rankings/"<<a.eventCode;
	if(a.which){
		visit(
			overloaded{
				[&](Team_number a){ ss.add("teamNumber",a); },
				[&](int i){ ss.add("top",i); }
			},
			*a.which
		);
	}
	return ss.str();
}

URL url(Event_schedule const& a){
	URL_generator ss;
	//ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/schedule/"<<a.eventCode;
	ss<<"https://frc-api.firstinspires.org/v3.0/"<<a.season<<"/schedule/"<<a.eventCode;
	//tournamentLevel=&teamNumber=101&start=20&end=25
	visit(
		overloaded{
			[&](Tournament_level t){ ss.add("tournamentLevel",t); },
			[&](Team_number t){ ss.add("teamNumber",t); }
		},
		a.which2
	);
	ss.add("start",a.start);
	ss.add("end",a.end);
	return ss.str();
}

URL url(Hybrid_schedule const& a){
	URL_generator ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/schedule/"<<a.eventCode<<"/"<<a.tournamentLevel<<"/hybrid";
	ss.add("start",a.start);
	ss.add("end",a.end);
	return ss.str();
}

URL url(Season_summary const& a){
	URL_generator ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season;
	return ss.str();
}

URL url(Event_listings const& a){
	URL_generator ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/events";
	//?eventCode=CMPMO&teamNumber=1&districtCode=PNW&excludeDistrict=true
	visit(
		overloaded{
			[&](std::optional<Event_code> a){ ss.add("eventCode",a); },
			[&](Event_criteria a){
				ss.add("teamNumber",std::get<0>(a));
				visit(
					overloaded{
						[&](std::optional<District_code> a){
							ss.add("districtCode",a);
						},
						[&](ExcludeDistrict a){
							ss.add("excludeDistrict",a?"true":"false");
						}
					},
					std::get<1>(a)
				);
			}
		},
		a.which
	);
	return ss.str();
}

URL url(District_listings const& a){
	URL_generator ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/districts";
	return ss.str();
}

URL url(Team_listings const& a){
	URL_generator ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/teams";
	//?teamNumber=101&eventCode=CMP&districtCode=PNW&page=2";
	visit(
		overloaded{
			[&](Team_number a){ ss.add("teamNumber",a); },
			[&](auto a){
				ss.add("eventCode",std::get<0>(a));
				ss.add("districtCode",std::get<1>(a));
				ss.add("state",std::get<2>(a));
				ss.add("page",std::get<3>(a));
			}
		},
		a.which
	);
	return ss.str();
}

URL url(Registrations_query const& a){
	URL_generator ss;
	ss<<"https://frc-api.firstinspires.org/v2.0/"<<a.season<<"/registrations";
	if(a.which){
		visit(
			overloaded{
				[&](Team_number a){ ss.add("teamNumber",a); },
				[&](Event_code a){ ss.add("eventCode",a); }
			},
			*a.which
		);
	}
	return ss.str();
}

template<typename A,typename ... B>
URL url1(std::variant<A,B...> const& a){
	return std::visit([](auto const& b){
		return url(b);
	},a);
}

URL url(Query const& a){
	return url1(a);
}

int main(){
	//do a breadth-first search for queries somehow...
	//cout<<example((Query*)0);
	
	//auto e=examples((Query*)nullptr);
	std::set<Query> seen;
	std::set<URL> urls;
	int since_new=0;
	while(since_new<200){
		auto v=rand((Query*)nullptr);
		if(seen.count(v)){
			since_new++;
		}else{
			since_new=0;
			seen|=v;
			FRC_API_PRINT(v);
			auto u=url(v);
			//check that all of the generated urls are unique!
			if(urls.count(u)){
				FRC_API_PRINT(u);
			}
			assert(urls.count(u)==0);
			urls|=u;
		}
	}

	return 0;
}

}

//int main(){ return frc_api::main(); }
