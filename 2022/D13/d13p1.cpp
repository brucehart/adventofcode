#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

// Structure to represent a Packet, which can be either an integer or a list of Packets
struct Packet {
    std::variant<int, std::vector<Packet>> value;
};

// Function to parse a packet from a string starting at position 'pos'
Packet parse_packet(const std::string& s, size_t& pos);

// Helper function to parse a list
std::vector<Packet> parse_list(const std::string& s, size_t& pos) {
    std::vector<Packet> list;
    // Assume that the current character is '['
    pos++; // Skip '['
    while (pos < s.size() && s[pos] != ']') {
        // Parse each element in the list
        list.emplace_back(parse_packet(s, pos));
        // If the next character is ',', skip it
        if (pos < s.size() && s[pos] == ',') {
            pos++;
        }
    }
    pos++; // Skip ']'
    return list;
}

// Function to parse a single packet (integer or list)
Packet parse_packet(const std::string& s, size_t& pos) {
    Packet packet;
    if (s[pos] == '[') {
        // It's a list
        packet.value = parse_list(s, pos);
    } else {
        // It's an integer, possibly negative and with multiple digits
        int num = 0;
        bool negative = false;
        if (s[pos] == '-') {
            negative = true;
            pos++;
        }
        while (pos < s.size() && std::isdigit(s[pos])) {
            num = num * 10 + (s[pos] - '0');
            pos++;
        }
        if (negative) {
            num = -num;
        }
        packet.value = num;
    }
    return packet;
}

// Function to compare two packets
// Returns -1 if left < right, 0 if equal, 1 if left > right
int compare_packets(const Packet& left, const Packet& right) {
    // If both are integers
    if (std::holds_alternative<int>(left.value) && std::holds_alternative<int>(right.value)) {
        int l = std::get<int>(left.value);
        int r = std::get<int>(right.value);
        if (l < r) return -1;
        if (l > r) return 1;
        return 0;
    }
    
    // If both are lists
    if (std::holds_alternative<std::vector<Packet>>(left.value) && std::holds_alternative<std::vector<Packet>>(right.value)) {
        const auto& l_list = std::get<std::vector<Packet>>(left.value);
        const auto& r_list = std::get<std::vector<Packet>>(right.value);
        size_t min_size = std::min(l_list.size(), r_list.size());
        for (size_t i = 0; i < min_size; ++i) {
            int cmp = compare_packets(l_list[i], r_list[i]);
            if (cmp != 0) return cmp;
        }
        if (l_list.size() < r_list.size()) return -1;
        if (l_list.size() > r_list.size()) return 1;
        return 0;
    }
    
    // If one is integer and the other is list, convert the integer to a list and compare
    if (std::holds_alternative<int>(left.value)) {
        // Convert left to list
        Packet converted_left;
        converted_left.value = std::vector<Packet>{Packet{std::get<int>(left.value)}};
        return compare_packets(converted_left, right);
    } else {
        // Convert right to list
        Packet converted_right;
        converted_right.value = std::vector<Packet>{Packet{std::get<int>(right.value)}};
        return compare_packets(left, converted_right);
    }
}

int main() {
    std::vector<std::pair<Packet, Packet>> pairs;
    std::string line;
    std::vector<std::string> lines;
    
    // Read all lines from stdin
    while (std::getline(std::cin, line)) {
        lines.emplace_back(line);
    }
    
    // Group lines into pairs, separated by blank lines
    for (size_t i = 0; i < lines.size(); ) {
        if (lines[i].empty()) {
            i++;
            continue;
        }
        // First packet
        std::string first = lines[i++];
        // Check if there's a second packet
        if (i >= lines.size()) break;
        std::string second = lines[i++];
        // Parse first packet
        size_t pos1 = 0;
        Packet pkt1 = parse_packet(first, pos1);
        // Parse second packet
        size_t pos2 = 0;
        Packet pkt2 = parse_packet(second, pos2);
        // Add to pairs
        pairs.emplace_back(std::make_pair(pkt1, pkt2));
    }
    
    // Compare each pair and sum the indices of those in the right order
    int sum = 0;
    for (size_t i = 0; i < pairs.size(); ++i) {
        int cmp = compare_packets(pairs[i].first, pairs[i].second);
        if (cmp == -1) { // First packet is less than second
            sum += static_cast<int>(i + 1); // Indices are 1-based
        }
    }
    
    std::cout << sum << std::endl;
    return 0;
}

