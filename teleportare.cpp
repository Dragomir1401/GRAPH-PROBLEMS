#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
using namespace std;
struct Edge
{
    int node;
    long long cost;

    bool operator<(const Edge &other) const
    {
        return cost > other.cost;
    }
};

static constexpr int NMAX = (int)1e5 + 5;
static constexpr int MAX_PERIOD = (int)8;
static constexpr long long DP_MAX = 1e12;
static constexpr int COST_MOD = 60;
long long dp[COST_MOD][NMAX];

class Task
{
public:
    void solve()
    {
        read_input();
        print_output(get_result());
    }

private:
    int nr_nodes, nr_Edges, nr_special_Edges;
    vector<Edge> adj[NMAX]; // adjancency list for normal Edges
    // vector<int> special_adj[MAX_PERIOD + 1][NMAX]; // adjancency list for special Edges
    vector<int> special_adj[MAX_PERIOD + 1][NMAX];
    priority_queue<Edge> pq;

    void read_input()
    {
        ifstream fin("teleportare.in");
        fin >> nr_nodes >> nr_Edges >> nr_special_Edges;

        // Read normal Edges
        for (int i = 1, x, y, z; i <= nr_Edges; i++)
        {
            // Node, neighbour, edge cost
            fin >> x >> y >> z;
            adj[x].push_back({y, z});
            adj[y].push_back({x, z});
        }

        // Read special Edges
        for (int i = 1, x, y, z; i <= nr_special_Edges; i++)
        {
            // Node, neighbour, period
            fin >> x >> y >> z;
            special_adj[z][x].push_back(y);
            special_adj[z][y].push_back(x);
        }

        fin.close();
    }

    void initialise_dp()
    {
        // Initialise dp
        for (int i = 0; i < COST_MOD; i++)
        {
            for (int j = 1; j <= nr_nodes; j++)
            {
                dp[i][j] = DP_MAX;
            }
        }

        dp[0][1] = 0;
    }

    long long find_min_dp()
    {
        // Find the minimum dp value
        long long min = DP_MAX;
        for (long unsigned int i = 0; i < COST_MOD; i++)
        {
            if (dp[i][nr_nodes] < min)
            {
                min = dp[i][nr_nodes];
            }
        }

        return min;
    }

    void update_dp_normal_edge(int node, long long cost)
    {
        for (auto &Edge : adj[node])
        {
            // Get the normal edge node and cost
            int neigh = Edge.node;
            long long edge_cost = Edge.cost;

            // Calculate the new cost and dp
            long long new_cost = cost + edge_cost;
            int rest = new_cost % COST_MOD;
            long long old_dp = dp[rest][neigh];
            long long new_dp = dp[cost % COST_MOD][node] + edge_cost;

            // Check if we can improve dp value
            if (old_dp > new_dp)
            {
                // Update dp and push the new node in the queue
                dp[rest][neigh] = new_dp;
                pq.push({neigh, new_dp});
            }
        }
    }

    void update_dp_special_edge(int node, long long cost)
    {
        // Check if we can use a special edge
        for (int period = 1; period <= MAX_PERIOD; period++)
        {

            // If we are at a step that is a multiple of the period
            if (cost % period == 0)
            {
                for (long unsigned int i = 0; i < special_adj[period][node].size(); i++)
                {
                    // Get the special edge node and cost
                    int neigh = special_adj[period][node][i];
                    int teleport_cost = 1;

                    // Calculate the new cost and dp
                    long long new_cost = cost + teleport_cost;
                    int rest = new_cost % COST_MOD;
                    long long old_dp = dp[rest][neigh];
                    long long new_dp = dp[cost % COST_MOD][node] + teleport_cost;

                    // Check if we can improve dp value
                    if (old_dp > new_dp)
                    {
                        // Update dp and push the new node in the queue
                        dp[rest][neigh] = new_dp;
                        pq.push({neigh, new_dp});
                    }
                }
            }
        }
    }

    long long get_result()
    {

        initialise_dp();

        pq.push({1, 0});

        while (!pq.empty())
        {
            int node = pq.top().node;
            long long cost = pq.top().cost;
            pq.pop();

            if (node == nr_nodes)
            {
                break;
            }

            update_dp_normal_edge(node, cost);

            update_dp_special_edge(node, cost);
        }

        return find_min_dp();
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
