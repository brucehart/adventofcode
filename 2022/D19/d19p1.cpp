#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <limits>

/*
  Advent of Code 2022 - Day 19: Not Enough Minerals

  We want to determine, for each blueprint, the maximum number of geodes that
  can be opened in 24 minutes. Each blueprint is described by:

      Blueprint ID
      oreRobotOreCost
      clayRobotOreCost
      obsidianRobotOreCost, obsidianRobotClayCost
      geodeRobotOreCost, geodeRobotObsidianCost

  We start with:
    - 1 ore-collecting robot
    - 0 clay-collecting robots
    - 0 obsidian-collecting robots
    - 0 geode-cracking robots
    - 0 of each resource

  Each robot collects exactly 1 of its resource type per minute. Building a
  robot consumes resources and takes one minute to complete (the new robot is
  available at the end of that minute).

  We want to figure out the best sequence of robot builds to maximize the
  number of opened geodes by minute 24. Then, for each blueprint, the "quality
  level" is (blueprintID * maxOpenedGeodes). We sum up these quality levels
  for all blueprints.

  Approach:
    - We use a depth-limited search (DFS) with memoization and pruning.
    - State is defined by the minute, the quantities of resources, and the
      counts of each robot type.
    - At each state, we can decide to build any robot (if resources are
      sufficient) or do nothing. After deciding, we collect from the existing
      robots, move to the next minute, and so on.
    - We prune states if it's not possible to beat our global best with the
      time remaining.
*/

struct Blueprint {
    int id;
    int oreRobotOreCost;
    int clayRobotOreCost;
    int obsidianRobotOreCost;
    int obsidianRobotClayCost;
    int geodeRobotOreCost;
    int geodeRobotObsCost;
};

struct State {
    int minute;
    int ore;
    int clay;
    int obsidian;
    int geodes;
    int oreRobots;
    int clayRobots;
    int obsRobots;
    int geoRobots;
};

// Custom key for hashing State in an unordered_map.
struct StateHash {
    std::size_t operator()(const State& s) const {
        // A simple combination for hashing all fields
        // (you can use more sophisticated combinations if desired)
        // Here we do a standard "combine" approach:
        //    hash = ((hash << 5) + hash) ^ value
        // for each field
        std::size_t result = 0;
        auto combine = [&](int x) {
            result ^= std::hash<int>()(x) + 0x9e3779b97f4a7c15ULL +
                      (result << 6) + (result >> 2);
        };
        combine(s.minute);
        combine(s.ore);
        combine(s.clay);
        combine(s.obsidian);
        combine(s.geodes);
        combine(s.oreRobots);
        combine(s.clayRobots);
        combine(s.obsRobots);
        combine(s.geoRobots);
        return result;
    }
};

// Equality operator for State (needed by unordered_map).
struct StateEq {
    bool operator()(const State& a, const State& b) const {
        return a.minute      == b.minute      &&
               a.ore         == b.ore         &&
               a.clay        == b.clay        &&
               a.obsidian    == b.obsidian    &&
               a.geodes      == b.geodes      &&
               a.oreRobots   == b.oreRobots   &&
               a.clayRobots  == b.clayRobots  &&
               a.obsRobots   == b.obsRobots   &&
               a.geoRobots   == b.geoRobots;
    }
};

static const int MAX_MINUTES = 24;

/*
  Returns an upper bound on how many geodes we could possibly get if we
  built a geode-cracking robot in every remaining minute. This is used
  for pruning states that cannot possibly exceed our current best solution.
*/
int theoreticalMaxGeodes(int currentGeodes, int geoRobots, int minutesLeft) {
    // We already have 'currentGeodes'.
    // We produce 'geoRobots' new geodes next minute, and each minute we can add
    // at most one more geode-cracking robot (in an ideal scenario).
    // A quick over-estimate:
    //   next minute, we have geoRobots geodes,
    //   then geoRobots+1,
    //   then geoRobots+2, ...
    //   for 'minutesLeft' total minutes.

    // Summation of an arithmetic series:
    //   geoRobots + (geoRobots+1) + ... up to minutesLeft terms
    // We'll just do a direct small loop since minutesLeft <= 24.
    int total = currentGeodes;
    int currentGeoRobots = geoRobots;
    for (int i = 0; i < minutesLeft; ++i) {
        total += currentGeoRobots;
        // each minute, hypothetically, we build one more geode-robot
        // (this is an over-estimate, but good for pruning)
        currentGeoRobots++;
    }
    return total;
}

