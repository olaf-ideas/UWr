#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <queue>

template <typename T>
using vector2D = std::vector<std::vector<T>>;

template <typename T>
void InitWithValue(vector2D<T> &v, int n, int m, T init_value) {
    v.resize(n);
    for(int i = 0; i < n; i++) {
        v[i].resize(m);
        std::fill(v[i].begin(), v[i].end(), init_value);
    }
}

bool IsFree(const std::vector<int> &line, int l, int r) {
    for(int i = l; i <= r; i++) {
        if(line[i] == -1) {
            return false;
        }
    }
    return true;
}

void AddOne(std::vector<int> &v, int l, int r) {
    for(int i = l; i <= r; i++) {
        v[i]++;
    } 
}

//line is filled
bool IsConflict(const std::vector<int> &constraints, const std::vector<int> &line) {
    int n = line.size();
    int m  = constraints.size();
    int c = 0;
    for(int i = 0; i < n; i++) {
        if(line[i] == 0) return true;
        if(line[i] == -1) continue;
        if(c == m) return true;
        int j = i;
        while(j < n && line[j] == 1) j++;
        if(j-i != constraints[c]) {
            return true;
        }
        c++;
        i = j-1;
    }
    return c < m;
}

void ComputePrefixDP(const std::vector<int> &constraints, const std::vector<int> &line,
                     vector2D<bool> &satisfy_prefix) {
    int n = line.size();
    int m = constraints.size();
    InitWithValue(satisfy_prefix, m+1, n, false);
    int first_one = 0;
    for(; first_one < n; first_one++) {
        if(line[first_one] == 1) break;
    }
    std::fill(satisfy_prefix[0].begin(), satisfy_prefix[0].begin()+first_one, true);
    //fill first row for prefix dp
    for(int i = constraints[0]-1, c = constraints[0]; i < n; i++) {
        if(line[i] != 1 && i > 0 && satisfy_prefix[1][i-1]) {
            satisfy_prefix[1][i] = true;
            continue;
        }
        if(IsFree(line, i-c+1, i) && first_one > i-c) {
            satisfy_prefix[1][i] = true;
        }
    }
    for(int i = 1; i < m; i++) {
        int c = constraints[i];
        for(int j = c-1; j < n; j++) {
            if(line[j] != 1 && j > 0 && satisfy_prefix[i+1][j-1]) {
                satisfy_prefix[i+1][j] = true;
                continue;
            }
            if(IsFree(line, j-c+1, j) && j-c-1 >= 0 && line[j-c] != 1 && satisfy_prefix[i][j-c-1]) {
                satisfy_prefix[i+1][j] = true;
            }
        }
    }
}

void ComputeSuffixDP(const std::vector<int> &constraints, const std::vector<int> &line,
                     vector2D<bool> &satisfy_suffix) {
    int n = line.size();
    int m = constraints.size();
    InitWithValue(satisfy_suffix, m+1, n, false);
    int last_one = n-1;
    for(; last_one >= 0; last_one--) {
        if(line[last_one] == 1) break;
    }
    std::fill(satisfy_suffix[m].begin()+last_one+1, satisfy_suffix[m].end(), true);
    //fill last row for suffix dp
    for(int i = n-constraints[m-1], c = constraints[m-1]; i >= 0; i--) {
        if(line[i] != 1 && i+1 < n && satisfy_suffix[m-1][i+1]) {
            satisfy_suffix[m-1][i] = true;
            continue;
        }
        if(IsFree(line, i, i+c-1) && last_one < i+c) {
            satisfy_suffix[m-1][i] = true;
        }
    }
    for(int i = m-2; i >= 0; i--) {
        int c = constraints[i];
        for(int j = n-c; j >= 0; j--) {
            if(line[j] != 1 && j+1 < n && satisfy_suffix[i][j+1]) {
                satisfy_suffix[i][j] = true;
                continue;
            }
            if(IsFree(line, j, j+c-1) && j+c+1 < n && line[j+c] != 1 && satisfy_suffix[i+1][j+c+1]) {
                satisfy_suffix[i][j] = true;
            }
        }
    }
}

