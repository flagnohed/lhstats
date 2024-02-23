#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include "game.h"
#include <cassert>
#include <algorithm>

/* Parses the first line of the file, saving goals and team names in a game struct. */
struct game *init_game(std::string line) {
    struct game *g = new game;
    struct tmp_team *ht = new tmp_team;
    struct tmp_team *at = new tmp_team;
 
    /* Keep track of where we are during parsing. */
    std::size_t idx{};
    
    std::size_t dash_idx = line.find_first_of('-');
    ht->name = line.substr(0, dash_idx - 1);

    /* Skip " - ". */
    idx += ht->name.size() + SKIP_THREE;
    
    /* There are two whitespace after away team. */
    std::size_t away_team_end = line.find_first_of("  "); 
    std::size_t at_len = idx + away_team_end - SKIP_THREE - 2;
    at->name = line.substr(idx, at_len);
    idx += at_len + SKIP_TWO;

    /* Found the result! Parse it. */
    if (isdigit(line[idx])) {
        ht->goals = line[idx] - '0';
        idx += 2*SKIP_TWO;  // skip ht->goals + " - "
        at->goals = line[idx] - '0';
    }

    idx += SKIP_THREE;  // skip at->goals + " ("

    /* Get goals per team and period. */
    int p = 0;

    while (idx < line.size()) {
        p++;

        ht->goals_per_period.push_back(line[idx] - '0');
        idx += SKIP_TWO;
        at->goals_per_period.push_back(line[idx] - '0');
        
        if (p == 3)
            break;
        
        idx += SKIP_THREE;
    } /* Done parsing the line. */
    g->ht = ht;
    g->at = at;
    return g;
}

void parse_event(struct event *e, std::string const& line) {
    std::size_t idx{}, pos_open{}, pos_closed{};
    
    /* Add time to struct event. */
    int minutes = std::stoi(line.substr(1, 2));
    int seconds = std::stoi(line.substr(4, 2));
    e->time = std::pair<int, int>{minutes, seconds};
    
    idx += LEN_TIME_STAMP + 1;

    /* Get event type. */
    std::string event_type = line.substr(idx, LEN_EVENT_TYPE);
    idx += LEN_EVENT_TYPE + 1;

    // struct player p;
    std::string rest = line.substr(idx, line.size() - idx);
    idx = 0;  // reset idx relative to rest instead of line.
    
    if (event_type == "UTV") {
        /* Penalties have (penalty type) before name, so we need to parse that first. */
        e->event_type = ETYPE::PENALTY;
        idx++;  // skip open paren
        pos_closed = rest.find_first_of(CLOSED_PAREN);
        e->penalty_type = rest.substr(idx, pos_closed - 1);
        idx += e->penalty_type.size() + 2;

        // remove penalty type from rest
        rest = line.substr(idx, rest.size() - idx);
        idx = 0;
    }

    /* Get player name. */
    pos_open = rest.find_first_of(OPEN_PAREN);
    e->player_name = rest.substr(0, pos_open - 1);
    idx += pos_open + 1;
    rest = rest.substr(idx, rest.size() - idx);
    idx = 0;

    if (isdigit(event_type[0]) && event_type[1] == '-' && isdigit(event_type[2])) {
        /* Goal! Save score and involved players' name. */
        e->event_type = ETYPE::GOAL;
        e->score = std::pair<int, int>{event_type[0] - '0', event_type[2] - '0'};

        /* Handle special teams. */
        if (rest[0] == rest[1] == 'P' && rest[2] == CLOSED_PAREN) {
            e->goal_format = GFORMAT::PP;
            idx += 5;  // "PP) ("
        } else if (rest[0] == 'B' && rest[1] == 'P' && rest[2] == CLOSED_PAREN) {
            e->goal_format = GFORMAT::BP;
            idx = 5;
        } else {
            e->goal_format = GFORMAT::ES;
        }

        /* Check for assists. In Livehockey, there cannot be 0 assists, 
            its either 1 or 2. Idx is currently pointing at the name of first assist. */
        
        std::string assist_str = rest.substr(idx, rest.size() - 1 - idx);

        std::string::size_type n = assist_str.find(',');
        if (n == std::string::npos) {
            /* Single assist. */
            e->ass1 = assist_str;
        } else {
            /* Two assists. */
            e->ass1 = assist_str.substr(0, n);
            e->ass2 = assist_str.substr(n + 2, assist_str.size() - n - 2);
        }
        /* Goals do not disclose team abbreviation. */
        return;
    }
    
    if (event_type == "SOG") 
        e->event_type = ETYPE::SHOT;

    /* Parse team abbreviation. */
    e->team_short = rest.substr(0, 3);

    #if 0
    std::cerr << "Unrecognized event type. " << std::endl;
    exit(EXIT_FAILURE);
    #endif


}


