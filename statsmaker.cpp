#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <clocale>

#include "game.h"
#include "team.h"
#include "player.h"


std::vector<std::string> TEAM_NAMES = {              \
    "Arnö Hockey", "Qwick Silver", "Real Östersund", \
    "Cluben HC", "Lich Lightning", "Jämtland HC",    \
    "HV 71", "Jaeger", "VRÅNGÖ HADDOCKS", "Arsenal", \
    "Älmhult if", "Young Guns"};


struct league {
    std::vector<game*> games{};
    std::vector<team*> teams{};
};


int main(int argc, char *argv[]) {

    std::setlocale(LC_ALL, "sv_SE.UTF-8");
    /* Initialize and fill LHL. */
    struct league lhl;
    
    struct team *arno = parse_team("teams/arno.txt");
    struct team *arsenal = parse_team("teams/arsenal.txt");
    struct game *g = parse_game("games/arsenal_arno.txt");
    
    print_game_stats(g);
    print_team(arno);
    print_team(arsenal);

    return 0;
}

