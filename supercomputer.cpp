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

    vector<int> adj[NMAX];       // adjancency list
    vector<int> data_set_needed; // data_set_needed[i] = 1 if task i needs data set 1, 2 if task i needs data set 2

    void read_input()
    {
        // Parse input
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

    void make_in_degree(vector<int> &in_degree)
    {
        // For each task, compute in_degree
        for (int i = 1; i <= nr_tasks; i++)
        {
            for (auto &j : adj[i])
            {
                in_degree[j]++;
            }
        }
    }

    void add_nodes_with_in_degree0(vector<int> &in_degree, deque<pair<int, int>> &q)
    {
        for (int i = 1; i <= nr_tasks; i++)
        {
            // Just for the the nodes with in degree = 0
            if (in_degree[i] == 0)
            {
                // Those with data_set_needed = 1 go in the front of the q
                if (data_set_needed[i] == 1)
                {
                    q.push_front({i, data_set_needed[i]});
                }
                // Those with data_set_needed = 2 go in the back of the q
                else if (data_set_needed[i] == 2)
                {
                    q.push_back({i, data_set_needed[i]});
                }
            }
        }
    }

    void update_queue(deque<pair<int, int>> &q, vector<int> &in_degree, int task)
    {
        // Add the nodes that have in_degree = 0 after we remove a node
        for (auto &i : adj[task])
        {
            // We remove a node, so we decrease the in_degree of the nodes that depend on it
            in_degree[i]--;
            if (in_degree[i] == 0)
            {
                // Those with data_set_needed = 1 go in the front of the q
                if (data_set_needed[i] == 1)
                {
                    q.push_front({i, data_set_needed[i]});
                }
                // Those with data_set_needed = 2 go in the back of the q
                else if (data_set_needed[i] == 2)
                {
                    q.push_back({i, data_set_needed[i]});
                }
            }
        }
    }

    void start_with_first_dataset(deque<pair<int, int>> &q, vector<int> &in_degree, int &min_switches1)
    {
        int last_task = 0;
        int last_data_set_needed = 0;

        while (!q.empty())
        {
            int task = 0;
            int set_needed;

            // If we don't have a last task, we take the first task from the queue so it needs
            //  the first data set
            if (!last_data_set_needed)
            {
                task = q.front().first;
                set_needed = q.front().second;
                q.pop_front();
            }
            // If we have a last task, we take the task from the front of the queue that needs
            // the same data set as the last task
            else if (last_data_set_needed == 1)
            {
                task = q.front().first;
                set_needed = q.front().second;
                q.pop_front();

                // If the task needs data set 2, we need to switch
                if (set_needed == 2)
                {
                    min_switches1++;
                }
            }
            // If we have a last task, we take the task from the back of the queue that needs
            // the same data set as the last task
            else if (last_data_set_needed == 2)
            {
                task = q.back().first;
                set_needed = q.back().second;
                q.pop_back();

                // If the task needs data set 1, we need to switch
                if (set_needed == 1)
                {
                    min_switches1++;
                }
            }

            // Update the queue after we remove a node
            update_queue(q, in_degree, task);

            // Update the last task and the last data set needed
            last_task = task;
            last_data_set_needed = set_needed;
        }
    }

    void start_with_second_dataset(deque<pair<int, int>> &q_copy, vector<int> &in_degree_copy,
                                   int &min_switches2)
    {
        int last_task = 0;
        int last_data_set_needed = 0;

        while (!q_copy.empty())
        {
            int task = 0;
            int set_needed;

            // If we don't have a last task, we take the last task from the queue so it needs
            //  the second data set
            if (!last_data_set_needed)
            {
                task = q_copy.back().first;
                set_needed = q_copy.back().second;
                q_copy.pop_back();
            }
            // If we have a last task, we take the task from the front of the queue that needs
            // the same data set as the last task
            else if (last_data_set_needed == 1)
            {
                task = q_copy.front().first;
                set_needed = q_copy.front().second;
                q_copy.pop_front();

                // If the task needs data set 2, we need to switch
                if (set_needed == 2)
                {
                    min_switches2++;
                }
            }
            // If we have a last task, we take the task from the back of the queue that needs
            // the same data set as the last task
            else if (last_data_set_needed == 2)
            {
                task = q_copy.back().first;
                set_needed = q_copy.back().second;
                q_copy.pop_back();

                // If the task needs data set 1, we need to switch
                if (set_needed == 1)
                {
                    min_switches2++;
                }
            }

            // Update the queue after we remove a node
            update_queue(q_copy, in_degree_copy, task);

            // Update the last task and the last data set needed
            last_task = task;
            last_data_set_needed = data_set_needed[task];
        }
    }

    int get_result()
    {
        int min_switches1 = 0;
        int min_switches2 = 0;

        // Create in_degree vector
        vector<int> in_degree(nr_tasks + 1, 0);

        // Create q of pairs (task, data_set_needed) that is sorted by
        // data_set_needed
        deque<pair<int, int>> q;

        // For each task, compute in_degree
        make_in_degree(in_degree);

        // For each task with in_degree = 0
        add_nodes_with_in_degree0(in_degree, q);

        // Make a copy of the q
        deque<pair<int, int>> q_copy = q;

        // Make a copy of the in_degree vector
        vector<int> in_degree_copy = in_degree;

        // Start with the first data set and compute the minimum number of switches
        start_with_first_dataset(q, in_degree, min_switches1);

        // Start with the second data set and compute the minimum number of switches
        start_with_second_dataset(q_copy, in_degree_copy, min_switches2);

        // Return the minimum number of switches between the two data sets
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