struct game *parse_game(std::string filename) {
    std::ifstream ifs(filename);
    std::string line;
    
    if (!ifs.is_open()) {
        std::cerr << "Could not open file." << std::endl;
        exit(1);
    }

    /* Process first line in file, which contains team names and goals. */

    struct game *g = new game;
    bool home_team_parsed = false;
    int row = 0;

    while (std::getline(ifs, line)) {
        /* This if else if shit is disgusting, how can it be changed? */

        if (line[0] == OPEN_PAREN) {
            /* This line is an event. */
            struct event *e = new event;
            parse_event(e, line);
            g->events.push_back(e);
        }

        /* Handle the headline. */
        else if (!row) { g = init_game(line); row++; continue; }
            
        /* Get game-date. */
        else if (line.substr(0, MATCH_DATE.size()) == MATCH_DATE) 
            g->date = line.substr(MATCH_DATE.size(), line.size() - MATCH_DATE.size());
        
        /* Get game-id. */
        else if (line.substr(0, MATCH_ID.size()) == MATCH_ID) 
            g->id = line.substr(MATCH_ID.size(), line.size() - MATCH_ID.size());
        
        /* Get arena. */
        else if (line.substr(0, ARENA_NAME.size()) == ARENA_NAME) 
            g->arena = line.substr(ARENA_NAME.size(), line.size() - ARENA_NAME.size());
        
        else if (line == g->at->name) {
            /* From here it will be the away team's info. */
            home_team_parsed = true;
        }

        else if (line.substr(0, TEAM_GRADE.size()) == TEAM_GRADE) {

            std::string s = line.substr(TEAM_GRADE.size(), line.size() - TEAM_GRADE.size());
            /* If team grade >= 1000 its displayed as e.g. '1 000', so remove that whitespace before int conversion. */
            s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
            int grade = std::stoi(s);
            home_team_parsed ? g->at->grade = grade : g->ht->grade = grade;
        }

        else if (line.substr(0, ICE_TIME.size()) == ICE_TIME) {
            line = line.substr(ICE_TIME.size(), line.size() - ICE_TIME.size());

            std::string tmp = "";
            int current_line = 1;   /* Line as in 5 players, not line in a file. */
            std::size_t space_idx = line.find(' ');
            std::cout << line << std::endl;
            for (char c : line) {
                if (c == '-' && current_line <= 3) {
                    home_team_parsed ? g->at->ice_time.push_back(std::stoi(tmp)) : g->ht->ice_time.push_back(std::stoi(tmp));
                    
                    tmp = "";   /* Reset tmp. */
                    current_line++;
                    continue;
                } else if (c == ' ') 
                    break;  /* We have parsed the ice time for each line, but we still need to see order ( e.g. (1-2-3) ). */

                tmp += c;
            }
            /* Line 3 does not have a following '-', handle that. */
            home_team_parsed ? g->at->ice_time.push_back(std::stoi(tmp)) : g->ht->ice_time.push_back(std::stoi(tmp));

            /* Handle cases where ice time refers to other line sequence than default. */
            line = line.substr(space_idx + 2, line.size() - (space_idx + 2) - 1);
            int digits_seen = 0;
            for (char c : line) {
                if (c == '-') 
                    continue;
                /* Digit! */
                int cval = c - '0';
                if (cval == ++digits_seen)
                    continue;   /* We do not need to swap anything, it is in the "correct" spot. */

                /* If we are here, it means that line cval is in the wrong spot. Swap it. */
                if (home_team_parsed) {
                    std::swap(g->at->ice_time[digits_seen - 1], g->at->ice_time[cval - 1]);
                } else {
                    std::swap(g->ht->ice_time[digits_seen - 1], g->ht->ice_time[cval - 1]);
                }
            }
            assert(digits_seen == 3);
        }
        
    
    }
    return g;
}

void print_game_stats(struct game *g) {
    using namespace std;
    
    cout << "Arena: " << g->arena << endl;
    cout << "Datum: " << g->date << endl;
    cout << "Id: " << g->id << endl << endl;
    
    print_tmp_team(g->ht);
    print_tmp_team(g->at);
}

void print_tmp_team(struct tmp_team *tmp) {
    using namespace std;
    cout << tmp->name << ": " << tmp->goals << endl;
    cout << "Periodsiffror: ";
    for (auto pg : tmp->goals_per_period) 
        cout << pg << " ";
    cout << endl;

    cout << "Istid: " << endl;
    for (auto it : tmp->ice_time) 
        cout << it << " ";
    cout << endl;

    cout << "Lagbetyg: " << tmp->grade << endl << endl;
    
}