/*
  Memoized DFS to return the maximum number of geodes that can be opened
  starting from the given state, for the specified blueprint.

  We'll keep track of the best result found so far (globalBest) so that
  we can prune aggressively. The function returns the best possible number
  of geodes from the current state onward.
*/
int dfsMaxGeodes(
    const Blueprint& bp,
    State state,
    std::unordered_map<State, int, StateHash, StateEq>& memo,
    int& globalBest
) {
    // If we've reached the time limit, return how many geodes we have
    if (state.minute == MAX_MINUTES) {
        if (state.geodes > globalBest) {
            globalBest = state.geodes;
        }
        return state.geodes;
    }

    // Pruning: if the theoretical max is still not beating our globalBest, skip
    int minutesLeft = MAX_MINUTES - state.minute;
    if (theoreticalMaxGeodes(state.geodes, state.geoRobots, minutesLeft)
        <= globalBest) {
        return state.geodes;
    }

    // Check memo
    if (auto it = memo.find(state); it != memo.end()) {
        return it->second;
    }

    // We'll try all build options plus the "build nothing" option
    // 1) Build nothing (just collect)
    // 2) Build ore-robot (if enough ore)
    // 3) Build clay-robot (if enough ore)
    // 4) Build obsidian-robot (if enough ore and clay)
    // 5) Build geode-robot (if enough ore and obsidian)

    int best = 0;

    // We'll collect at the end of the minute after deciding what to build.
    // That means we first decide to spend resources, then we add in the
    // resource production from existing robots, then we increment minute.

    // Option 1: Don't build anything
    {
        State next = state;
        // Collect resources from existing robots
        next.ore      += state.oreRobots;
        next.clay     += state.clayRobots;
        next.obsidian += state.obsRobots;
        next.geodes   += state.geoRobots;
        next.minute++;
        best = std::max(best, dfsMaxGeodes(bp, next, memo, globalBest));
    }

    // Option 2: Build ore-robot if we can afford it
    if (state.ore >= bp.oreRobotOreCost) {
        State next = state;
        // Spend resources
        next.ore -= bp.oreRobotOreCost;
        // Collect
        next.ore      += state.oreRobots;
        next.clay     += state.clayRobots;
        next.obsidian += state.obsRobots;
        next.geodes   += state.geoRobots;
        // New robot is ready
        next.oreRobots++;
        next.minute++;
        best = std::max(best, dfsMaxGeodes(bp, next, memo, globalBest));
    }

    // Option 3: Build clay-robot if we can afford it
    if (state.ore >= bp.clayRobotOreCost) {
        State next = state;
        next.ore -= bp.clayRobotOreCost;
        // Collect
        next.ore      += state.oreRobots;
        next.clay     += state.clayRobots;
        next.obsidian += state.obsRobots;
        next.geodes   += state.geoRobots;
        // New robot
        next.clayRobots++;
        next.minute++;
        best = std::max(best, dfsMaxGeodes(bp, next, memo, globalBest));
    }

    // Option 4: Build obsidian-robot if we can afford it
    if (state.ore >= bp.obsidianRobotOreCost &&
        state.clay >= bp.obsidianRobotClayCost) {
        State next = state;
        next.ore   -= bp.obsidianRobotOreCost;
        next.clay  -= bp.obsidianRobotClayCost;
        // Collect
        next.ore      += state.oreRobots;
        next.clay     += state.clayRobots;
        next.obsidian += state.obsRobots;
        next.geodes   += state.geoRobots;
        // New robot
        next.obsRobots++;
        next.minute++;
        best = std::max(best, dfsMaxGeodes(bp, next, memo, globalBest));
    }

    // Option 5: Build geode-robot if we can afford it
    if (state.ore >= bp.geodeRobotOreCost &&
        state.obsidian >= bp.geodeRobotObsCost) {
        State next = state;
        next.ore      -= bp.geodeRobotOreCost;
        next.obsidian -= bp.geodeRobotObsCost;
        // Collect
        next.ore      += state.oreRobots;
        next.clay     += state.clayRobots;
        next.obsidian += state.obsRobots;
        next.geodes   += state.geoRobots;
        // New robot
        next.geoRobots++;
        next.minute++;
        best = std::max(best, dfsMaxGeodes(bp, next, memo, globalBest));
    }

    memo[state] = best;
    return best;
}

