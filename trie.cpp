#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace enjoy
{

struct TrieNode
{
    string val;
    map< char, unique_ptr<TrieNode> > children;
};

class Trie
{
public:
    Trie():
        root_(unique_ptr<TrieNode>(new TrieNode))
    {
    }
    void insert(const string& s)
    {
        int len = s.length();
        if (len <= 0) return;

        for (int i = 0; i < len; i++)
        {
            TrieNode* node = _find(s, i, len-i+1);
            node->val = s;
        }
    }

    void erase(const string& s)
    {
        int n = s.length();
        if (n <= 0) return;

        for (int i = 0; i < n; i++)
        {
            TrieNode* node = _find(s, i, n-i+1);
            node->val.clear();
        }
    }

    int find(const string& s, vector<string> result, int result_n)
    {
        TrieNode* node = _find(s, 0, s.length()-1);
        if (!node->val.empty()) 
        {
            result.push_back(node->val);
            return 1;
        }

        int count = 0;
        for (auto& kv : node->children)
        {
            TrieNode* node = kv.second.get();
            _search_leaf(node, result);
        }

        return result.size();
    }

private:
    TrieNode* _find(const string& s, int index, int len)
    {
        TrieNode *node = root_.get();
        for (int i = 0; i<len; i++) 
        {
            char c = s[index+i];
            if (!node->children[c])
            {
                node->children[c] = unique_ptr<TrieNode>(new TrieNode);
                node = node->children[c].get();
            }
        }
        return node;
    }

    int _search_leaf(TrieNode* node, vector<string> result)
    {
        if (!node->val.empty()) 
        {
            result.push_back(node->val);
        }
        else
        {
            for (auto& kv : node->children) 
            {
                TrieNode* node = kv.second.get();
                _search_leaf(node, result);
            }
        }
    }

private:
    unique_ptr<TrieNode> root_;
};

};

int main()
{
    enjoy::Trie trie;
    trie.insert("hello world");
    vector<string> result;
    cout << trie.find("hello", result, 1) << endl;

    trie.erase("hello world");

    vector<string> result1;
    cout << trie.find("hello", result1, 1) << endl;
}
