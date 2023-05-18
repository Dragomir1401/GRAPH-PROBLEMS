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
	long cost;
};

// Binary heap implementation for priority queue
struct BinaryHeap
{
	static const int MAX_SIZE = 1e5 + 5;

	Edge heap[MAX_SIZE];
	int size;

	BinaryHeap() : size(0) {}

	// Push an element to the heap
	void push(const Edge &edge)
	{
		heap[size++] = edge;
		shiftUp(size - 1);
	}

	// Get the top element
	Edge top() const
	{
		return heap[0];
	}

	// Pop the top element
	void pop()
	{
		heap[0] = heap[--size];
		shiftDown(0);
	}

	// Check if the heap is empty
	bool empty() const
	{
		return size == 0;
	}

private:
	// Swap two elements
	void shiftUp(int index)
	{
		while (index > 0)
		{
			int parent = (index - 1) / 2;
			if (heap[parent].cost <= heap[index].cost)
			{
				break;
			}
			swap(heap[parent], heap[index]);
			index = parent;
		}
	}

	// Shift down an element
	void shiftDown(int index)
	{
		while (2 * index + 1 < size)
		{
			int leftChild = 2 * index + 1;
			int rightChild = 2 * index + 2;
			int smallestChild = leftChild;
			if (rightChild < size && heap[rightChild].cost <
										 heap[leftChild].cost)
			{
				smallestChild = rightChild;
			}
			if (heap[index].cost <= heap[smallestChild].cost)
			{
				break;
			}
			swap(heap[index], heap[smallestChild]);
			index = smallestChild;
		}
	}
};

static constexpr int NMAX = (int)1e5 + 5;
static constexpr int MAX_PERIOD = (int)8;
static constexpr long DP_MAX = 1e14;
static constexpr int COST_MOD = 360;
long dp[COST_MOD][NMAX];

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
	// adjancency list for normal Edges
	vector<pair<int, int>> adj[NMAX];
	// adjancency list for special Edges
	vector<pair<int, int>> special_adj[NMAX];
	BinaryHeap pq;

	void read_input()
	{
		ifstream fin("teleportare.in");
		fin >> nr_nodes >> nr_Edges >> nr_special_Edges;

		for (int i = 0; i < NMAX; i++)
		{
			adj[i].reserve(nr_Edges);
			special_adj[i].reserve(nr_special_Edges);
		}

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
			special_adj[x].push_back({y, z});
			special_adj[y].push_back({x, z});
		}

		fin.close();
	}

	void initialise_dp()
	{
		// Flatten and fill the 2D array
		std::fill(&dp[0][0], &dp[0][0] + sizeof(dp) / sizeof(dp[0][0]), DP_MAX);

		// Set dp[0][1] to 0
		dp[0][1] = 0;
	}

	long find_min_dp()
	{
		// Find the minimum dp value
		long min = DP_MAX;
		for (long unsigned int i = 0; i < COST_MOD; i++)
		{
			if (dp[i][nr_nodes] < min)
			{
				min = dp[i][nr_nodes];
			}
		}
		return min;
	}

	void update_dp_normal_edge(int node, long cost, int cost_mod)
	{
		for (int i = 0; i < (int)adj[node].size(); i++)
		{
			// Get the normal edge node and cost
			int neigh = adj[node][i].first;
			long edge_cost = adj[node][i].second;

			// Calculate the new cost and dp
			long new_cost = cost + edge_cost;
			int rest = new_cost % COST_MOD;
			long old_dp = dp[rest][neigh];
			long new_dp = dp[cost_mod][node] + edge_cost;

			// Check if we can improve dp value
			if (old_dp <= new_dp)
			{
				continue;
			}

			// Update dp and push the new node in the queue
			dp[rest][neigh] = new_dp;
			pq.push({neigh, new_dp});
		}
	}

	void update_dp_special_edge(int node, long cost, int cost_mod)
	{
		int teleport_cost = 1;
		long new_cost = cost + teleport_cost;
		int rest = new_cost % COST_MOD;

		// For period in periods
		for (int i = 0; i < (int)special_adj[node].size(); i++)
		{
			// Get the special edge node and period
			int neigh = special_adj[node][i].first;
			int period_cost = special_adj[node][i].second;

			// Check if we can use this period
			if (cost % period_cost != 0)
			{
				continue;
			}

			// Calculate the new dp
			long old_dp = dp[rest][neigh];
			long new_dp = dp[cost_mod][node] + teleport_cost;

			// Check if we can improve dp value
			if (old_dp <= new_dp)
			{
				continue;
			}

			// Update dp and push the new node in the queue
			dp[rest][neigh] = new_dp;
			pq.push({neigh, new_dp});
		}
	}

	long get_result()
	{
		// Initialise dp
		initialise_dp();

		// Push the first node in the queue
		pq.push({1, 0});

		// Dijkstra
		while (!pq.empty())
		{
			int node = pq.top().node;
			long cost = pq.top().cost;
			pq.pop();

			int cost_mod = cost % COST_MOD;

			if (cost > dp[cost_mod][node])
			{
				continue;
			}

			// Check if we reached the last node
			if (node == nr_nodes)
			{
				break;
			}

			// Update dp for normal and special Edges
			update_dp_normal_edge(node, cost, cost_mod);
			update_dp_special_edge(node, cost, cost_mod);
		}

		return find_min_dp();
	}

	void
	print_output(long result)
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
