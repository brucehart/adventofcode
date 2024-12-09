#include <iostream>
#include <vector>
#include <queue>
#include <map>

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        grid.push_back(line);
    }

    int start_row = -1, start_col = -1;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == 'S') {
                start_row = i;
                start_col = j;
                grid[i][j] = '.';
                break;
            }
        }
        if (start_row != -1) break;
    }

    int rows = grid.size();
    int cols = grid[0].size();
    long long steps = 26501365;

    
    std::vector<std::pair<int,int>> reachable_in_one;
     for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if(grid[r][c] == '.'){
              reachable_in_one.push_back({r,c});
            }
        }
    }

    std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> adj;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for(auto p : reachable_in_one){
      for (int i = 0; i < 4; ++i) {
          int nr = p.first + dr[i];
          int nc = p.second + dc[i];
          if (grid[(nr % rows + rows) % rows][(nc % cols + cols) % cols] == '.') {
            adj[p].push_back({nr, nc});
          }
      }
    }

    auto bfs = [&](int max_steps) {
      std::queue<std::pair<int, int>> q;
      q.push({start_row, start_col});
      std::map<std::pair<int, int>, int> dist;
      dist[{start_row, start_col}] = 0;
      
      while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();
        if (dist[{r,c}] == max_steps) continue;
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            int orow = (nr % rows + rows) % rows;
            int ocol = (nc % cols + cols) % cols;
            
            if (grid[orow][ocol] == '.') {
                if (dist.find({nr, nc}) == dist.end()){
                  dist[{nr, nc}] = dist[{r,c}] + 1;
                  q.push({nr, nc});
                }
            }
        }
      }
      long long count = 0;
      for(auto const& [key, val] : dist){
        if (val % 2 == max_steps % 2) count++;
      }
      return count;
    };

    long long y0 = bfs(65);
    long long y1 = bfs(65 + 131);
    long long y2 = bfs(65 + 131 * 2);

    auto extrapolate = [](long long n, long long y0, long long y1, long long y2) {
        long long a = (y2 - 2 * y1 + y0) / 2;
        long long b = y1 - y0 - a;
        long long c = y0;
        return a * n * n + b * n + c;
    };

    long long n = steps / rows;
    std::cout << extrapolate(n, y0, y1, y2) << std::endl;

    return 0;
}
