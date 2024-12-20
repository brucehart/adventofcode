#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

// This solution is similar to Part 1, but now we must consider cheats
// that can last up to 20 picoseconds, not just up to 2. Each cheat can start
// from any track cell reachable from S, can move through walls for up to 20 steps,
// and must end on a track cell. The result is identified solely by start/end positions.
//
// We'll approach this as follows:
// 1. Compute distS: distance from S to every reachable track cell without cheating.
// 2. Compute distE: distance from E to every reachable track cell without cheating.
// 3. normalDist = distS at E
//
// For each track cell A reachable from S, do a limited BFS of up to 20 steps where
// walls are ignored. Record the shortest distance ignoring walls to each track cell B.
// If distE[B] is known, then a cheat from A to B of length L gives a saving of:
//     saving = normalDist - (distS[A] + L + distE[B])
// We only record cheats that save >= 100. 
// We store discovered cheats in a set keyed by (Ax, Ay, Bx, By) so we don't double-count.
//
// Note: The BFS ignoring walls can be large if the grid is large, but we'll trust that 
// the input is manageable for this approach within a time limit. We only do this BFS from 
// track cells reachable from S. If the map is large and most cells are walls, hopefully 
// the overhead is still okay.
//
// We'll optimize by:
// - Using a fast BFS
// - Early stop if no improvements.
//
// Also note that we are only asked for the count of such cheats, not their details.
//
// Complexity note: In worst case, if many cells are reachable from S, for each cell we run a BFS 
// of radius up to 20 in all directions. This can be costly. But since we have no further constraints,
// we'll implement this straightforwardly.
//
// Steps to implement:
// - Read grid, find S, E
// - BFS to find distS and distE on track
// - normalDist = distS[E]
// - For each cell (x,y) reachable from S, run a BFS ignoring walls up to depth 20
//   and for each found track cell (fx,fy) at depth d, compute saving and record if saving >= 100.
//
// Output the count of unique such cheats.


struct CellHash {
    std::size_t operator()(const std::array<int,4>& arr) const {
        // Simple hash combination
        std::size_t h = 0;
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

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::string> grid;
    {
        std::string line;
        while (true) {
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) continue;
            grid.push_back(line);
        }
    }

    int H = (int)grid.size();
    if (H == 0) {
        std::cout << 0 << std::endl;
        return 0;
    }
    int W = (int)grid[0].size();

    // Find S and E
    std::pair<int,int> start{-1,-1}, end{-1,-1};
    for (int r=0; r<H; r++){
        for (int c=0; c<W; c++){
            if (grid[r][c] == 'S') start = {r,c};
            else if (grid[r][c] == 'E') end = {r,c};
        }
    }

    if (start.first == -1 || end.first == -1) {
        // No start or end
        std::cout << 0 << std::endl;
        return 0;
    }

    auto is_track = [&](int r, int c) {
        if (r<0||r>=H||c<0||c>=W) return false;
        if (grid[r][c] == '#') return false;
        return true;
    };

    const int INF = 1000000000;
    std::vector<std::vector<int>> distS(H, std::vector<int>(W, INF));
    std::vector<std::vector<int>> distE(H, std::vector<int>(W, INF));

    // BFS from S
    {
        std::queue<std::pair<int,int>>q;
        distS[start.first][start.second] = 0;
        q.push(start);
        while(!q.empty()){
            auto [x,y] = q.front();q.pop();
            static const int dx[4]={1,-1,0,0};
            static const int dy[4]={0,0,1,-1};
            for (int i=0;i<4;i++){
                int nx=x+dx[i], ny=y+dy[i];
                if (is_track(nx,ny) && distS[nx][ny]>distS[x][y]+1){
                    distS[nx][ny]=distS[x][y]+1;
                    q.push({nx,ny});
                }
            }
        }
    }

