#include <bits/stdc++.h>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read the map from stdin
    std::vector<std::string> grid;
    {
        std::string line;
        while (true) {
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) continue; // skip empty lines just in case
            grid.push_back(line);
        }
    }

    int H = (int)grid.size();
    if (H == 0) {
        // No input? Just print 0.
        std::cout << 0 << std::endl;
        return 0;
    }
    int W = (int)grid[0].size();

    // Find 'S' and 'E' positions
    std::pair<int,int> start = {-1, -1}, end = {-1, -1};
    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            if (grid[r][c] == 'S') {
                start = {r,c};
            } else if (grid[r][c] == 'E') {
                end = {r,c};
            }
        }
    }

    // We'll assume there's always a start and end.
    // Create a function to check if a cell is track (including S and E)
    auto is_track = [&](int r, int c) {
        if (r < 0 || r >= H || c < 0 || c >= W) return false;
        if (grid[r][c] == '#' ) return false;
        return true;
    };

    // BFS from S on track cells only
    const int INF = 1000000000;
    std::vector<std::vector<int>> distS(H, std::vector<int>(W, INF));
    {
        std::queue<std::pair<int,int>>q;
        distS[start.first][start.second] = 0;
        q.push(start);
        while (!q.empty()) {
            auto [x,y] = q.front(); q.pop();
            static const int dx[4] = {1,-1,0,0};
            static const int dy[4] = {0,0,1,-1};
            for (int i = 0; i < 4; i++) {
                int nx = x+dx[i], ny = y+dy[i];
                if (is_track(nx,ny) && distS[nx][ny] > distS[x][y]+1) {
                    distS[nx][ny] = distS[x][y]+1;
                    q.push({nx,ny});
                }
            }
        }
    }

    // BFS from E on track cells only (to get dist to E)
    std::vector<std::vector<int>> distE(H, std::vector<int>(W, INF));
    {
        std::queue<std::pair<int,int>>q;
        distE[end.first][end.second] = 0;
        q.push(end);
        while (!q.empty()) {
            auto [x,y] = q.front(); q.pop();
            static const int dx[4] = {1,-1,0,0};
            static const int dy[4] = {0,0,1,-1};
            for (int i = 0; i < 4; i++) {
                int nx = x+dx[i], ny = y+dy[i];
                if (is_track(nx,ny) && distE[nx][ny] > distE[x][y]+1) {
                    distE[nx][ny] = distE[x][y]+1;
                    q.push({nx,ny});
                }
            }
        }
    }

    int normalDist = distS[end.first][end.second];
    if (normalDist == INF) {
        // No normal route from S to E at all. Then no cheat can save time.
        std::cout << 0 << std::endl;
        return 0;
    }

    // We need to find all possible cheats. A cheat is identified by a start track cell and an end track cell.
    // Cheat = start a track cell 'a', perform up to 2 steps ignoring walls, must end on track cell 'b'.
    // Each step can go into walls or track. After finishing cheat (1 or 2 steps), we must be on track.
    // We'll consider 1-step and 2-step cheats.
    // For each cheat a->b:
    //   Saving = normalDist - (distS[a] + cheat_length + distE[b])
    // We'll count how many have saving >= 100.
    // We'll use a set to avoid duplicates. Key = (ax, ay, bx, by).

    struct CellHash {
        std::size_t operator()(const std::array<int,4>& arr) const {
            // Simple hash combination
            // We'll assume coordinates fit in int without issue.
            std::size_t h = 0;
            // Use a standard hash combine approach
            auto hash_int = std::hash<int>();
            for (int i = 0; i < 4; i++) {
                h ^= (std::size_t)hash_int(arr[i]) + 0x9e3779b97f4a7c16ULL + (h << 6) + (h >> 2);
            }
            return h;
        }
    };

    struct CellEq {
        bool operator()(const std::array<int,4>& a, const std::array<int,4>& b) const {
            return a[0]==b[0] && a[1]==b[1] && a[2]==b[2] && a[3]==b[3];
        }
    };

    std::unordered_set<std::array<int,4>, CellHash, CellEq> found_cheats;

    // Directions for convenience
    int dx[4] = {1,-1,0,0};
    int dy[4] = {0,0,1,-1};

    // Iterate over all track cells reachable from S
    for (int x = 0; x < H; x++) {
        for (int y = 0; y < W; y++) {
            if (distS[x][y] == INF) continue; // not reachable from S
            // Try 1-step cheat
            for (int i = 0; i < 4; i++) {
                int nx = x+dx[i], ny = y+dy[i];
                // 1-step: final must be track
                // Even though we ignore walls for movement, final cell must be track cell
                // Check bounds
                if (nx<0||nx>=H||ny<0||ny>=W) continue;
                if (grid[nx][ny] != '#') {
                    // It's track at the end of 1-step cheat
                    if (distE[nx][ny] != INF) {
                        int cheat_length = 1;
                        int saving = normalDist - (distS[x][y] + cheat_length + distE[nx][ny]);
                        if (saving >= 100) {
                            found_cheats.insert({x,y,nx,ny});
                        }
                    }
                }
            }

            // Try 2-step cheat
            // For the first step, we can land on wall or track (no restriction)
            // For the second step, we must land on track.
            for (int i = 0; i < 4; i++) {
                int ix = x+dx[i], iy = y+dy[i]; // intermediate cell after first step
                if (ix<0||ix>=H||iy<0||iy>=W) continue;
                // intermediate can be anything (wall or track), no restriction here.
                for (int j = 0; j < 4; j++) {
                    int fx = ix+dx[j], fy = iy+dy[j]; // final cell
                    if (fx<0||fx>=H||fy<0||fy>=W) continue;
                    // final must be track
                    if (grid[fx][fy] != '#') {
                        // final track cell found
                        if (distE[fx][fy] != INF) {
                            int cheat_length = 2;
                            int saving = normalDist - (distS[x][y] + cheat_length + distE[fx][fy]);
                            if (saving >= 100) {
                                found_cheats.insert({x,y,fx,fy});
                            }
                        }
                    }
                }
            }
        }
    }

    // Output how many cheats save at least 100 picoseconds
    std::cout << (int)found_cheats.size() << std::endl;
    return 0;
}

