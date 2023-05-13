#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

class Task
{
public:
    void solve()
    {
        read_input();
        print_output(get_result());
    }

private:
    static constexpr int NMAX = (int)1e5 + 5;
    int nr_stations, nr_paths, source;
    vector<int> adj[NMAX];
    vector<int> adj_condense[NMAX];
    map<int, int> scc_containing_node_map;
    vector<int> parent;
    vector<bool> vis;
    vector<int> low_link;
    stack<int> in_stack;
    vector<vector<int>> all_sccs;
    int timestamp = 0;

    void read_input()
    {
        ifstream fin("ferate.in");
        fin >> nr_stations >> nr_paths >> source;

        for (int i = 1, x, y; i <= nr_paths; i++)
        {
            fin >> x >> y;
            adj[x].push_back(y);
        }
        fin.close();
    }

    void tarjan(int node, vector<vector<int>> &all_sccs)
    {
        parent[node] = low_link[node] = ++timestamp;
        in_stack.push(node);
        vis[node] = true;

        for (long unsigned int i = 0; i < adj[node].size(); i++)
        {
            if (parent[adj[node][i]] == 0)
            {
                tarjan(adj[node][i], all_sccs);
                low_link[node] = min(low_link[node], low_link[adj[node][i]]);
            }
            else if (vis[adj[node][i]] == true)
            {
                low_link[node] = min(low_link[node], low_link[adj[node][i]]);
            }
        }

        if (low_link[node] == parent[node])
        {
            vector<int> scc;
            int aux;
            do
            {
                aux = in_stack.top();
                scc.push_back(aux);
                scc_containing_node_map[aux] = all_sccs.size() + 1;
                in_stack.pop();
                vis[aux] = false;
            } while (aux != node);

            all_sccs.push_back(scc);
        }
    }

    int find_number_of_adding_edges()
    {
        // Find all nodes with 0 in degree
        vector<int> in_degree(all_sccs.size() + 1, 0);

        for (int i = 1; i <= nr_stations; i++)
        {
            int scc = scc_containing_node_map[i];
            for (int j = 0; j < adj[i].size(); j++)
            {
                int scc_neigh = scc_containing_node_map[adj[i][j]];

                // If the neighbour is not in the same scc
                if (scc != scc_neigh)
                {
                    in_degree[scc_neigh]++;
                }
            }
        }

        int nr_adding_edges = 0;
        int scc_src = scc_containing_node_map[source];
        for (int i = 1; i <= all_sccs.size(); i++)
        {
            if (in_degree[i] == 0 && i != scc_src)
            {
                nr_adding_edges++;
            }
        }

        return nr_adding_edges;
    }

    int get_result()
    {
        parent = vector<int>(nr_stations + 1, 0);
        vis = vector<bool>(nr_stations + 1, false);
        low_link = vector<int>(nr_stations + 1, 0);

        for (int node = 1; node <= nr_stations; node++)
        {
            if (parent[node] == 0)
            {
                tarjan(node, all_sccs);
            }
        }

        return find_number_of_adding_edges();
    }

    void
    print_output(int min_switches)
    {
        ofstream fout("ferate.out");
        fout << min_switches << '\n';
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
