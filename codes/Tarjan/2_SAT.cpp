#include <bits/stdc++.h>

namespace internal {

template <class E>
struct csr {
  std::vector<int> start;
  std::vector<E> elist;
  explicit csr(int n, const std::vector<std::pair<int, E>>& edges)
      : start(n + 1), elist(edges.size()) {
    for (auto e : edges) {
      start[e.first + 1]++;
    }
    for (int i = 1; i <= n; i++) {
      start[i] += start[i - 1];
    }
    auto counter = start;
    for (auto e : edges) {
      elist[counter[e.first]++] = e.second;
    }
  }
};
// Reference:
// R. Tarjan,
// Depth-First Search and Linear Graph Algorithms
struct scc_graph {
 public:
  explicit scc_graph(int n) : _n(n) {}

  int num_vertices() { return _n; }

  void add_edge(int from, int to) { edges.push_back({from, {to}}); }

  // @return pair of (# of scc, scc id)
  std::pair<int, std::vector<int>> scc_ids() {
    auto g = csr<edge>(_n, edges);
    int now_ord = 0, group_num = 0;
    std::vector<int> visited, low(_n), ord(_n, -1), ids(_n);
    visited.reserve(_n);
    auto dfs = [&](auto self, int v) -> void {
      low[v] = ord[v] = now_ord++;
      visited.push_back(v);
      for (int i = g.start[v]; i < g.start[v + 1]; i++) {
        auto to = g.elist[i].to;
        if (ord[to] == -1) {
          self(self, to);
          low[v] = std::min(low[v], low[to]);
        } else {
          low[v] = std::min(low[v], ord[to]);
        }
      }
      if (low[v] == ord[v]) {
        while (true) {
          int u = visited.back();
          visited.pop_back();
          ord[u] = _n;
          ids[u] = group_num;
          if (u == v) break;
        }
        group_num++;
      }
    };
    for (int i = 0; i < _n; i++) {
      if (ord[i] == -1) dfs(dfs, i);
    }
    for (auto& x : ids) {
      x = group_num - 1 - x;
    }
    return {group_num, ids};
  }

  std::vector<std::vector<int>> scc() {
    auto ids = scc_ids();
    int group_num = ids.first;
    std::vector<int> counts(group_num);
    for (auto x : ids.second) counts[x]++;
    std::vector<std::vector<int>> groups(ids.first);
    for (int i = 0; i < group_num; i++) {
      groups[i].reserve(counts[i]);
    }
    for (int i = 0; i < _n; i++) {
      groups[ids.second[i]].push_back(i);
    }
    return groups;
  }

 private:
  int _n;
  struct edge {
    int to;
  };
  std::vector<std::pair<int, edge>> edges;
};

}  // namespace internal

struct scc_graph {
 public:
  scc_graph() : internal(0) {}
  explicit scc_graph(int n) : internal(n) {}

  void add_edge(int from, int to) {
    int n = internal.num_vertices();
    assert(0 <= from && from < n);
    assert(0 <= to && to < n);
    internal.add_edge(from, to);
  }

  std::vector<std::vector<int>> scc() { return internal.scc(); }

 private:
  internal::scc_graph internal;
};

struct two_sat {
 public:
  two_sat() : _n(0), scc(0) {}
  explicit two_sat(int n) : _n(n), _answer(n), scc(2 * n) {}

  void add_clause(int i, bool f, int j, bool g) {
    assert(0 <= i && i < _n);
    assert(0 <= j && j < _n);
    scc.add_edge(2 * i + (f ? 0 : 1), 2 * j + (g ? 1 : 0));
    scc.add_edge(2 * j + (g ? 0 : 1), 2 * i + (f ? 1 : 0));
  }
  bool satisfiable() {
    auto id = scc.scc_ids().second;
    for (int i = 0; i < _n; i++) {
      if (id[2 * i] == id[2 * i + 1]) return false;
      _answer[i] = id[2 * i] < id[2 * i + 1];
    }
    return true;
  }
  std::vector<bool> answer() { return _answer; }

 private:
  int _n;
  std::vector<bool> _answer;
  internal::scc_graph scc;
};

using namespace std;
int32_t main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  int t, n;
  array<array<int, 501>, 3> a;

  cin >> t;
  while (t--) {
    cin >> n;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < n; j++) {
        cin >> a[i][j];
      }
    }

    two_sat ts(n);
    for (int j = 0; j < n; j++) {
      for (int i = 0; i < 3; i++) {
        int nxt_i = (i + 1) % 3;
        ts.add_clause(abs(a[i][j]) - 1, a[i][j] > 0, abs(a[nxt_i][j]) - 1,
                      a[nxt_i][j] > 0);
      }
    }

    cout << (ts.satisfiable() ? "YES" : "NO") << '\n';
  }
  return 0;
}