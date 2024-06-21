#ifndef FRC_API_DATA_H
#define FRC_API_DATA_H

#include<iosfwd>
#include<string>
#include<vector>
#include<optional>
#include<any>
#include<variant>
#include "rapidjson/document.h"
#include "simdjson.h"

namespace frc_api{

using JSON=rapidjson::GenericValue<rapidjson::UTF8<>>;

#define FRC_API_INST(TYPE,NAME) TYPE NAME;

//TODO: Make this do stuff.
#define FRC_API_DATA(A,B) \
	struct A{\
		B(FRC_API_INST)\
		auto operator<=>(frc_api::A const&)const=default;\
	};\
	std::ostream& operator<<(std::ostream&,A const&);\
	A example(const A*);\
	A rand(const A*);\
	A decode(JSON const&,const A*);\
	A decode(JSON_value,const A*);\
	A decode(JSON_array,A const*);\
	A decode(JSON_object,A const*);\
	A decode(std::nullptr_t,A const*);\

#define FRC_API_LIST(A) A,

#define FRC_API_ENUM(A,B) \
	enum class A{B(FRC_API_LIST)};\
	std::ostream& operator<<(std::ostream&,A);\
	A example(const A*);\
	A rand(const A*);\
	A decode(JSON const&,const A*);\
	A decode(JSON_value,A const*);

class Team_number{
	int i;

	public:
	explicit Team_number(int);
	int get()const;

	auto operator<=>(Team_number const&)const=default;
};
std::ostream& operator<<(std::ostream&,Team_number);
Team_number decode(JSON const&,const Team_number*);
Team_number decode(JSON_value,const Team_number*);

class String2{
	//String of length at least 2 that is alphanumeric
	std::string s;

	public:
	explicit String2(std::string);
	std::string const& get()const;