//Returns modified line
//Returns false if there's a conflict
std::pair<bool, std::vector<int>> Infer(const std::vector<int> &constraints, const std::vector<int> &line) {
    int n = line.size();
    int m = constraints.size();
    auto res = line;
    if(m == 1 && constraints[0] == 0) {
        for(int i = 0; i < n; i++) {
            res[i] = -1;
            if(line[i] == 1) {
                return {true, res};
            }
        }
        return {false, res};
    }
    vector2D<bool> satisfy_prefix, satisfy_suffix;
    ComputePrefixDP(constraints, line, satisfy_prefix);
    ComputeSuffixDP(constraints, line, satisfy_suffix);
    //Check for conflict
    if(!satisfy_suffix[0][0]) {
        return {true, res};
    }
    auto CanSatisfyPrefix = [&n, &m, &line, &satisfy_prefix](int pos, int i) {
        if(i == -1) {
            return pos < 0 || satisfy_prefix[0][pos];
        }
        return pos > 0 && line[pos] != 1 && satisfy_prefix[i+1][pos-1];
    };
    auto CanSatisfySuffix = [&n, &m, &line, &satisfy_suffix](int pos, int i) {
        if(i == m) {
            return pos >= n || satisfy_suffix[m][pos];
        }
        return pos+1 < n && line[pos] != 1 && satisfy_suffix[i][pos+1];
    };
    std::vector<int> visited(n);
    std::vector<bool> always_empty(n, true);
    for(int i = 0; i < m; i++) {
        std::fill(visited.begin(), visited.end(), 0);
        int c = constraints[i];
        int nr_visited = 0;
        for(int j = 0; j+c-1 < n; j++) {
            if(!CanSatisfyPrefix(j-1, i-1)) continue;
            if(!CanSatisfySuffix(j+c, i+1)) continue;
            if(!IsFree(line, j, j+c-1)) continue;
            AddOne(visited, j, j+c-1);
            nr_visited++;
        }
        for(int j = 0; j < n; j++) {
            if(visited[j] > 0) {
                always_empty[j] = false;
            }
            if(visited[j] == nr_visited) {
                res[j] = 1;
            }
        }
    }
    for(int i = 0; i < n; i++) {
        if(always_empty[i]) {
            res[i] = -1;
        }
    }
    return {false, res};
}


class BacktrackingSolver {
public:
    BacktrackingSolver(int _n, int _m, const vector2D<int> &c);
    void Solve();
    std::string ShowBoard();
private:
    vector2D<int> rows, cols, row_constraints, col_constraints;
    const int n, m;
    std::queue <int> Q;

    void InitInferingQueue();
    bool ProcessInferingQueue();
    bool SimpleInfer(int idx);
    bool SetSquare(int x, int y, int v);
    bool Backtrack(int x, int y);
    bool WeakBacktrack(int x, int y);
    bool CheckIfSolved();
    std::pair<int,int> NextSquare(int x, int y);
};

BacktrackingSolver::BacktrackingSolver(int _n, int _m, const vector2D<int> &c): n(_n), m(_m) {
    row_constraints.resize(n);
    col_constraints.resize(m);
    for(int i = 0; i < n; i++) {
        row_constraints[i] = c[i];
    }
    for(int i = 0; i < m; i++) {
        col_constraints[i] = c[n+i];
    }
    InitWithValue(rows, n, m, 0);
    InitWithValue(cols, m, n, 0);
}

bool BacktrackingSolver::CheckIfSolved() {
    for(int i = 0; i < n; i++) {
        if(IsConflict(row_constraints[i], rows[i])) {
            return false;
        }
    }
    for(int i = 0; i < m; i++) {
        if(IsConflict(col_constraints[i], cols[i])) {
            return false;
        }
    }
    return true;
}

void BacktrackingSolver::InitInferingQueue() {
    for(int i = 0; i < n+m; i++) {
        Q.push(i);
    }
}
//Returns true if there's a conflict
bool BacktrackingSolver::ProcessInferingQueue() {
    while(!Q.empty()) {
        int v = Q.front();
        Q.pop();
        if(v >= n) {
            auto [conflict, new_line] = Infer(col_constraints[v-n], cols[v-n]);
            if(conflict) {
                while(!Q.empty()) Q.pop();
                return true;
            }
            for(int i = 0; i < n; i++) {
                if(new_line[i] != cols[v-n][i]) {
                    cols[v-n][i] = new_line[i];
                    rows[i][v-n] = new_line[i];
                    Q.push(i);
                }
            }
        }
        else {
            auto [conflict, new_line] = Infer(row_constraints[v], rows[v]);
            if(conflict) {
                while(!Q.empty()) Q.pop();
                return true;
            }
            for(int i = 0; i < m; i++) {
                if(new_line[i] != rows[v][i]) {
                    rows[v][i] = new_line[i];
                    cols[i][v] = new_line[i];
                    Q.push(i+n);
                }
            }
        }
    }
    return false;
}

