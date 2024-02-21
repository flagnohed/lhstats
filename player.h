#include <string>
#include <map>

typedef enum {GOALIE=0, DEFENCEMAN=1, FORWARD=2} POSITION;

struct player {
    std::string name;
    std::map<std::string, float> stats;

    POSITION pos;
};