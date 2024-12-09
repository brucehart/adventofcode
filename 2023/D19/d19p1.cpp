#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <numeric>

struct Part {
    int x, m, a, s;
    int sum() const { return x + m + a + s; }
};

struct Rule {
    char category;     // x, m, a, s
    char operation;    // <, >
    int value;
    std::string destination;
};

struct Workflow {
    std::string name;
    std::vector<Rule> rules;
    std::string defaultDest;
};

// Parse a single rule like "x>10:one"
Rule parseRule(const std::string& rule) {
    Rule r;
    size_t colonPos = rule.find(':');
    if (colonPos == std::string::npos) {
        r.destination = rule;
        return r;
    }
    
    r.category = rule[0];
    r.operation = rule[1];
    r.value = std::stoi(rule.substr(2, colonPos - 2));
    r.destination = rule.substr(colonPos + 1);
    return r;
}

// Parse workflow string like "px{a<2006:qkq,m>2090:A,rfg}"
Workflow parseWorkflow(const std::string& line) {
    Workflow w;
    size_t bracePos = line.find('{');
    w.name = line.substr(0, bracePos);
    
    std::string rulesStr = line.substr(bracePos + 1, line.length() - bracePos - 2);
    std::stringstream ss(rulesStr);
    std::string ruleStr;
    
    while (std::getline(ss, ruleStr, ',')) {
        if (ruleStr.find(':') == std::string::npos) {
            w.defaultDest = ruleStr;
        } else {
            w.rules.push_back(parseRule(ruleStr));
        }
    }
    return w;
}

// Parse part string like "{x=787,m=2655,a=1222,s=2876}"
Part parsePart(const std::string& line) {
    Part p{};
    std::stringstream ss(line.substr(1, line.length() - 2));
    std::string item;
    while (std::getline(ss, item, ',')) {
        int value = std::stoi(item.substr(2));
        switch (item[0]) {
            case 'x': p.x = value; break;
            case 'm': p.m = value; break;
            case 'a': p.a = value; break;
            case 's': p.s = value; break;
        }
    }
    return p;
}

bool evaluateRule(const Rule& rule, const Part& part) {
    int partValue;
    switch (rule.category) {
        case 'x': partValue = part.x; break;
        case 'm': partValue = part.m; break;
        case 'a': partValue = part.a; break;
        case 's': partValue = part.s; break;
        default: return true; // Default rule
    }
    
    return rule.operation == '<' ? partValue < rule.value : partValue > rule.value;
}

int main() {
    std::map<std::string, Workflow> workflows;
    std::vector<Part> parts;
    std::string line;
    bool parsingParts = false;
    
    // Parse input
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            parsingParts = true;
            continue;
        }
        
        if (!parsingParts) {
            Workflow w = parseWorkflow(line);
            workflows[w.name] = w;
        } else {
            parts.push_back(parsePart(line));
        }
    }
    
    // Process parts
    int total = 0;
    for (const auto& part : parts) {
        std::string currentWorkflow = "in";
        
        while (currentWorkflow != "A" && currentWorkflow != "R") {
            const Workflow& w = workflows[currentWorkflow];
            bool ruleMatched = false;
            
            for (const Rule& rule : w.rules) {
                if (evaluateRule(rule, part)) {
                    currentWorkflow = rule.destination;
                    ruleMatched = true;
                    break;
                }
            }
            
            if (!ruleMatched) {
                currentWorkflow = w.defaultDest;
            }
        }
        
        if (currentWorkflow == "A") {
            total += part.sum();
        }
    }
    
    std::cout << total << std::endl;
    return 0;
}
