#ifndef FRC_API_QUERY_H
#define FRC_API_QUERY_H

#include<variant>
#include<string>
#include<tuple>
#include "data.h"
#include "util.h"

namespace frc_api{

class Season{
	int i;

	public:
	explicit Season(int);

	int get()const;
};
std::ostream& operator<<(std::ostream&,Season);
bool operator==(Season,int);
Season decode(JSON const&,const Season*);

class Match_number{
	int i;

	public:
	explicit Match_number(int);
	int get()const;
};
std::ostream& operator<<(std::ostream&,Match_number);
bool operator<(Match_number,Match_number);
Match_number decode(JSON const&,const Match_number*);

#define FRC_API_ALLIANCE_SELECTION(X)\
	X(Season,season)\
	X(Event_code,eventCode)

FRC_API_DATA(Alliance_selection,FRC_API_ALLIANCE_SELECTION)

#define FRC_API_API_INDEX(X)
FRC_API_DATA(API_index,FRC_API_API_INDEX)

using Which_event=std::variant<Event_code,Team_number,std::pair<Event_code,Team_number>>;

#define FRC_API_EVENT_AWARDS(X)\
	X(Season,season)\
	X(Which_event,which)

FRC_API_DATA(Event_awards,FRC_API_EVENT_AWARDS)

#define FRC_API_AWARD_LISTINGS(X)\
	X(Season,season)

FRC_API_DATA(Award_listings,FRC_API_AWARD_LISTINGS)

#define FRC_API_M1(X)\
	X(Team_number,teamNumber)\
	X(Tournament_level,tournamentLevel)\
	X(std::optional<Match_number>,start)\
	X(std::optional<Match_number>,end)

FRC_API_DATA(M1,FRC_API_M1)

#define FRC_API_M2(X)\
	X(Tournament_level,tournamentLevel)\
	X(std::optional<Match_number>,matchNumber)\
	X(std::optional<Match_number>,start)\
	X(std::optional<Match_number>,end)

FRC_API_DATA(M2,FRC_API_M2)

using Match_results_qualifier=std::variant<M1,Team_number,M2>;

#define FRC_API_MATCH_RESULTS(X)\
	X(Season,season)\
	X(Event_code,eventCode)\
	X(Match_results_qualifier,match_results_qualifier)

FRC_API_DATA(Match_results,FRC_API_MATCH_RESULTS)

#define FRC_API_SCORE_DETAILS(X)\
	X(Season,season)\
	X(Event_code,eventCode)\
	X(Tournament_level,tournamentLevel)\
	X(std::optional<Team_number>,teamNumber)\
	X(std::optional<Match_number>,matchNumber)\
	X(std::optional<Match_number>,start)\
	X(std::optional<Match_number>,end)

FRC_API_DATA(Score_details,FRC_API_SCORE_DETAILS)

//int is for if you only want the top N teams.
using Which_rankings=std::optional<std::variant<Team_number,int>>;

#define FRC_API_EVENT_RANKINGS(X)\
	X(Season,season)\
	X(Event_code,eventCode)\
	X(Which_rankings,which)

//	X(Team_number,teamNumber)
//	X(std::optional<int>,top)

FRC_API_DATA(Event_rankings,FRC_API_EVENT_RANKINGS)

using Which2=std::variant<Tournament_level,Team_number>;

#define FRC_API_EVENT_SCHEDULE(X)\
	X(Season,season)\
	X(Event_code,eventCode)\
	X(Which2,which2)\
	X(std::optional<Match_number>,start)\
	X(std::optional<Match_number>,end)

FRC_API_DATA(Event_schedule,FRC_API_EVENT_SCHEDULE)

#define FRC_API_HYBRID_SCHEDULE(X)\
	X(Season,season)\
	X(Event_code,eventCode)\
	X(Tournament_level,tournamentLevel)\
	X(std::optional<Match_number>,start)\
	X(std::optional<Match_number>,end)

FRC_API_DATA(Hybrid_schedule,FRC_API_HYBRID_SCHEDULE)

#define FRC_API_SEASON_SUMMARY(X)\
	X(Season,season)

FRC_API_DATA(Season_summary,FRC_API_SEASON_SUMMARY)

using ExcludeDistrict=bool;

using Event_criteria=std::tuple<
	std::optional<Team_number>,
	std::variant<std::optional<District_code>,ExcludeDistrict>
>;

using Which3=std::variant<
	Event_code,
	Event_criteria
>;

#define FRC_API_EVENT_LISTINGS(X)\
	X(Season,season)\
	X(Which3,which)

FRC_API_DATA(Event_listings,FRC_API_EVENT_LISTINGS)

#define FRC_API_DISTRICT_LISTINGS_QUERY(X)\
	X(Season,season)

FRC_API_DATA(District_listings,FRC_API_DISTRICT_LISTINGS_QUERY)

using State=std::string;//the whole name; not an abbreviation.

using Which4=std::variant<
	Team_number,
	std::tuple<std::optional<Event_code>,std::optional<District_code>,std::optional<State>,int>
>;

#define FRC_API_TEAM_LISTINGS(X)\
	X(Season,season)\
	X(Which4,which)

FRC_API_DATA(Team_listings,FRC_API_TEAM_LISTINGS)

using Which5=std::optional<std::variant<Team_number,Event_code>>;

#define FRC_API_REGISTRATIONS_QUERY(X)\
	X(Season,season)\
	X(Which5,which)

FRC_API_DATA(Registrations_query,FRC_API_REGISTRATIONS_QUERY)

#define FRC_API_QUERY_TYPES(X)\
	X(Alliance_selection,std::optional<Alliances>)\
	X(API_index,ApiIndex)\
	X(Event_awards,std::optional<Award>)\
	X(Award_listings,AwardListings)\
	X(Match_results,std::optional<Matches>)\
	X(Score_details,std::optional<MatchScores>)\
	X(Event_rankings,std::optional<Rankings>)\
	X(Event_schedule,Schedule)\
	X(Hybrid_schedule,HybridSchedule_whole)\
	X(Season_summary,Season_info)\
	X(Event_listings,Events)\
	X(District_listings,DistrictListings)\
	X(Team_listings,TeamListings)\
	X(Registrations_query,std::optional<Registrations>)

using Query=std::variant<
	Alliance_selection,
	API_index,
	Event_awards,
	Award_listings,
	Match_results,
	Score_details,
	Event_rankings,
	Event_schedule,
	Hybrid_schedule,
	Season_summary,
	Event_listings,
	District_listings,
	Team_listings,
	Registrations_query
>;

using URL=std::string;

URL url(Query const&);

std::string rand(const std::string*);
unsigned rand(const unsigned*);
int rand(const int*);
bool rand(const bool*);

template<typename T>
std::optional<T> rand(const std::optional<T>*){
	if(::rand()%2) return {};
	return rand((T*)0);
}

template<typename A,typename B,typename C,typename D>
std::tuple<A,B,C,D> rand(const std::tuple<A,B,C,D>*){
	return std::make_tuple(
		rand((A*)nullptr),
		rand((B*)nullptr),
		rand((C*)nullptr),
		rand((D*)nullptr)
	);
}

template<typename ... A>
struct Get_all{
	using Out=std::variant<A...>;

	template<typename A1>
	Out randomize(const std::variant<A1>*){
		return rand((A1*)nullptr);
	}

	template<typename A1,typename ... B1>
	Out randomize(const std::variant<A1,B1...>*){
		if(::rand()%2){
			return Out{rand((A1*)0)};
		}
		return randomize((std::variant<B1...>*)0);
	}
};

template<typename ... A>
std::variant<A...> rand(const std::variant<A...> *x){
	Get_all<A...> a{};
	return a.randomize(x);
}

}

#endif
