#include <iostream>
#include <string>
#include <vector>
#include <map>


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

    /* Initialize and fill LHL. */
    struct league lhl;
    

    // char *l = setlocale(LC_ALL, NULL);
    // std::cout << "Current locale: " << l << std::endl;

    /* Parse game. */
    struct game *g = parse_game("arsenal_arno.txt");
    struct team *t = parse_team("teams/arno.txt");
    // std::cout << g->events.at(63)->ass1 << std::endl;
    
    print_team(t);

    return 0;
}

