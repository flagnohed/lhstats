#include <string>
#include "team.h"
#include "player.h"
#include <fstream>


struct team *init_team(std::string team_name)  {
    struct team *t = new team;
    t->name = team_name;
    return t;
}

bool player_in_team(struct player* p, struct team* t) {
    for (auto* ptemp : t->players) {
        if (ptemp->name == p->name && ptemp->pos == p->pos)
            /* Probably the player we are looking for. */ 
            return true;
    }
    return false;
}

struct team *parse_team(std::string filename) {
    std::ifstream ifs(filename);
    std::string line;
    struct team *t = new team;
    if (!ifs.is_open()) {
        std::cerr << "Could not open file." << std::endl;
        exit(1);
    }

    /* 0 = goalie, 1 = def, 2 = fwd*/
    int pos = 0;

    while (std::getline(ifs, line)) {

        if (line.empty()) {
            pos++;
            continue;
        }
            

        struct player *p = new player;
        /* When we have seen 2 spaces, we have both first- and lastname. */
        int spaces = 0;

        for (std::size_t i{}; i < line.size(); i++) {
            if (line[i] == ' ') 
                spaces++;
            
            if (spaces == 2) {
                p->name = line.substr(0, i);
                p->pos = static_cast<POSITION>(pos);
                t->players.push_back(p);
                break;
            }
        }
    }
    return t;
}

void print_team(struct team* t) {
    std::cout << t->name << std::endl;
    for (auto *p : t->players) 
        std::cout <<p->pos << " " << p->name << std::endl;
}