    // BFS from E
    {
        std::queue<std::pair<int,int>>q;
        distE[end.first][end.second] = 0;
        q.push(end);
        while(!q.empty()){
            auto [x,y] = q.front();q.pop();
            static const int dx[4]={1,-1,0,0};
            static const int dy[4]={0,0,1,-1};
            for (int i=0;i<4;i++){
                int nx=x+dx[i], ny=y+dy[i];
                if (is_track(nx,ny) && distE[nx][ny]>distE[x][y]+1){
                    distE[nx][ny]=distE[x][y]+1;
                    q.push({nx,ny});
                }
            }
        }
    }

    int normalDist = distS[end.first][end.second];
    if (normalDist == INF) {
        // No route at all
        std::cout << 0 << std::endl;
        return 0;
    }

    // We'll find all cheats now.
    // For each cell reachable from S, run a BFS up to 20 steps ignoring walls.
    // For the BFS ignoring walls, every cell (even '#') is considered reachable,
    // but we must only track up to distance 20.
    // When we reach a track cell at distance d <= 20, consider a cheat from current cell to that track cell.

    // Precompute which cells are track and which are not to quickly check final conditions.
    // For BFS ignoring walls:
    // dist_ignore array will be reused for each starting cell to reduce memory overhead.

    static const int DX[4] = {1,-1,0,0};
    static const int DY[4] = {0,0,1,-1};

    std::unordered_set<std::array<int,4>, CellHash, CellEq> found_cheats;

    // We'll make a list of all track cells reachable from S to iterate over.
    std::vector<std::pair<int,int>> reachableFromS;
    reachableFromS.reserve(H*W);
    for (int x=0;x<H;x++){
        for (int y=0;y<W;y++){
            if (distS[x][y]<INF) {
                reachableFromS.push_back({x,y});
            }
        }
    }

    // We'll do a BFS for each cell in reachableFromS.
    // To speed up repeated BFS:
    // We can use a queue and a distance array for each iteration.
    // We'll create them outside the loop to minimize overhead.

    std::vector<std::vector<int>> distIgnore(H, std::vector<int>(W, INF));
    std::queue<std::pair<int,int>> q;

    for (auto [sx,sy] : reachableFromS) {
        // Reset distIgnore
        for (int xx=0;xx<H;xx++){
            // Setting entire rows to INF might be expensive if large grid.
            // An optimization would be to keep track of visited cells and only reset them.
            // But let's just do this straightforwardly.
            std::fill(distIgnore[xx].begin(), distIgnore[xx].end(), INF);
        }

        distIgnore[sx][sy] = 0;
        while(!q.empty()) q.pop();
        q.push({sx,sy});

        while(!q.empty()){
            auto [x,y]=q.front();q.pop();
            int curd = distIgnore[x][y];
            if (curd==20) continue; // can't go beyond 20 steps

            for (int i=0;i<4;i++){
                int nx=x+DX[i], ny=y+DY[i];
                if (nx<0||nx>=H||ny<0||ny>=W) continue;
                if (distIgnore[nx][ny]>curd+1){
                    distIgnore[nx][ny]=curd+1;
                    q.push({nx,ny});
                }
            }
        }

        // Now distIgnore contains the min steps ignoring walls to every cell reachable within 20 steps.
        // For each track cell reachable within <=20 steps, record the cheat.
        for (int fx=0; fx<H; fx++){
            for (int fy=0; fy<W; fy++){
                int d = distIgnore[fx][fy];
                if (d<=20 && d>0) {
                    // final cell must be track
                    if (is_track(fx,fy)) {
                        if (distE[fx][fy]<INF) {
                            int cheat_length = d;
                            int saving = normalDist - (distS[sx][sy] + cheat_length + distE[fx][fy]);
                            if (saving>=100) {
                                found_cheats.insert({sx,sy,fx,fy});
                            }
                        }
                    }
                }
            }
        }
    }

    std::cout << (int)found_cheats.size() << std::endl;
    return 0;
}

