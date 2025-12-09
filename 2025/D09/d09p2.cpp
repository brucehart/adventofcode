#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct Point {
    long long x;
    long long y;
};

struct Strip {
    long long y_start;
    long long y_end;
    std::vector<std::pair<long long, long long>> intervals;  // inclusive [l, r]
};

static std::vector<std::pair<long long, long long>> build_intervals(const std::multiset<long long>& active) {
    std::vector<std::pair<long long, long long>> intervals;
    if (active.empty()) {
        return intervals;
    }

    std::vector<long long> xs;
    xs.reserve(active.size());
    for (auto x : active) {
        xs.push_back(x);
    }

    if (xs.size() % 2 != 0) {
        return intervals;  // malformed polygon; no interior on this row
    }

    intervals.reserve(xs.size() / 2);
    for (std::size_t i = 0; i + 1 < xs.size(); i += 2) {
        intervals.push_back({xs[i], xs[i + 1]});
    }

    return intervals;
}

static void merge_intervals(std::vector<std::pair<long long, long long>>& v) {
    if (v.empty()) {
        return;
    }
    std::sort(v.begin(), v.end());
    std::vector<std::pair<long long, long long>> merged;
    merged.reserve(v.size());
    merged.push_back(v[0]);
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (v[i].first <= merged.back().second + 1) {
            merged.back().second = std::max(merged.back().second, v[i].second);
        } else {
            merged.push_back(v[i]);
        }
    }
    v.swap(merged);
}

static bool covers(const std::vector<std::pair<long long, long long>>& intervals, long long x_lo, long long x_hi) {
    for (const auto& seg : intervals) {
        if (seg.first <= x_lo && seg.second >= x_hi) {
            return true;
        }
    }
    return false;
}

int main() {
    std::vector<Point> points;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        std::replace(line.begin(), line.end(), ',', ' ');
        std::stringstream ss(line);
        Point p{};
        if (ss >> p.x >> p.y) {
            points.push_back(p);
        }
    }

    const std::size_t n = points.size();
    if (n < 2) {
        std::cout << 0 << '\n';
        return 0;
    }

    // Collect horizontal edge segments for boundary coverage.
    std::map<long long, std::vector<std::pair<long long, long long>>> horiz;
    for (std::size_t i = 0; i < n; ++i) {
        const auto& a = points[i];
        const auto& b = points[(i + 1) % n];
        if (a.y == b.y) {
            const long long x1 = std::min(a.x, b.x);
            const long long x2 = std::max(a.x, b.x);
            horiz[a.y].push_back({x1, x2});
        }
    }
    for (auto& kv : horiz) {
        merge_intervals(kv.second);
    }

    long long min_y = points.front().y;
    long long max_y = points.front().y;
    for (const auto& p : points) {
        min_y = std::min(min_y, p.y);
        max_y = std::max(max_y, p.y);
    }

    // Build events for vertical edges: active on rows y in [y_low, y_high).
    std::map<long long, std::vector<long long>> add_events;
    std::map<long long, std::vector<long long>> remove_events;
    for (std::size_t i = 0; i < n; ++i) {
        const auto& a = points[i];
        const auto& b = points[(i + 1) % n];
        if (a.x == b.x) {
            const long long y1 = std::min(a.y, b.y);
            const long long y2 = std::max(a.y, b.y);
            add_events[y1].push_back(a.x);
            remove_events[y2].push_back(a.x);
        }
    }

    std::set<long long> event_ys;
    for (const auto& kv : add_events) {
        event_ys.insert(kv.first);
    }
    for (const auto& kv : remove_events) {
        event_ys.insert(kv.first);
    }

    event_ys.insert(max_y + 1);  // sentinel to cover the top boundary row

    std::vector<long long> sorted_events(event_ys.begin(), event_ys.end());

    std::multiset<long long> active;
    std::vector<Strip> strips;

    for (std::size_t idx = 0; idx < sorted_events.size(); ++idx) {
        const long long y = sorted_events[idx];

        auto rem_it = remove_events.find(y);
        if (rem_it != remove_events.end()) {
            for (auto x : rem_it->second) {
                auto it = active.find(x);
                if (it != active.end()) {
                    active.erase(it);
                }
            }
        }

        auto add_it = add_events.find(y);
        if (add_it != add_events.end()) {
            for (auto x : add_it->second) {
                active.insert(x);
            }
        }

        const long long next_y = (idx + 1 < sorted_events.size()) ? sorted_events[idx + 1] : y;
        if (next_y > y) {
            strips.push_back(Strip{y, next_y - 1, build_intervals(active)});
        }
    }

    long long best_area = 0;
    for (std::size_t i = 0; i + 1 < n; ++i) {
        for (std::size_t j = i + 1; j < n; ++j) {
            const long long x_lo = std::min(points[i].x, points[j].x);
            const long long x_hi = std::max(points[i].x, points[j].x);
            const long long y_lo = std::min(points[i].y, points[j].y);
            const long long y_hi = std::max(points[i].y, points[j].y);

            const long long area = (x_hi - x_lo + 1) * (y_hi - y_lo + 1);
            if (area <= best_area) {
                continue;
            }

            // Find first strip that could overlap y_lo.
            const auto it = std::lower_bound(
                strips.begin(), strips.end(), y_lo,
                [](const Strip& s, long long value) { return s.y_end < value; });
            std::size_t idx = static_cast<std::size_t>(it - strips.begin());
            if (idx == strips.size()) {
                continue;
            }

            bool ok = true;
            for (; idx < strips.size() && strips[idx].y_start <= y_hi; ++idx) {
                const long long overlap_start = std::max(strips[idx].y_start, y_lo);
                const long long overlap_end = std::min(strips[idx].y_end, y_hi);
                const long long rows = overlap_end - overlap_start + 1;

                const bool interior = covers(strips[idx].intervals, x_lo, x_hi);

                if (interior) {
                    // All rows in this strip are covered by interior fill.
                    continue;
                }

                if (rows == 1) {
                    // Only a single row intersects this strip; allow horizontal boundary coverage.
                    auto h_it = horiz.find(overlap_start);
                    if (h_it == horiz.end() || !covers(h_it->second, x_lo, x_hi)) {
                        ok = false;
                        break;
                    }
                } else {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                best_area = area;
            }
        }
    }

    std::cout << best_area << '\n';
    return 0;
}