	auto operator<=>(String2 const&)const=default;
};
std::ostream& operator<<(std::ostream&,String2 const&);
bool operator==(String2 const&,const char*);
bool operator!=(String2 const&,const char*);
String2 example(const String2*);
String2 rand(const String2*);
String2 decode(JSON const&,const String2*);
String2 decode(JSON_value,const String2*);

using Event_code=String2;
using District_code=String2;

using Alliance_number=int;//1-8

#define FRC_API_ALLIANCE(X)\
	X(std::optional<std::string>,name)\
	X(Alliance_number,number)\
	X(std::optional<Team_number>,captain)\
	X(std::optional<Team_number>,round1)\
	X(std::optional<Team_number>,round2)\
	X(std::optional<Team_number>,round3)\
	X(std::optional<Team_number>,backup)\
	X(std::optional<Team_number>,backupReplaced)

FRC_API_DATA(Alliance_t,FRC_API_ALLIANCE)

#define FRC_API_ALLIANCES(X)\
	X(int,count)\
	X(std::vector<Alliance_t>,Alliances)\

FRC_API_DATA(Alliances,FRC_API_ALLIANCES)

#define FRC_API_APIINDEX(X)\
	X(std::string,name)\
	X(std::string,apiVersion)\
	X(std::string,status)\
	X(std::string,serviceManifestName)\
	X(std::string,serviceManifestVersion)\
	X(std::string,codePackageName)\
	X(std::string,codePackageVersion)\
	X(int,currentSeason)\
	X(int,maxSeason)

FRC_API_DATA(ApiIndex,FRC_API_APIINDEX)

#define FRC_API_AWARD(X)\
	X(int,awardId)\
	X(std::optional<int>,teamId)\
	X(std::optional<int>,eventId)\
	X(std::optional<int>,eventDivisionId)\
	X(std::string,name)\
	X(int,series)\
	X(std::optional<Team_number>,teamNumber)\
	X(std::optional<std::string>,fullTeamName)\
	X(std::optional<std::string>,person)\
	X(std::string,eventCode)\
	X(std::optional<std::string>,schoolName)

FRC_API_DATA(Award,FRC_API_AWARD)\

#define FRC_API_AWARDS(X)\
	X(std::vector<Award>,Awards)

FRC_API_DATA(Awards,FRC_API_AWARDS)

#define FRC_API_AWARD_EVENT_TYPES(X)\
	X(None)\
	X(Regional)\
	X(DistrictEvent)\
	X(DistrictChampionship)\
	X(ChampionshipSubdivision)\
	X(ChampionshipDivision)\
	X(Championship)\
	X(DistrictChampionshipDivision)\
	X(DistrictChampionshipWithLevels)\
	X(Remote)\
	X(DistrictParent)\
	X(RegionalParent)\

FRC_API_ENUM(Award_Event_type,FRC_API_AWARD_EVENT_TYPES)

#define FRC_API_AWARD_LISTING(X)\
	X(int,awardId)\
	X(Award_Event_type,eventType)\
	X(std::string,description)\
	X(bool,forPerson)

FRC_API_DATA(AwardListing,FRC_API_AWARD_LISTING)

#define FRC_API_AWARDLISTINGS(X)\
	X(std::vector<AwardListing>,awards)

FRC_API_DATA(AwardListings,FRC_API_AWARDLISTINGS)

using Time=std::string;//TODO: Revisit this.

#define FRC_API_TOURNAMENT_LEVEL(X)\
	X(Qualification)\
	X(Playoff)

FRC_API_ENUM(Tournament_level,FRC_API_TOURNAMENT_LEVEL)

#define FRC_API_STATION(X)\
	X(Red1)\
	X(Red2)\
	X(Red3)\
	X(Blue1)\
	X(Blue2)\
	X(Blue3)

FRC_API_ENUM(Station,FRC_API_STATION)

#define FRC_API_MATCH_TEAM(X)\
	X(Team_number,teamNumber)\
	X(Station,station)\
	X(bool,dq)

FRC_API_DATA(Match_Team,FRC_API_MATCH_TEAM)

#define FRC_API_MATCH(X)\
	X(Time,actualStartTime)\
	X(std::string,description)\
	X(Tournament_level,tournamentLevel)\
	X(int,matchNumber)\
	X(std::optional<Time>,postResultTime)\
	X(int,scoreRedFinal)\
	X(int,scoreRedFoul)\
	X(int,scoreRedAuto)\
	X(int,scoreBlueFinal)\
	X(int,scoreBlueFoul)\
	X(int,scoreBlueAuto)\
	X(std::vector<Match_Team>,teams)\
	X(std::optional<std::string>,level)\
	X(std::string,autoStartTime)

FRC_API_DATA(Match,FRC_API_MATCH)

#define FRC_API_MATCHES(X)\
	X(std::vector<Match>,Matches)

FRC_API_DATA(Matches,FRC_API_MATCHES)

#define FRC_API_AUTO_2017(X)\
	X(Unknown)\
	X(None)\
	X(Mobility)

FRC_API_ENUM(Auto_2017,FRC_API_AUTO_2017)

#define FRC_API_TOUCHPAD(X)\
	X(None)\
	X(ReadyForTakeoff)

FRC_API_ENUM(Touchpad,FRC_API_TOUCHPAD)

//autoFuelLow can be -2; I have no idea why; 
//teleopFuelLow can be -5; why???
#define FRC_API_SCORE_2017(X)\
	X(std::string,alliance)\
	X(Auto_2017,robot1Auto)\
	X(Auto_2017,robot2Auto)\
	X(Auto_2017,robot3Auto)\
	X(int,autoFuelLow)\
	X(unsigned,autoFuelHigh)\
	X(bool,rotor1Auto)\
	X(bool,rotor2Auto)\
	X(bool,rotor1Engaged)\
	X(bool,rotor2Engaged)\
	X(bool,rotor3Engaged)\
	X(bool,rotor4Engaged)\
	X(int,teleopFuelLow)\
	X(unsigned,teleopFuelHigh)\
	X(Touchpad,touchpadNear)\
	X(Touchpad,touchpadMiddle)\
	X(Touchpad,touchpadFar)\
	X(std::optional<bool>,kPaRankingPointAchieved)\
	X(std::optional<bool>,rotorRankingPointAchieved)\
	X(unsigned,foulCount)\
	X(unsigned,autoPoints)\
	X(unsigned,autoMobilityPoints)\
	X(unsigned,autoRotorPoints)\
	X(unsigned,teleopPoints)\
	X(int,teleopFuelPoints)\
	X(unsigned,teleopRotorPoints)\
	X(unsigned,teleopTakeoffPoints)\
	X(std::optional<unsigned>,kPaBonusPoints)\
	X(std::optional<unsigned>,rotorBonusPoints)\
	X(int,adjustPoints)\
	X(unsigned,foulPoints)\
	X(unsigned,totalPoints)\
	X(unsigned,techFoulCount)\
	X(int,autoFuelPoints)\

FRC_API_DATA(Score_2017,FRC_API_SCORE_2017)

#define FRC_API_RESPONSE_DETAILS_2017(X)\
	X(std::string,matchLevel)\
	X(int,matchNumber)\
	X(std::vector<Score_2017>,alliances)

FRC_API_DATA(Response_details_2017,FRC_API_RESPONSE_DETAILS_2017)

#define FRC_API_AUTO_2016(X)\
	X(Unknown) X(None) X(Reached) X(Crossed)

FRC_API_ENUM(Auto_2016,FRC_API_AUTO_2016)

#define FRC_API_TOWER_FACE(X)\
	X(Unknown) X(None) X(Challenged) X(Scaled) X(Both)

FRC_API_ENUM(Tower_face,FRC_API_TOWER_FACE)

//using R0_2=int;//number 0-2 inclusive.

class R0_2{
	int i;

