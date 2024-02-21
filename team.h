

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>


/* Contributing to a classless society by using structs instead. */
struct team {
    std::string name;
    std::vector<struct player*> players;
    std::map<std::string, float> stats;
};

struct team *init_team(std::string team_name);

/* Checks if a given player plays for a given team. NOT collision resistant! */
bool player_in_team(struct player* p, struct team* t);

void add_new_player(struct player* p, struct team* t);
void calculate_stats(struct team* t);
struct team *parse_team(std::string filename);
void print_team(struct team* t);