/*
  Computes the maximum number of geodes we can open in 24 minutes, given a blueprint.
*/
int maxOpenedGeodes(const Blueprint& bp) {
    // initial state
    State start{};
    start.minute     = 0;
    start.ore        = 0;
    start.clay       = 0;
    start.obsidian   = 0;
    start.geodes     = 0;
    start.oreRobots  = 1;  // We start with exactly 1 ore-collecting robot
    start.clayRobots = 0;
    start.obsRobots  = 0;
    start.geoRobots  = 0;

    std::unordered_map<State, int, StateHash, StateEq> memo;
    int globalBest = 0;
    return dfsMaxGeodes(bp, start, memo, globalBest);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<Blueprint> blueprints;

    /*
      Each line is expected to follow the pattern, for example:
        Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.

      We need to parse the blueprint ID, then extract the relevant costs.

      For safety, we'll parse in a more flexible manner:
      1) Read the entire line as a string
      2) Use string parsing (stringstream) to find the integers in the correct order
    */

    std::string line;
    while (true) {
        if (!std::getline(std::cin, line)) {
            break; // no more input
        }
        if (line.empty()) {
            continue; // skip empty lines if any
        }

        // Example line format we expect (all in one line):
        // Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore and 12 obsidian.
        // We'll just extract numbers in the order they appear:
        //   ID
        //   oreRobotOreCost
        //   clayRobotOreCost
        //   obsidianRobotOreCost, obsidianRobotClayCost
        //   geodeRobotOreCost, geodeRobotObsCost

        // We assume the input is well-formed. If it's not, we'd need more robust parsing.

        Blueprint bp{};
        {
            // We will look for "Blueprint <ID>:" first
            // Then the costs in order:
            //   "Each ore robot costs <x> ore."
            //   "Each clay robot costs <y> ore."
            //   "Each obsidian robot costs <a> ore and <b> clay."
            //   "Each geode robot costs <c> ore and <d> obsidian."

            // We'll do a simple approach: extract all integers in the order they appear
            std::stringstream ss(line);
            std::string tmp;
            std::vector<int> nums;
            while (ss >> tmp) {
                // If it's an integer, convert it
                try {
                    int val = std::stoi(tmp);
                    nums.push_back(val);
                } catch (...) {
                    // not a number
                }
            }
            // The order (assuming well-formed input) is:
            //   nums[0] => Blueprint ID
            //   nums[1] => oreRobotOreCost
            //   nums[2] => clayRobotOreCost
            //   nums[3] => obsidianRobotOreCost
            //   nums[4] => obsidianRobotClayCost
            //   nums[5] => geodeRobotOreCost
            //   nums[6] => geodeRobotObsCost
            if (nums.size() >= 7) {
                bp.id = nums[0];
                bp.oreRobotOreCost       = nums[1];
                bp.clayRobotOreCost      = nums[2];
                bp.obsidianRobotOreCost  = nums[3];
                bp.obsidianRobotClayCost = nums[4];
                bp.geodeRobotOreCost     = nums[5];
                bp.geodeRobotObsCost     = nums[6];
                blueprints.push_back(bp);
            }
        }
    }

    long long resultSum = 0;
    for (auto& bp : blueprints) {
        int bestGeodes = maxOpenedGeodes(bp);
        long long quality = static_cast<long long>(bp.id) * bestGeodes;
        resultSum += quality;
    }

    std::cout << resultSum << std::endl;
    return 0;
}
