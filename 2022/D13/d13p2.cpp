#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

// Structure to represent a Packet, which can be either an integer or a list of Packets
struct Packet {
    std::variant<int, std::vector<struct Packet>> value;

    // Equality operator to compare two packets
    bool operator==(const Packet& other) const {
        return value == other.value;
    }
};

// Forward declaration of parse_packet
Packet parse_packet(const std::string& s, size_t& pos);

// Helper function to parse a list from the string starting at position 'pos'
std::vector<Packet> parse_list(const std::string& s, size_t& pos) {
    std::vector<Packet> list;
    pos++; // Skip the '[' character
    while (pos < s.size() && s[pos] != ']') {
        // Parse each element in the list
        list.emplace_back(parse_packet(s, pos));
        // If the next character is ',', skip it
        if (pos < s.size() && s[pos] == ',') {
            pos++;
        }
    }
    pos++; // Skip the ']' character
    return list;
}

// Function to parse a single packet (integer or list) from the string starting at position 'pos'
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
    std::vector<Packet> packets;
    std::string line;

    // Read all non-empty lines from stdin and parse them into packets
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue; // Disregard blank lines
        }
        size_t pos = 0;
        Packet pkt = parse_packet(line, pos);
        packets.emplace_back(pkt);
    }

    // Define the two divider packets [[2]] and [[6]]
    // We'll parse them using the same parse_packet function
    std::string divider1_str = "[[2]]";
    size_t pos1 = 0;
    Packet divider1 = parse_packet(divider1_str, pos1);

    std::string divider2_str = "[[6]]";
    size_t pos2 = 0;
    Packet divider2 = parse_packet(divider2_str, pos2);

    // Add the divider packets to the list
    packets.emplace_back(divider1);
    packets.emplace_back(divider2);

    // Define a comparator for std::sort using the compare_packets function
    auto comparator = [](const Packet& a, const Packet& b) -> bool {
        return compare_packets(a, b) == -1;
    };

    // Sort all packets
    std::sort(packets.begin(), packets.end(), comparator);

    // Find the indices (1-based) of the divider packets
    int index1 = 0, index2 = 0;
    for (size_t i = 0; i < packets.size(); ++i) {
        if (packets[i] == divider1) {
            index1 = static_cast<int>(i) + 1; // 1-based index
        }
        if (packets[i] == divider2) {
            index2 = static_cast<int>(i) + 1; // 1-based index
        }
    }

    // Calculate the decoder key
    long long decoder_key = static_cast<long long>(index1) * static_cast<long long>(index2);

    // Output the decoder key
    std::cout << decoder_key << std::endl;
    return 0;
}