bool BacktrackingSolver::SimpleInfer(int idx) {
    if(idx >= n) {
        auto [conflict, new_line] = Infer(col_constraints[idx-n], cols[idx-n]);
        if(conflict) return true;
        for(int i = 0; i < n; i++) {
            if(new_line[i] != cols[idx-n][i]) {
                cols[idx-n][i] = new_line[i];
                rows[i][idx-n] = new_line[i];
            }
        }
    }
    else {
        auto [conflict, new_line] = Infer(row_constraints[idx], rows[idx]);
        if(conflict) return true;
        for(int i = 0; i < m; i++) {
            if(new_line[i] != rows[idx][i]) {
                rows[idx][i] = new_line[i];
                cols[i][idx] = new_line[i];
            }
        }
    }
    return false;
}


void BacktrackingSolver::Solve() {
    InitInferingQueue();
    ProcessInferingQueue();
    Backtrack(0, 0);
    // std::cerr << WeakBacktrack(0, 0) << std::endl;
    // std::cerr << ShowBoard() << std::endl;
}

std::string BacktrackingSolver::ShowBoard() {
    std::string s;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(rows[i][j] == -1) {
                s.push_back('.');
            }
            else if(rows[i][j] == 1) {
                s.push_back('#');
            }
            else {
                s.push_back(' ');
            }
        }
        s.push_back('\n');
    }
    return s;
}

std::pair<int,int> BacktrackingSolver::NextSquare(int x, int y) {
    return y == m-1 ? std::make_pair(x+1, 0) : std::make_pair(x, y+1);
}

bool BacktrackingSolver::SetSquare(int x, int y, int v) {
    rows[x][y] = v;
    cols[y][x] = v;
    // return SimpleInfer(x) || SimpleInfer(n+y);
    Q.push(x); Q.push(n+y);
    return ProcessInferingQueue();
}

bool BacktrackingSolver::Backtrack(int x, int y) {
    if(x == n && y == 0) {
        return true;
    }
    auto [new_x, new_y] = NextSquare(x, y);
    if(rows[x][y] != 0) {
        return Backtrack(new_x, new_y);
    }
    auto rows_copy = rows, cols_copy = cols;
    if(!SetSquare(x, y, 1)) {
        if(Backtrack(new_x, new_y)) {
            return true;
        }
    }
    rows = rows_copy;
    cols = cols_copy;
    if(!SetSquare(x, y, -1)) {
        if(Backtrack(new_x, new_y)) {
            return true;
        }
    }
    rows = rows_copy;
    cols = cols_copy;
    return false;
}

bool BacktrackingSolver::WeakBacktrack(int x, int y) {
    if(x == n && y == 0) {
        return CheckIfSolved();
    }
    auto [new_x, new_y] = NextSquare(x, y);
    if(rows[x][y] != 0) {
        return WeakBacktrack(new_x, new_y);
    }
    auto rows_copy = rows, cols_copy = cols;
    if(!SetSquare(x, y, 1)) {
        if(WeakBacktrack(new_x, new_y)) {
            return true;
        }
    }
    rows = rows_copy;
    cols = cols_copy;
    if(!SetSquare(x, y, -1)) {
        if(WeakBacktrack(new_x, new_y)) {
            return true;
        }
    }
    rows = rows_copy;
    cols = cols_copy;
    return false;
}

struct InputData{
    int n, m;
    std::vector <std::vector <int>> constraints;
};

void read_data(InputData &data){
    std::fstream fi;
    fi.open("zad_input.txt", std::fstream::in);
    int n, m;
    fi >> n >> m;
    fi.ignore();
    data.constraints.clear();
    data.constraints.resize(n + m);
    for(int i = 0; i < n + m; i++){
        std::string line;
        getline(fi, line);
        std::istringstream iss(line);
        int x;
        while(iss >> x){
            data.constraints[i].push_back(x);
        }
    }
    fi.close();
    data.n = n;
    data.m = m;
}



int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(0);
    std::fstream fo;
    fo.open("zad_output.txt", std::fstream::out);
    InputData in_data;
    read_data(in_data);
    BacktrackingSolver solver(in_data.n, in_data.m, in_data.constraints);
    solver.Solve();
    fo << solver.ShowBoard();
    fo.close();

    return 0;
}
