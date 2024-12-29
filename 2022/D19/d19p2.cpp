#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <limits>

/*
  Advent of Code 2022 - Day 19 (Part 2): Not Enough Minerals

  Now we have 32 minutes instead of 24, and only the first three blueprints
  remain. We want to find, for each of these three blueprints, the maximum
  number of geodes that can be opened in 32 minutes, then multiply those
  three maxima together.

  The mechanics are the same as before, except we use a 32-minute limit and
  only consider the first 3 blueprints:

    - We still have:
      1 ore-collecting robot to start,
      0 clay-collecting robots,
      0 obsidian-collecting robots,
      0 geode-cracking robots,
      0 of each resource.

    - Each build action consumes the appropriate resources and takes a minute,
      after which the new robot is available.

    - The goal is to figure out which sequence of builds yields the most
      geodes opened by minute 32.

  We'll use a depth-limited search (DFS) with memoization and pruning, similar
  to part 1, but updated for a 32-minute limit. Then we only multiply the best
  results from the first three blueprints.
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

// Custom hash for State used in memoization
struct StateHash {
    std::size_t operator()(const State& s) const {
        std::size_t result = 0;
        auto combine = [&](int x) {
            // A typical way to combine hashes
            result ^= std::hash<int>()(x) + 0x9e3779b97f4a7c15ULL
                      + (result << 6) + (result >> 2);
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

// Equality operator for State
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

/*
  Returns an upper bound on how many geodes we could possibly get if we
  built a geode-cracking robot in every remaining minute (an overestimate
  for pruning).
*/
int theoreticalMaxGeodes(int currentGeodes, int geoRobots, int minutesLeft) {
    // Summation approach:
    //   next minute: +geoRobots
    //   following minute: +(geoRobots+1)
    //   ...
    int total = currentGeodes;
    int currentGeoRobots = geoRobots;
    for (int i = 0; i < minutesLeft; ++i) {
        total += currentGeoRobots;
        // Hypothetically build one new geode robot every minute
        currentGeoRobots++;
    }
    return total;
}

/*
  DFS with memoization to compute maximum geodes for a given blueprint and
  time limit. Returns the best possible number of geodes from the current
  state onward.
*/
int dfsMaxGeodes(
    const Blueprint& bp,
    const State& state,
    int timeLimit,
    std::unordered_map<State, int, StateHash, StateEq>& memo,
    int& globalBest
) {
    // Base case: if we've used up the allotted time
    if (state.minute == timeLimit) {
        if (state.geodes > globalBest) {
            globalBest = state.geodes;
        }
        return state.geodes;
    }

    // Pruning with theoretical maximum
    int minutesLeft = timeLimit - state.minute;
    if (theoreticalMaxGeodes(state.geodes, state.geoRobots, minutesLeft)
        <= globalBest) {
        return state.geodes;
    }

    // Check memo
    auto it = memo.find(state);
    if (it != memo.end()) {
        return it->second;
    }

    // Try up to five actions:
    //   1) Build nothing
    //   2) Build ore robot (if affordable)
    //   3) Build clay robot (if affordable)
    //   4) Build obsidian robot (if affordable)
    //   5) Build geode-cracking robot (if affordable)

    int best = 0;

    // 1) Build nothing
    {
        State next = state;
        // Collect resources
        next.ore      += state.oreRobots;
        next.clay     += state.clayRobots;
        next.obsidian += state.obsRobots;
        next.geodes   += state.geoRobots;
        next.minute++;
        best = std::max(best, dfsMaxGeodes(bp, next, timeLimit, memo, globalBest));
    }

    // 2) Build ore-robot
    if (state.ore >= bp.oreRobotOreCost) {
        State next = state;
        next.ore -= bp.oreRobotOreCost;
        // Collect resources
        next.ore      += state.oreRobots;
        next.clay     += state.clayRobots;
        next.obsidian += state.obsRobots;
        next.geodes   += state.geoRobots;
        // New robot
        next.oreRobots++;
        next.minute++;
        best = std::max(best, dfsMaxGeodes(bp, next, timeLimit, memo, globalBest));
    }

    // 3) Build clay-robot
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
        best = std::max(best, dfsMaxGeodes(bp, next, timeLimit, memo, globalBest));
    }

    // 4) Build obsidian-robot
    if (state.ore >= bp.obsidianRobotOreCost &&
        state.clay >= bp.obsidianRobotClayCost) {
        State next = state;
        next.ore  -= bp.obsidianRobotOreCost;
        next.clay -= bp.obsidianRobotClayCost;
        // Collect
        next.ore      += state.oreRobots;
        next.clay     += state.clayRobots;
        next.obsidian += state.obsRobots;
        next.geodes   += state.geoRobots;
        // New robot
        next.obsRobots++;
        next.minute++;
        best = std::max(best, dfsMaxGeodes(bp, next, timeLimit, memo, globalBest));
    }

    // 5) Build geode-robot
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
        best = std::max(best, dfsMaxGeodes(bp, next, timeLimit, memo, globalBest));
    }

    memo[state] = best;
    return best;
}

/*
  Computes the maximum number of geodes we can open in `timeLimit` minutes
  using the given blueprint.
*/
int maxOpenedGeodes(const Blueprint& bp, int timeLimit) {
    // Initial state
    State start{};
    start.minute     = 0;
    start.ore        = 0;
    start.clay       = 0;
    start.obsidian   = 0;
    start.geodes     = 0;
    start.oreRobots  = 1;  // We start with 1 ore-collecting robot
    start.clayRobots = 0;
    start.obsRobots  = 0;
    start.geoRobots  = 0;

    // Memo for DFS
    std::unordered_map<State, int, StateHash, StateEq> memo;
    int globalBest = 0;

    return dfsMaxGeodes(bp, start, timeLimit, memo, globalBest);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<Blueprint> blueprints;
    std::string line;

    /*
      Expected input format (same as part 1):
        Blueprint <ID>: Each ore robot costs <x> ore. Each clay robot costs <y> ore. ...
      We only care about the first three lines (the first three blueprints).
    */

    while (true) {
        if (!std::getline(std::cin, line)) {
            break; // end of input
        }
        if (line.empty()) {
            continue;
        }

        // Parse the line for numbers in the assumed order
        std::stringstream ss(line);
        std::string tmp;
        std::vector<int> nums;
        while (ss >> tmp) {
            try {
                int val = std::stoi(tmp);
                nums.push_back(val);
            } catch (...) {
                // Not a number
            }
        }

        if (nums.size() >= 7) {
            Blueprint bp{};
            bp.id                     = nums[0];
            bp.oreRobotOreCost       = nums[1];
            bp.clayRobotOreCost      = nums[2];
            bp.obsidianRobotOreCost  = nums[3];
            bp.obsidianRobotClayCost = nums[4];
            bp.geodeRobotOreCost     = nums[5];
            bp.geodeRobotObsCost     = nums[6];
            blueprints.push_back(bp);
        }
    }

    // We only consider the first 3 blueprints
    // Compute the max geodes for each (in 32 minutes), then multiply
    long long product = 1;
    int countUsed = 0;

    for (std::size_t i = 0; i < blueprints.size() && i < 3; ++i) {
        int bestGeodes = maxOpenedGeodes(blueprints[i], 32);
        product *= bestGeodes;
        ++countUsed;
    }

    // If there were fewer than 3 blueprints in input, product is over fewer ones,
    // but the puzzle states that we only need the first three if they exist.
    if (countUsed > 0) {
        std::cout << product << std::endl;
    } else {
        std::cout << 0 << std::endl;
    }

    return 0;
}

