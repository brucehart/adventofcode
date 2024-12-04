#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

struct RangeEntry {
    unsigned long destinationRangeStart;
    unsigned long sourceRangeStart;
    unsigned long numElements;
};

struct MapData {
    std::vector<RangeEntry> entries;
};

void parseSeeds(const std::string& line, std::vector<unsigned long>& seeds) {
    std::istringstream iss(line.substr(line.find(":") + 1));
    unsigned long seed;
    while (iss >> seed) {
        seeds.push_back(seed);
    }
}

std::string extractMapName(const std::string& line) {
    std::size_t startPos = line.find("to-") + 3; // Skip "to-"
    std::size_t endPos = line.find(" map");
    return line.substr(startPos, endPos - startPos);
}

void parseMap(const std::string& name, std::istream& input, MapData& mapData) {
    std::string line;
    while (std::getline(input, line) && !line.empty()) {
        std::istringstream iss(line);
        RangeEntry entry;
        iss >> entry.destinationRangeStart >> entry.sourceRangeStart >> entry.numElements;
        mapData.entries.push_back(entry);
    }
}

unsigned long lookupMap(unsigned long n, const MapData& mapData){
    for (const RangeEntry& entry : mapData.entries) {
        if (n >= entry.sourceRangeStart && n < entry.sourceRangeStart + entry.numElements) {
            return entry.destinationRangeStart + (n - entry.sourceRangeStart);
        }
    }
    return n;
}

unsigned long seedToLocation(unsigned long seed, const std::unordered_map<std::string, MapData>& maps) {
    std::vector<std::string> lookupOrder = {"soil","fertilizer","water","light","temperature","humidity","location"};
    unsigned long n = seed;
    for (const std::string& mapName : lookupOrder) {
        n = lookupMap(n, maps.at(mapName));
    }
    return n;    
}

int main() {
    std::vector<unsigned long> seeds;
    std::unordered_map<std::string, MapData> maps;
    
    std::vector<unsigned long> locations;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.find("seeds:") == 0) {
            parseSeeds(line, seeds);
        } else if (line.find("map:") != std::string::npos) {
            std::string mapName = extractMapName(line);
            MapData mapData;
            parseMap(mapName, std::cin, mapData);
            maps[mapName] = mapData;
        }
    }

    std::for_each(seeds.begin(), seeds.end(), [&](unsigned long seed) {
        locations.push_back(seedToLocation(seed, maps));
    });

    std::sort(locations.begin(), locations.end());

    std::cout << locations[0] << std::endl;

    return 0;
}
