#include "MinHeap.h"
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

int find_heuristic(string s1, string s2) {
    int heur = 0;
    for (int i = 0; i < (int)s1.size(); i++) {
        if (s1[i] != s2[i]) {
            heur++;
        }
    }
    return heur;
}

int main(int argc, char* argv[]) {
    // stores the command line arguments
    string word1 = argv[1];
    for (int i = 0; i < (int)word1.length(); i++) {
        word1[i] = toupper(word1[i]);
    }
    string word2 = argv[2];
    for (int i = 0; i < (int)word2.length(); i++) {
        word2[i] = toupper(word2[i]);
    }

    // read in the file
    int num_lines;
    string curr_word;
    map<string, int> words;
    ifstream text_file;
    text_file.open(argv[3]);
    // error handling
    if (!text_file) {
        cout << "Couldn't open file." << endl;
        return 0;
    }
    // store number of lines
    text_file >> num_lines;
    // get rid of first line
    string garbage;
    getline(text_file, garbage);
    // store each word in file in a map - allows us to look up if each possible word is in our file in log(n) time
    for (int i = 1; i <= num_lines; i++) {
        text_file >> curr_word >> ws;
        // turn to lowercase first !!!
        for (int i = 0; i < (int)curr_word.length(); i++) {
            curr_word[i] = toupper(curr_word[i]);
        }
        // cout << curr_word << endl;
        words.insert(pair<string, int>(curr_word, i));
    }
    text_file.close();

    // building a graph using an adjacency list
    map<string, vector<string> > graphs;
    map<string, int>::iterator it;
    // iterate through each word in the list
    for (it = words.begin(); it != words.end(); it++) {
        // iterate through the length of each word
        for (int i = 0; i < (int)it->first.length(); i++) {
            // change the current position to each letter and check if it's in the map AND it isn't the same as the
            // original word
            string word_copy = it->first;
            for (int j = 0; j < 26; j++) {
                word_copy[i] = (char)j + 65;
                if (word_copy != it->first && word_copy != word1 && words.find(word_copy) != words.end()) {
                    // add the new word into the adjacency list
                    graphs[it->first].push_back(word_copy);
                }
            }
        }
    }

    // a-star algorithm
    MinHeap<string> pq(2);

    vector<int> dist;
    int expansions = 0;

    for (int i = 0; i <= num_lines; i++) {
        dist.push_back(INT_MAX);
    }
    set<string> visited;
    // i used the line numbers from the words vector to access the specific index of a vertex
    dist[words[word1]] = 0;
    pq.add(word1, dist[words[word1]]);
    // cout << pq.peek() << endl;

    while (!pq.isEmpty()) {
        string peeked = pq.peek();
        pq.remove();
        // if (curr_dist[words[peeked]] >= dist[words[peeked]])
        if (visited.find(peeked) == visited.end()) {
             cout << "peeked: " << peeked << endl;
            // if it's the final word print the shit out
            if (peeked == word2) {
                cout << dist[words[peeked]] << endl;
                cout << expansions << endl;
                return 0;
            }
            expansions++;
            for (int i = 0; i < (int)graphs[peeked].size(); i++) {
                string neighbor = graphs[peeked][i];
                int heurist = find_heuristic(word2, neighbor);
                // if they're the same size and if it hasn't been visited or there's a shorter path then update
                if (neighbor.size() == peeked.size()
                    && (visited.find(neighbor) == visited.end() || dist[words[peeked]] + 1 < dist[words[neighbor]])) {
                    // if it's an uninitialized distance or there's a shorter path then update it - otherwise keep the
                    // same distance as before
                    if (dist[words[neighbor]] == INT_MAX || dist[words[peeked]] + 1 < dist[words[neighbor]]) {
                        dist[words[neighbor]] = dist[words[peeked]] + 1;
                    }
                    // cout << "g of neighbor: " << dist[words[neighbor]] << endl;
                    int f = dist[words[neighbor]] + heurist;
                    pq.add(neighbor, f * (peeked.size() + 1) + heurist);
                    cout << "priority of " << neighbor << ": " << f*(peeked.size()+1)+heurist << endl;
                }
            }
            // set as visited
            visited.insert(peeked);
        }
    }
    // if there's no transformation available
    cout << "No transformation" << endl;
    cout << expansions << endl;
}
