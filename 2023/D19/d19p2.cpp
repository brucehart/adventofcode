#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cstdint>  // Add this include

struct Range {
    int64_t min, max;
    
    int64_t size() const {
        return max >= min ? max - min + 1 : 0;
    }
};

struct RatingRanges {
    Range x{1, 4000};
    Range m{1, 4000};
    Range a{1, 4000};
    Range s{1, 4000};
    
    int64_t combinations() const {
        return x.size() * m.size() * a.size() * s.size();
    }
};

struct Rule {
    char category;
    char operation;
    int64_t value;  // int changed to int64_t
    std::string destination;
};

struct Workflow {
    std::vector<Rule> rules;
    std::string defaultDest;
};

Workflow parseWorkflow(const std::string& line) {
    Workflow w;
    size_t bracePos = line.find('{');
    std::string rulesStr = line.substr(bracePos + 1, line.length() - bracePos - 2);
    std::stringstream ss(rulesStr);
    std::string ruleStr;
    
    while (std::getline(ss, ruleStr, ',')) {
        if (ruleStr.find(':') == std::string::npos) {
            w.defaultDest = ruleStr;
        } else {
            Rule r;
            r.category = ruleStr[0];
            r.operation = ruleStr[1];
            size_t colonPos = ruleStr.find(':');
            r.value = static_cast<int64_t>(std::stoi(ruleStr.substr(2, colonPos - 2)));
            r.destination = ruleStr.substr(colonPos + 1);
            w.rules.push_back(r);
        }
    }
    return w;
}

int64_t processRanges(const std::map<std::string, Workflow>& workflows, 
                      const std::string& currentFlow, 
                      RatingRanges ranges) {
    if (currentFlow == "A") return ranges.combinations();
    if (currentFlow == "R") return 0;
    
    const Workflow& workflow = workflows.at(currentFlow);
    int64_t total = 0;
    
    for (const Rule& rule : workflow.rules) {
        RatingRanges matchedRanges = ranges;
        RatingRanges unmatchedRanges = ranges;
        Range* matchedRange;
        Range* unmatchedRange;
        
        // Select which range to modify based on category
        switch (rule.category) {
            case 'x': 
                matchedRange = &matchedRanges.x;
                unmatchedRange = &unmatchedRanges.x;
                break;
            case 'm': 
                matchedRange = &matchedRanges.m;
                unmatchedRange = &unmatchedRanges.m;
                break;
            case 'a': 
                matchedRange = &matchedRanges.a;
                unmatchedRange = &unmatchedRanges.a;
                break;
            case 's': 
                matchedRange = &matchedRanges.s;
                unmatchedRange = &unmatchedRanges.s;
                break;
            default: continue;
        }
        
        // Split range based on operation
        if (rule.operation == '<') {
            matchedRange->max = std::min(matchedRange->max, rule.value - 1);
            unmatchedRange->min = std::max(unmatchedRange->min, rule.value);
        } else { // '>'
            matchedRange->min = std::max(matchedRange->min, rule.value + 1);
            unmatchedRange->max = std::min(unmatchedRange->max, rule.value);
        }
        
        // Process matching range
        total += processRanges(workflows, rule.destination, matchedRanges);
        ranges = unmatchedRanges;
    }
    
    // Process default destination with remaining ranges
    return total + processRanges(workflows, workflow.defaultDest, ranges);
}

int main() {
    std::map<std::string, Workflow> workflows;
    std::string line;
    
    // Parse workflows until empty line
    while (std::getline(std::cin, line) && !line.empty()) {
        size_t bracePos = line.find('{');
        std::string name = line.substr(0, bracePos);
        workflows[name] = parseWorkflow(line);
    }
    
    // Start processing from "in" workflow with full ranges
    RatingRanges initial;
    int64_t result = processRanges(workflows, "in", initial);
    
    std::cout << result << std::endl;
    return 0;
}
