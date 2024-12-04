#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// Represents a mapping from a source range to a destination range
struct RangeEntry {
    unsigned long destinationStart;
    unsigned long sourceStart;
    unsigned long elementCount;
};

// Represents a range with a starting point and its length
struct ValueRange {
    unsigned long start;
    unsigned long length;
};

// Holds all range entries for a particular map
struct MapData {
    std::vector<RangeEntry> entries;
};

/**
 * Parses a line containing seed ranges and populates the seeds vector.
 * Expected format: "seeds: start1 length1 start2 length2 ..."
 *
 * @param line The input line containing seed information.
 * @param seeds The vector to store parsed seed ValueRanges.
 */
void parseSeeds(const std::string& line, std::vector<ValueRange>& seeds) {
    // Find the position after the colon to start parsing
    std::size_t colonPos = line.find(':');
    if (colonPos == std::string::npos) return;

    std::istringstream iss(line.substr(colonPos + 1));
    ValueRange seed;
    
    // Read pairs of start and length
    while (iss >> seed.start >> seed.length) {
        seeds.emplace_back(seed);
    }
}

/**
 * Extracts the map name from a line containing "to-<mapName> map".
 *
 * @param line The input line containing the map name.
 * @return The extracted map name.
 */
std::string extractMapName(const std::string& line) {
    std::size_t startPos = line.find("to-");
    if (startPos == std::string::npos) return "";

    startPos += 3; // Move past "to-"
    std::size_t endPos = line.find(" map", startPos);
    if (endPos == std::string::npos) endPos = line.length();

    return line.substr(startPos, endPos - startPos);
}

/**
 * Parses map data from the input stream until an empty line is encountered.
 *
 * @param mapName The name of the map being parsed.
 * @param input The input stream to read map data from.
 * @param mapData The MapData object to populate with parsed RangeEntries.
 */
void parseMap(const std::string& mapName, std::istream& input, MapData& mapData) {
    std::string line;
    
    // Read lines until an empty line signifies the end of the map data
    while (std::getline(input, line) && !line.empty()) {
        std::istringstream iss(line);
        RangeEntry entry;
        
        // Parse destination start, source start, and number of elements
        if (iss >> entry.destinationStart >> entry.sourceStart >> entry.elementCount) {
            mapData.entries.emplace_back(entry);
        }
    }

    // Sort the entries by sourceStart for efficient lookup
    std::sort(mapData.entries.begin(), mapData.entries.end(),
              [](const RangeEntry& a, const RangeEntry& b) {
                  return a.sourceStart < b.sourceStart;
              });
}

/**
 * Looks up and maps a given ValueRange using the provided MapData.
 *
 * @param range The ValueRange to be mapped.
 * @param mapData The MapData containing range mappings.
 * @return A vector of ValueRanges after applying the map.
 */
std::vector<ValueRange> lookupMapRange(const ValueRange& range, const MapData& mapData) {
    std::vector<ValueRange> mappedRanges;
    ValueRange current = range;

    while (current.length > 0) {
        bool found = false;

        // Iterate through all range entries to find a matching source range
        for (const auto& entry : mapData.entries) {
            if (current.start >= entry.sourceStart &&
                current.start < entry.sourceStart + entry.elementCount) {
                
                // Calculate the mapped start position
                unsigned long mappedStart = entry.destinationStart + (current.start - entry.sourceStart);
                
                // Determine how many elements can be mapped in this entry
                unsigned long available = entry.elementCount - (current.start - entry.sourceStart);
                unsigned long mappedLength = std::min(available, current.length);
                
                // Add the mapped range to the result
                mappedRanges.push_back(ValueRange{mappedStart, mappedLength});
                
                // Update the current range
                current.start += mappedLength;
                current.length -= mappedLength;
                found = true;
                break;
            }
        }

        // If no mapping is found, retain the current range as is
        if (!found) {
            mappedRanges.push_back(current);
            break;
        }
    }

    return mappedRanges;
}

/**
 * Translates a seed ValueRange through a sequence of maps to determine final locations.
 *
 * @param seed The initial ValueRange seed.
 * @param maps The collection of all maps to apply in order.
 * @return A vector of ValueRanges representing the final locations.
 */
std::vector<ValueRange> rangeToLocations(const ValueRange& seed,
                                        const std::unordered_map<std::string, MapData>& maps) {
    // Define the order in which maps should be applied
    const std::vector<std::string> lookupOrder = {
        "soil", "fertilizer", "water", "light", "temperature", "humidity", "location"
    };
    
    std::vector<ValueRange> currentRanges = {seed};
    std::vector<ValueRange> nextRanges;

    // Apply each map in the specified order
    for (const auto& mapName : lookupOrder) {
        nextRanges.clear();
        
        // Apply the current map to all existing ranges
        for (const auto& range : currentRanges) {
            auto mapped = lookupMapRange(range, maps.at(mapName));
            nextRanges.insert(nextRanges.end(), mapped.begin(), mapped.end());
        }
        
        // Update the current ranges for the next map
        currentRanges = std::move(nextRanges);
    }

    return currentRanges;
}

int main() {
    std::vector<ValueRange> seeds;
    std::unordered_map<std::string, MapData> maps;
    std::vector<ValueRange> finalLocations;
    std::string line;

    // Read input line by line
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue; // Skip empty lines

        if (line.find("seeds:") == 0) {
            // Parse seed ranges
            parseSeeds(line, seeds);
        } 
        else if (line.find("map:") != std::string::npos) {
            // Extract map name and parse its data
            std::string mapName = extractMapName(line);
            if (mapName.empty()) continue;

            MapData mapData;
            parseMap(mapName, std::cin, mapData);
            maps.emplace(mapName, std::move(mapData));
        }
    }

    // Process each seed to determine final locations
    for (const auto& seed : seeds) {
        auto locations = rangeToLocations(seed, maps);
        finalLocations.insert(finalLocations.end(), locations.begin(), locations.end());
    }

    // Sort the final locations by their start value
    std::sort(finalLocations.begin(), finalLocations.end(),
              [](const ValueRange& a, const ValueRange& b) {
                  return a.start < b.start;
              });

    // Output the start of the first location if available
    if (!finalLocations.empty()) {
        std::cout << finalLocations.front().start << std::endl;
    } else {
        std::cout << "No locations found." << std::endl;
    }

    return 0;
}
