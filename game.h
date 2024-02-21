

#pragma once

#include <string>
#include <map>
#include <vector>
#include <stdlib.h>

typedef enum {PENALTY, GOAL, SHOT} ETYPE;
typedef enum {PP, BP, ES} GFORMAT;



struct event {
    std::string player_name;  /* The "main" player of the event (penalty, goal, shot) */
    std::string team_short, ass1 = "", ass2 = "", penalty_type = "";
    /* Format: "MM:SS" */
    std::pair<int, int> time;
    std::pair<int, int> score;
    ETYPE event_type;
    GFORMAT goal_format; 
};

struct game {
    std::string id;
    std::string date;
    std::string arena;

    int home_goals, away_goals;  /* Total number of goals for each team. */
    std::string home_name, away_name;  /* Team names for each team. */
    std::vector<int> home_goals_per_period, away_goals_per_period{};  /* Goals per period for each team. */
    // struct team home_team, away_team;
    std::vector<event*> events{};
};


const int SKIP_TWO = 2;
const int SKIP_THREE = 3;
const int LEN_EVENT_TYPE = 3;
const int LEN_TIME_STAMP = 7;

const std::string MATCH_DATE = "Matchdatum: ";
const std::string MATCH_ID = "Match-id: ";
const std::string ARENA_NAME = "Arena:";  /* No whitespace, bug in external game report. */
const std::string ICE_TIME = "Istid";
const std::string TEAM_GRADE = "Lagbetyg";


const char OPEN_PAREN = '(';
const char CLOSED_PAREN = ')';


struct game *parse_game(std::string filename);
struct game *init_game(std::string line);
void print_game_stats(struct game *g);

