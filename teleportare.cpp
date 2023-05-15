#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
using namespace std;
struct elem
{
    int node;
    long long cost;
    bool operator<(const elem &other) const
    {
        return cost > other.cost;
    }
};
static constexpr int NMAX = (int)1e5 + 5;
static constexpr int MAX_PERIOD = (int)8;
long long dp[420][NMAX];

class Task
{
public:
    void solve()
    {
        read_input();
        print_output(get_result());
    }

private:
    int nr_nodes, nr_edges, nr_special_edges;
    vector<elem> adj[NMAX];                        // adjancency list for normal edges
    vector<int> special_adj[NMAX][MAX_PERIOD + 2]; // adjancency list for special edges
    priority_queue<elem> pq;

    void read_input()
    {
        ifstream fin("teleportare.in");
        fin >> nr_nodes >> nr_edges >> nr_special_edges;

        // Read normal edges
        for (int i = 1, x, y, z; i <= nr_edges; i++)
        {
            fin >> x >> y >> z;
            adj[x].push_back({y, z});
            adj[y].push_back({x, z});
        }

        // Read special edges
        for (int i = 1, x, y, z; i <= nr_special_edges; i++)
        {
            fin >> x >> y >> z;
            special_adj[x][z].push_back(y);
            special_adj[y][z].push_back(x);
        }

        fin.close();
    }

    long long get_result()
    {

        for (int i = 0; i < 420; i++)
        {
            for (int j = 1; j <= nr_nodes; j++)
            {
                dp[i][j] = 1e18;
            }
        }

        pq.push({1, 0});
        dp[0][1] = 0;

        while (!pq.empty())
        {
            int node = pq.top().node;
            long long dist = pq.top().cost;

            pq.pop();
            if (node == nr_nodes)
            {
                break;
            }

            for (auto &elem : adj[node])
            {
                int neigh = elem.node;
                long long cost = elem.cost;

                if (dp[(dist + cost) % 420][neigh] > dp[dist % 420][node] + cost)
                {
                    dp[(dist + cost) % 420][neigh] = dp[dist % 420][node] + cost;
                    pq.push({neigh, dp[(dist + cost) % 420][neigh]});
                }
            }

            for (int period = 1; period <= MAX_PERIOD; period++)
            {
                if (dist % period == 0)
                {
                    for (int i = 0; i < special_adj[node][period].size(); i++)
                    {
                        int neigh = special_adj[node][period][i];
                        if (dp[(dist + 1) % 420][neigh] > dp[dist % 420][node] + 1)
                        {
                            dp[(dist + 1) % 420][neigh] = dp[dist % 420][node] + 1;
                            pq.push({neigh, dp[(dist + 1) % 420][neigh]});
                        }
                    }
                }
            }
        }

        long long min = LLONG_MAX;
        for (int i = 0; i < 420; i++)
        {
            if (dp[i][nr_nodes] < min)
            {
                min = dp[i][nr_nodes];
            }
        }
        return min;
    }

    void
    print_output(long long result)
    {
        ofstream fout("teleportare.out");
        fout << result << '\n';
        fout.close();
    }
};

int main()
{
    auto *task = new (nothrow) Task();
    if (!task)
    {
        cerr << "new failed: WTF are you doing? Throw your PC!\n";
        return -1;
    }
    task->solve();
    delete task;
    return 0;
}
