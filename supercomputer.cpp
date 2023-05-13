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

    int nr_tasks, nr_dependencies;

    vector<int> adj[NMAX];
    vector<int> data_set_needed;

    void read_input()
    {
        ifstream fin("supercomputer.in");
        fin >> nr_tasks >> nr_dependencies;
        data_set_needed.resize(nr_tasks + 1);

        for (int i = 1; i <= nr_tasks; i++)
        {
            fin >> data_set_needed[i];
        }

        for (int i = 1, x, y; i <= nr_dependencies; i++)
        {
            fin >> x >> y;
            adj[x].push_back(y);
        }
        fin.close();
    }

    int get_result()
    {
        int min_switches1 = 0;
        int min_switches2 = 0;

        vector<int> in_degree(nr_tasks + 1, 0);
        // Create q of pairs (task, data_set_needed) that is sorted by data_set_needed
        deque<pair<int, int>> q;

        for (int i = 1; i <= nr_tasks; i++)
        {
            for (auto &j : adj[i])
            {
                in_degree[j]++;
            }
        }

        // For each task with in_degree = 0
        for (int i = 1; i <= nr_tasks; i++)
        {
            if (in_degree[i] == 0)
            {
                if (data_set_needed[i] == 1)
                {
                    q.push_front({i, data_set_needed[i]});
                }
                else if (data_set_needed[i] == 2)
                {
                    q.push_back({i, data_set_needed[i]});
                }
            }
        }
        // deep copy the q
        deque<pair<int, int>> q_copy = q;
        // deep copy the in_degree
        vector<int> in_degree_copy = in_degree;

        cout << "Starting from task " << q_copy.front().first << "with first data set needed.\n";

        int last_task = 0;
        int last_data_set_needed = 0;
        while (!q.empty())
        {
            int task = 0;
            int set_needed;
            if (!last_data_set_needed)
            {
                task = q_copy.front().first;
                set_needed = q_copy.front().second;
                q.pop_front();
            }

            if (last_data_set_needed == 1)
            {
                task = q.front().first;
                set_needed = q.front().second;
                if (set_needed == 2)
                {
                    cout << "Switching from " << last_task << " to " << task << '\n'
                         << "last_data_set_needed = " << last_data_set_needed << '\n'
                         << "set_needed = " << set_needed << '\n'
                         << "min_switches = " << min_switches1 << '\n'
                         << '\n';
                    min_switches1++;
                }
                q.pop_front();
            }
            else if (last_data_set_needed == 2)
            {
                task = q.back().first;
                set_needed = q.back().second;
                if (set_needed == 1)
                {
                    cout << "Switching from " << last_task << " to " << task << '\n'
                         << "last_data_set_needed = " << last_data_set_needed << '\n'
                         << "set_needed = " << set_needed << '\n'
                         << "min_switches = " << min_switches1 << '\n'
                         << '\n';
                    min_switches1++;
                }
                q.pop_back();
            }

            for (auto &i : adj[task])
            {
                in_degree[i]--;
                if (in_degree[i] == 0)
                {
                    if (data_set_needed[i] == 1)
                    {
                        q.push_front({i, data_set_needed[i]});
                    }
                    else if (data_set_needed[i] == 2)
                    {
                        q.push_back({i, data_set_needed[i]});
                    }
                }
            }

            last_task = task;
            last_data_set_needed = data_set_needed[task];
        }

        cout << "Starting from task " << q_copy.back().first << "with second data set needed.\n";
        last_task = 0;
        last_data_set_needed = 0;
        while (!q_copy.empty())
        {
            int task = 0;
            int set_needed;
            if (!last_data_set_needed)
            {
                task = q_copy.back().first;
                set_needed = q_copy.back().second;
                q_copy.pop_back();
            }

            if (last_data_set_needed == 1)
            {
                task = q_copy.front().first;
                set_needed = q_copy.front().second;
                if (set_needed == 2)
                {
                    cout << "Switching from " << last_task << " to " << task << '\n'
                         << "last_data_set_needed = " << last_data_set_needed << '\n'
                         << "set_needed = " << set_needed << '\n'
                         << "min_switches = " << min_switches2 << '\n'
                         << '\n';
                    min_switches2++;
                }
                q_copy.pop_front();
            }
            else if (last_data_set_needed == 2)
            {
                task = q_copy.back().first;
                set_needed = q_copy.back().second;
                if (set_needed == 1)
                {
                    cout << "Switching from " << last_task << " to " << task << '\n'
                         << "last_data_set_needed = " << last_data_set_needed << '\n'
                         << "set_needed = " << set_needed << '\n'
                         << "min_switches = " << min_switches2 << '\n'
                         << '\n';
                    min_switches2++;
                }
                q_copy.pop_back();
            }

            for (auto &i : adj[task])
            {
                in_degree_copy[i]--;
                if (in_degree_copy[i] == 0)
                {
                    if (data_set_needed[i] == 1)
                    {
                        q_copy.push_front({i, data_set_needed[i]});
                    }
                    else if (data_set_needed[i] == 2)
                    {
                        q_copy.push_back({i, data_set_needed[i]});
                    }
                }
            }

            last_task = task;
            last_data_set_needed = data_set_needed[task];
        }

        return min(min_switches1, min_switches2);
    }

    void
    print_output(int min_switches)
    {
        ofstream fout("supercomputer.out");
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
