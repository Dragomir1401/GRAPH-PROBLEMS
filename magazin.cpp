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
    vector<int> adj[NMAX];
    vector<int> dfs;
    vector<pair<int, int>> queries;
    vector<bool> visited;
    vector<int> number_of_children;
    vector<int> pos;

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
        // mark the current node as discovered
        visited[src] = true;

        // add to dfs
        dfs.push_back(src);

        // do for every edge (v, u)
        for (int u : adj[src])
        {
            // if `u` is not yet discovered
            if (!visited[u])
            {
                // update number of children
                DFS(u);
                number_of_children[src] += number_of_children[u] + 1;
            }
        }
    }

    vector<int> get_result()
    {
        vector<int> result;
        // put false in visited vector
        visited = vector<bool>(nr_storages + 1, false);
        // put 0 in number_of_children vector
        number_of_children = vector<int>(nr_storages + 1, 0);
        // put 0 in pos vector
        pos = vector<int>(nr_storages + 1, 0);

        // Make dfs from 1
        DFS(1);

        for (int i = 0; i < dfs.size(); i++)
        {
            pos[dfs[i]] = i;
        }

        for (int i = 0; i < nr_queries; i++)
        {
            int src = queries[i].first;
            int steps = queries[i].second;

            // If pos is outside the number of children it has, then its -1
            int dest = steps > number_of_children[src] ? -1 : dfs[pos[src] + steps];

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