	public:
	explicit R0_2(int);
	int get()const;

	auto operator<=>(R0_2 const&)const=default;
};
std::ostream& operator<<(std::ostream&,R0_2);
R0_2 example(const R0_2*);
R0_2 rand(const R0_2*);
R0_2 decode(JSON const&,const R0_2*);
R0_2 decode(JSON_value,const R0_2*);

using Defense=std::string;//TODO: Make enum

#define FRC_API_SCORE_2016(X)\
	X(std::string,alliance)\
	X(Auto_2016,robot1Auto)\
	X(Auto_2016,robot2Auto)\
	X(Auto_2016,robot3Auto)\
	X(int,autoBouldersLow)\
	X(int,autoBouldersHigh)\
	X(int,teleopBouldersLow)\
	X(int,teleopBouldersHigh)\
	X(Tower_face,towerFaceA)\
	X(Tower_face,towerFaceB)\
	X(Tower_face,towerFaceC)\
	X(int,towerEndStrength)\
	X(bool,teleopTowerCaptured)\
	X(bool,teleopDefensesBreached)\
	X(R0_2,position1crossings)\
	X(Defense,position2)\
	X(R0_2,position2crossings)\
	X(Defense,position3)\
	X(R0_2,position3crossings)\
	X(Defense,position4)\
	X(R0_2,position4crossings)\
	X(Defense,position5)\
	X(R0_2,position5crossings)\
	X(int,foulCount)\
	X(int,techFoulCount)\
	X(int,autoPoints)\
	X(int,autoReachPoints)\
	X(int,autoCrossingPoints)\
	X(int,autoBoulderPoints)\
	X(int,teleopPoints)\
	X(int,teleopCrossingPoints)\
	X(int,teleopBoulderPoints)\
	X(int,teleopChallengePoints)\
	X(int,teleopScalePoints)\
	X(int,breachPoints)\
	X(int,capturePoints)\
	X(int,adjustPoints)\
	X(int,foulPoints)\
	X(int,totalPoints)

FRC_API_DATA(Score_2016,FRC_API_SCORE_2016)

#define FRC_API_RESPONSE_DETAILS_2016(X)\
	X(std::string,matchLevel)\
	X(int,matchNumber)\
	X(std::optional<std::string>,audienceGroup)\
	X(std::vector<Score_2016>,alliances)

//	X(std::vector<Alliance>,Alliances)

FRC_API_DATA(Response_details_2016,FRC_API_RESPONSE_DETAILS_2016)

using Coopertition=std::string;//TODO: Refine this.

#define FRC_API_ALLIANCE_COLOR(X) X(Blue) X(Red)
FRC_API_ENUM(Alliance_color,FRC_API_ALLIANCE_COLOR)
//using Alliance_color=std::string;

#define FRC_API_SCORE_2015(X)\
	X(bool,robotSet)\
	X(bool,containerSet)\
	X(bool,toteSet)\
	X(bool,toteStack)\
	X(int,toteCountFar)\
	X(int,toteCountNear)\
	X(int,containerCountLevel1)\
	X(int,containerCountLevel2)\
	X(int,containerCountLevel3)\
	X(int,containerCountLevel4)\
	X(int,containerCountLevel5)\
	X(int,containerCountLevel6)\
	X(int,litterCountLandfill)\
	X(int,litterCountContainer)\
	X(int,litterCountUnprocessed)\
	X(int,foulCount)\
	X(int,autoPoints)\
	X(int,totePoints)\
	X(int,containerPoints)\
	X(int,litterPoints)\
	X(int,teleopPoints)\
	X(int,adjustPoints)\
	X(int,foulPoints)\
	X(int,totalPoints)\
	X(Alliance_color,alliance)

FRC_API_DATA(Score_2015,FRC_API_SCORE_2015)

#define FRC_API_RESPONSE_DETAILS_2015(X)\
	X(std::string,matchLevel)\
	X(int,matchNumber)\
	X(Coopertition,coopertition)\
	X(int,coopertitionPoints)\
	X(std::vector<Score_2015>,alliances)

FRC_API_DATA(Response_details_2015,FRC_API_RESPONSE_DETAILS_2015)

#define FRC_API_SCORE_2018(X)\
	X(std::string,alliance)\
	X(std::string,autoRobot1)\
	X(std::string,autoRobot2)\
	X(std::string,autoRobot3)\
	X(int,autoSwitchOwnershipSec)\
	X(int,autoScaleOwnershipSec)\
	X(bool,autoSwitchAtZero)\
	X(int,autoRunPoints)\
	X(int,autoOwnershipPoints)\
	X(int,autoPoints)\
	X(int,teleopSwitchOwnershipSec)\
	X(int,teleopScaleOwnershipSec)\
	X(int,teleopSwitchBoostSec)\
	X(int,teleopSwitchForceSec)\
	X(int,teleopScaleForceSec)\
	X(int,teleopScaleBoostSec)\
	X(int,vaultForceTotal)\
	X(int,vaultForcePlayed)\
	X(int,vaultLevitateTotal)\
	X(int,vaultLevitatePlayed)\
	X(int,vaultBoostTotal)\
	X(int,vaultBoostPlayed)\
	X(std::string,endgameRobot1)\
	X(std::string,endgameRobot2)\
	X(std::string,endgameRobot3)\
	X(int,teleopPoints)\
	X(int,teleopOwnershipPoints)\
	X(int,vaultPoints)\
	X(int,endgamePoints)\
	X(bool,autoQuestRankingPoint)\
	X(bool,faceTheBossRankingPoint)\
	X(int,foulCount)\
	X(int,techFoulCount)\
	X(int,adjustPoints)\
	X(int,foulPoints)\
	X(int,rp)\
	X(int,totalPoints)

FRC_API_DATA(Score_2018,FRC_API_SCORE_2018)

#define FRC_API_RESPONSE_DETAILS_2018(X)\
	X(std::string,matchLevel)\
	X(int,matchNumber)\
	X(std::string,switchLeftNearColor)\
	X(std::string,switchRightNearColor)\
	X(std::string,scaleNearColor)\
	X(std::vector<Score_2018>,alliances)

FRC_API_DATA(Response_details_2018,FRC_API_RESPONSE_DETAILS_2018)

#define FRC_API_SCORE_2022(X)\
	X(std::string,alliance)\
	X(std::string,taxiRobot1)\
	X(std::string,endgameRobot1)\
	X(std::string,taxiRobot2)\
	X(std::string,endgameRobot2)\
	X(std::string,taxiRobot3)\
	X(std::string,endgameRobot3)\
	X(int,autoCargoLowerNear)\
	X(int,autoCargoLowerFar)\
	X(int,autoCargoLowerBlue)\
	X(int,autoCargoLowerRed)\
	X(int,autoCargoUpperNear)\
	X(int,autoCargoUpperFar)\
	X(int,autoCargoUpperBlue)\
	X(int,autoCargoUpperRed)\
	X(int,autoCargoTotal)\
	X(int,teleopCargoLowerNear)\
	X(int,teleopCargoLowerFar)\
	X(int,teleopCargoLowerBlue)\
	X(int,teleopCargoLowerRed)\
	X(int,teleopCargoUpperNear)\
	X(int,teleopCargoUpperFar)\
	X(int,teleopCargoUpperBlue)\
	X(int,teleopCargoUpperRed)\
	X(int,teleopCargoTotal)\
	X(int,matchCargoTotal)\
	X(int,autoTaxiPoints)\
	X(int,autoPoints)\
	X(int,autoCargoPoints)\
	X(bool,quintetAchieved)\
	X(int,teleopPoints)\
	X(int,teleopCargoPoints)\
	X(int,endgamePoints)\
	X(bool,cargoBonusRankingPoint)\
	X(bool,hangarBonusRankingPoint)\
	X(int,foulCount)\
	X(int,techFoulCount)\
	X(int,adjustPoints)\
	X(int,foulPoints)\
	X(int,rp)\
	X(int,totalPoints)

FRC_API_DATA(Score_2022,FRC_API_SCORE_2022)

#define FRC_API_RESPONSE_DETAILS_2022(X)\
	X(std::string,matchLevel)\
	X(int,matchNumber)\
	X(std::vector<Score_2022>,alliances)

FRC_API_DATA(Response_details_2022,FRC_API_RESPONSE_DETAILS_2022)

using MatchScore=std::variant<
	Response_details_2015,
	Response_details_2016,
	Response_details_2017,
	Response_details_2018,
	Response_details_2022
>;

#define FRC_API_MATCHSCORES(X)\
	X(std::vector<MatchScore>,MatchScores)

FRC_API_DATA(MatchScores,FRC_API_MATCHSCORES)

#define FRC_API_TEAMRANKING(X)\
	X(unsigned,dq)\
	X(unsigned,matchesPlayed)\
	X(double,qualAverage)\
	X(unsigned,rank)\
	X(Team_number,teamNumber)\
	X(unsigned,wins)\
	X(unsigned,losses)\
	X(unsigned,ties)\
	X(double,sortOrder1)\
	X(double,sortOrder2)\
	X(double,sortOrder3)\
	X(double,sortOrder4)\
	X(double,sortOrder5)\
	X(double,sortOrder6)\

FRC_API_DATA(TeamRanking,FRC_API_TEAMRANKING)

#define FRC_API_RANKINGS(X)\
	X(std::vector<TeamRanking>,Rankings)

FRC_API_DATA(Rankings,FRC_API_RANKINGS)

#define FRC_API_FIELD(X) X(Primary) X(Secondary)

FRC_API_ENUM(Field,FRC_API_FIELD)

#define FRC_API_SCHEDULEDMATCH_TEAM(X)\
	X(Team_number,teamNumber)\
	X(Station,station)\
	X(bool,surrogate)

FRC_API_DATA(ScheduledMatch_Team,FRC_API_SCHEDULEDMATCH_TEAM)

#define FRC_API_SCHEDULEDMATCH(X)\
	X(std::string,description)\
	X(Field,field)\
	X(Tournament_level,tournamentLevel)\
	X(int,matchNumber)\
	X(Time,startTime)\
	X(std::vector<ScheduledMatch_Team>,teams)\
	X(std::optional<std::string>,level)

FRC_API_DATA(ScheduledMatch,FRC_API_SCHEDULEDMATCH)

#define FRC_API_SCHEDULE(X)\
	X(std::vector<ScheduledMatch>,Schedule)

FRC_API_DATA(Schedule,FRC_API_SCHEDULE)

#define FRC_API_HYBRIDSCHEDULE_TEAM(X)\
	X(Team_number,teamNumber)\
	X(Station,station)\
	X(bool,surrogate)\
	X(std::optional<bool>,dq)

FRC_API_DATA(HybridSchedule_Team,FRC_API_HYBRIDSCHEDULE_TEAM)

#define FRC_API_HYBRIDSCHEDULE(X)\
	X(std::optional<Time>,actualStartTime)\
	X(std::string,description)\
	X(std::optional<std::string>,level)\
	X(int,matchNumber)\
	X(std::optional<Time>,postResultTime)\
	X(std::optional<unsigned>,scoreRedFinal)\
	X(std::optional<unsigned>,scoreRedFoul)\
	X(std::optional<unsigned>,scoreRedAuto)\
	X(std::optional<unsigned>,scoreBlueFinal)\
	X(std::optional<unsigned>,scoreBlueFoul)\
	X(std::optional<unsigned>,scoreBlueAuto)\
	X(Time,startTime)\
	X(Tournament_level,tournamentLevel)\
	X(std::vector<HybridSchedule_Team>,teams)

FRC_API_DATA(HybridSchedule,FRC_API_HYBRIDSCHEDULE)

#define FRC_API_HYBRIDSCHEDULE_WHOLE(X)\
	X(std::vector<HybridSchedule>,Schedule)

FRC_API_DATA(HybridSchedule_whole,FRC_API_HYBRIDSCHEDULE_WHOLE)

using Date=std::string;

#define FRC_API_FRCCHAMPIONSHIP(X)\
	X(std::string,name)\
	X(Date,startDate)\
	X(std::string,location)

FRC_API_DATA(FRCChampionship,FRC_API_FRCCHAMPIONSHIP)

#define FRC_API_SEASON(X)\
	X(unsigned,eventCount)\
	X(std::string,gameName)\
	X(Time,kickoff)\
	X(Team_number,rookieStart)\
	X(unsigned,teamCount)\
	X(std::vector<FRCChampionship>,frcChampionships)\
	X(std::optional<std::string>,frcChampionship)

FRC_API_DATA(Season_info,FRC_API_SEASON)

#define FRC_API_EVENT_EVENT_TYPES(X) \
	X(Regional)\
	X(DistrictEvent)\
	X(DistrictChampionship)\
	X(DistrictChampionshipWithLevels)\
	X(DistrictChampionshipDivision)\
	X(ChampionshipSubdivision)\
	X(ChampionshipDivision)\
	X(Championship)\
	X(OffSeason)\
	X(OffSeasonWithAzureSync)\
	X(DistrictParent)\
	X(RegionalParent)

FRC_API_ENUM(Event_Event_type,FRC_API_EVENT_EVENT_TYPES)

#define FRC_API_EVENT(X)\
	X(Event_code,code)\
	X(std::optional<std::string>,divisionCode)\
	X(std::string,name)\
	X(Event_Event_type,type)\
	X(std::optional<District_code>,districtCode)\
	X(std::string,venue)\
	X(std::optional<std::string>,address)\
	X(std::string,city)\
	X(std::string,stateprov)\
	X(std::string,country)\
	X(std::optional<std::string>,website)\
	X(std::string,timezone)\
	X(Date,dateStart)\
	X(Date,dateEnd)\
	X(std::vector<std::string>,webcasts)

FRC_API_DATA(Event,FRC_API_EVENT)

#define FRC_API_EVENTS(X)\
	X(std::vector<Event>,Events)\
	X(unsigned,eventCount)

FRC_API_DATA(Events,FRC_API_EVENTS)

#define FRC_API_DISTRICT(X)\
	X(District_code,code)\
	X(std::string,name)

FRC_API_DATA(District,FRC_API_DISTRICT)

#define FRC_API_DISTRICTLISTINGS(X)\
	X(unsigned,districtCount)\
	X(std::vector<District>,districts)

FRC_API_DATA(DistrictListings,FRC_API_DISTRICTLISTINGS)

#define FRC_API_TEAMLISTINGS_TEAM(X)\
	X(Team_number,teamNumber)\
	X(std::string,nameFull)\
	X(std::string,nameShort)\
	X(std::optional<std::string>,schoolName)\
	X(std::optional<Event_code>,homeCMP)\
	X(std::string,city)\
	X(std::optional<std::string>,stateProv)\
	X(std::string,country)\
	X(std::optional<std::string>,website)\
	X(int,rookieYear)\
	X(std::optional<std::string>,robotName)\
	X(std::optional<District_code>,districtCode)

FRC_API_DATA(TeamListings_Team,FRC_API_TEAMLISTINGS_TEAM)

#define FRC_API_TEAMLISTINGS(X)\
	X(int,pageCurrent)\
	X(int,pageTotal)\
	X(int,teamCountPage)\
	X(int,teamCountTotal)\
	X(std::vector<TeamListings_Team>,teams)

FRC_API_DATA(TeamListings,FRC_API_TEAMLISTINGS)

#define FRC_API_REGISTRATIONRECORD(X)\
	X(Team_number,teamNumber)\
	X(std::vector<Event_code>,Events)

FRC_API_DATA(RegistrationRecord,FRC_API_REGISTRATIONRECORD)

#define FRC_API_REGISTRATIONS(X)\
	X(unsigned,registrationCountTotal)\
	X(unsigned,registrationCountPage)\
	X(unsigned,pageCurrent)\
	X(unsigned,pageTotal)\
	X(std::vector<RegistrationRecord>,Registrations)

FRC_API_DATA(Registrations,FRC_API_REGISTRATIONS)

}

#endif
