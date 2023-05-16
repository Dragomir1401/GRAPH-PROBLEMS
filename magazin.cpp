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
    int nr_storages, nr_queries;
    vector<int> adj[NMAX];          // adjancency list
    vector<int> dfs;                // dfs traversal
    vector<pair<int, int>> queries; // queries[i] = {src, steps}
    vector<bool> visited;           // visited[i] = true if node i was visited
    vector<int> number_of_children; // number_of_children[i] = number of children of node i
    vector<int> pos;                // pos[i] = position of node i in dfs traversal

    void read_input()
    {
        ifstream fin("magazin.in");
        fin >> nr_storages >> nr_queries;
        for (int i = 2, x; i <= nr_storages; i++)
        {
            fin >> x;
            adj[x].push_back(i);
        }

        for (int i = 1, x, y; i <= nr_queries; i++)
        {
            fin >> x >> y;
            queries.push_back({x, y});
        }

        fin.close();
    }

    void DFS(int src)
    {
        // Mark the current node as discovered
        visited[src] = true;

        // Add it to dfs
        dfs.push_back(src);

        // For each node `u` adjacent to `src`
        for (int u : adj[src])
        {
            // If `u` is not yet discovered
            if (!visited[u])
            {
                // Make dfs from `u`
                DFS(u);

                // Update number of children
                number_of_children[src] += number_of_children[u] + 1;
            }
        }
    }

    vector<int> get_result()
    {
        vector<int> result;

        // Initialize visited, number_of_children and pos
        visited = vector<bool>(nr_storages + 1, false);
        number_of_children = vector<int>(nr_storages + 1, 0);
        pos = vector<int>(nr_storages + 1, 0);

        // Make dfs from first node
        DFS(1);

        // Create the pos array
        // pos[i] = position of node i in dfs traversal
        for (int i = 0; i < dfs.size(); i++)
        {
            pos[dfs[i]] = i;
        }

        // For each query
        for (int i = 0; i < nr_queries; i++)
        {
            // Get the source and the number of steps
            int src = queries[i].first;
            int steps = queries[i].second;

            // If we make more than `number_of_children[src]` steps then we will get out of
            // the subtree of `src`, so the destination will be -1
            int dest = steps > number_of_children[src] ? -1 : dfs[pos[src] + steps];

            // Add the destination to the result
            result.push_back(dest);
        }

        return result;
    }

    void
    print_output(vector<int> result)
    {
        ofstream fout("magazin.out");
        for (int i = 0; i < result.size(); i++)
        {
            fout << result[i] << "\n";
        }
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
