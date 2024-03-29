#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

class Task {
	public:
		void solve() {
			read_input();
			print_output(get_result());
		}

	private:
		static constexpr int NMAX = (int)1e5 + 5;
		int nr_stations, nr_paths, source;
		// adjancency list
		vector<int> adj[NMAX];
		// adjancency list for condensed graph
		vector<int> adj_condense[NMAX];
		// map node to scc containing it
		unordered_map<int, int> scc_containing_node_map;
		// parent[i] = parent of node i in dfs tree
		vector<int> parent;
		// vis[i] = true if node i was visited
		vector<bool> vis;
		// low_link[i] = lowest timestamp of node i
		vector<int> low_link;
		// stack used in tarjan
		stack<int> in_stack;
		// all sccs in the graph
		vector<vector<int>> all_sccs;
		// timestamp used in tarjan
		int timestamp = 0;

		void read_input() {
			ifstream fin("ferate.in");
			fin >> nr_stations >> nr_paths >> source;
			for (int i = 1, x, y; i <= nr_paths; i++) {
				fin >> x >> y;
				adj[x].push_back(y);
			}
			fin.close();
		}

		void tarjan(int node, vector<vector<int>> &all_sccs) {
			// Tarjan algorithm for finding all sccs

			// set parent and low_link
			parent[node] = low_link[node] = ++timestamp;
			// push node in stack
			in_stack.push(node);
			// mark node as visited
			vis[node] = true;

			// for each neighbour
			for (long unsigned int i = 0; i < adj[node].size(); i++) {
				// if parent is not set
				if (parent[adj[node][i]] == 0) {
					tarjan(adj[node][i], all_sccs);
					// update low_link
					low_link[node] = min(low_link[node],
										 low_link[adj[node][i]]);
				} else if (vis[adj[node][i]] == true) {
				 	// update low_link
					low_link[node] = min(low_link[node],
										 low_link[adj[node][i]]);
				}
			}

			// if node is the root of a scc
			if (low_link[node] == parent[node]) {
				vector<int> scc;
				int aux;

				// Pop nodes from stack until node is found
				do {
					// Pop node from stack
					aux = in_stack.top();
					in_stack.pop();

					// Add node to scc
					scc.push_back(aux);

					// Update scc_containing_node_map
					scc_containing_node_map[aux] = all_sccs.size() + 1;

					// Mark node as not visited
					vis[aux] = false;
				} while (aux != node);

				// Add scc to all_sccs
				all_sccs.push_back(scc);
			}
		}

		int find_number_of_adding_edges() {
			// Build condensed graph
			vector<int> in_degree(all_sccs.size() + 1, 0);

			// For each node
			for (int i = 1; i <= nr_stations; i++) {
				// Get scc containing node
				int scc = scc_containing_node_map[i];

				// For each neigh
				for (long unsigned int j = 0; j < adj[i].size(); j++) {
					// Get scc containing neighbour
					int scc_neigh = scc_containing_node_map[adj[i][j]];

					// If the neighbour is not in the same scc
					if (scc != scc_neigh) {
						// It means that there is an edge
						// between scc and scc_neigh
						in_degree[scc_neigh]++;
					}
				}
			}

			int nr_adding_edges = 0;

			// Get scc containing source
			long unsigned int scc_src = scc_containing_node_map[source];

			// Count the number of sccs that have in_degree 0
			for (long unsigned int i = 1; i <= all_sccs.size(); i++) {
				// If the scc is not the scc containing source
				// and it has in_degree 0
				if (in_degree[i] == 0 && i != scc_src) {
					nr_adding_edges++;
				}
			}

			return nr_adding_edges;
		}

		int get_result() {
			parent = vector<int>(nr_stations + 1, 0);
			vis = vector<bool>(nr_stations + 1, false);
			low_link = vector<int>(nr_stations + 1, 0);

			for (int node = 1; node <= nr_stations; node++) {
				// If parent is not set
				if (parent[node] == 0) {
					tarjan(node, all_sccs);
				}
			}

			return find_number_of_adding_edges();
		}

		void
		print_output(int result) {
			ofstream fout("ferate.out");
			fout << result << '\n';
			fout.close();
		}
	};

	int main() {
		auto *task = new (nothrow) Task();
		if (!task) {
			cerr << "new failed: WTF are you doing? Throw your PC!\n";
			return -1;
		}
		task->solve();
		delete task;
		return 0;
	}
