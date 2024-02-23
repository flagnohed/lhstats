#pragma once

#include <string>
#include <map>
#include <vector>
#include <stdlib.h>

typedef enum {PENALTY, GOAL, SHOT} ETYPE;
typedef enum {PP, BP, ES} GFORMAT;

const int SKIP_TWO = 2;
const int SKIP_THREE = 3;
const int LEN_EVENT_TYPE = 3;
const int LEN_TIME_STAMP = 7;

const std::string MATCH_DATE = "Matchdatum: ";
const std::string MATCH_ID = "Match-id: ";
const std::string ARENA_NAME = "Arena:";  /* No whitespace, bug in external game report. */
const std::string ICE_TIME = "Istid: ";
const std::string TEAM_GRADE = "Lagbetyg: ";
// const std::string LINE_TACTICS = "Femma ";  /* Don't care right now, but maybe in future to better fetch icetimes. */


const char OPEN_PAREN = '(';
const char CLOSED_PAREN = ')';

struct event {
    std::string player_name;    /* The "main" player of the event (penalty, goal, shot) */
    std::string team_short, ass1 = "", ass2 = "", penalty_type = "";
  
    std::pair<int, int> time;   /* Format in game file: "MM:SS" */
    std::pair<int, int> score;  
    ETYPE event_type;           /* PENALTY, GOAL, SHOT. */  
    GFORMAT goal_format;        /* PP, BP, ES. */
};

/* This struct keeps track of a certain team's stats in a certain game. 
    These stats are then used to update total team stats. */
struct tmp_team {
    int goals, grade;
    std::string name;
    std::vector<int> goals_per_period{};  
    std::vector<int> ice_time{};

};

struct game {
    std::string id;                 /* Game-Id parsed from game file. */
    std::string date;               /* Might not be useful right now, but doesn't hurt to have. */
    std::string arena;              /* Again, not useful now but would like to introduce percentage of arena filled. */
    struct tmp_team *ht, *at;        /* Keeps track on basic stats for each team in the game (those that do not require calculations). */
    std::vector<event*> events{};   /* All events during the game in chronological order. */
};


struct game *parse_game(std::string filename);  /* Takes the name of the game file, and parses into a game struct. */
struct game *init_game(std::string line);       /* Helper function to parse_game, handles the first row and initializes game. */
void print_game_stats(struct game *g);          /* Prints the stats of the game, mostly for debugging purposes. */
void print_tmp_team(struct tmp_team *tmp);
