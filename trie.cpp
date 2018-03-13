#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>

#include "spinlock.h"

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
        root_(unique_ptr<TrieNode>(new TrieNode)),
        lock_(unique_ptr<Spinlock>(new Spinlock))
    {
    }

    void insert(const string& s)
    {
        int n = s.length();
        if (n <= 0) return;

        for (int i = 0; i < n; i++)
        {
            TrieNode* node = _find(s, i, n-i);
            node->val = s;
        }
    }

    void erase(const string& s)
    {
        int n = s.length();
        if (n <= 0) return;

        for (int i = 0; i < n; i++)
        {
            TrieNode* node = _find(s, i, n-i);
            node->val.clear();
        }
    }

    int find(const string& s, vector<string> &result, int result_n)
    {
        TrieNode* node = _find(s, 0, s.length());
        if (!node->val.empty()) 
        {
            result.push_back(node->val);
            return 1;
        }

        int count = 0;
        for (auto& kv : node->children)
        {
            TrieNode* node = kv.second.get();
            _search_leaf(node, result, result_n);

            if (result_n <= 0) break;
        }

        return result.size();
    }

    void debug_print()
    {
        cout << "root:" << root_.get() << root_->val << endl;
        _debug_print(root_.get(), "");
    }

private:
    TrieNode* _find(const string& s, int index, int len)
    {
        TrieNode *node = root_.get();
        for (int i = 0; i < len; i++)
        {
            char c = s[index+i];
            if (!node->children[c])
            {
                lock_->lock();
                if (!node->children[c])
                {
                    node->children[c] = unique_ptr<TrieNode>(new TrieNode);
                }
                lock_->unlock();
            }

            node = node->children[c].get();
        }

        return node;
    }

    void _search_leaf(TrieNode* node, vector<string> &result, int &left)
    {
        if (left <= 0) 
            return;

        if (!node->val.empty()) 
        {
            result.push_back(node->val);
            --left;
        }
        else
        {
            for (auto& kv : node->children) 
            {
                TrieNode* node = kv.second.get();
                _search_leaf(node, result, left);
                if (left <= 0) 
                    return;
            }
        }
    }

    void _debug_print(TrieNode *node, string blank)
    {
        blank.append("    ");
        for (auto& kv : node->children)
        {
            TrieNode *node = kv.second.get();
            cout << blank << kv.first << ":" << node->val << "@" << node << endl;
            _debug_print(kv.second.get(), blank);
        }
    }

private:
    unique_ptr<TrieNode> root_;
    unique_ptr<Spinlock> lock_;
};

};

int main()
{
    enjoy::Trie trie;
    trie.insert("hello world");
    trie.insert("hello world 1");
    trie.debug_print();

    vector<string> result;
    cout << trie.find("hello world", result, 1) << endl;
    for (auto& v : result)
    {
        cout << "::::" << v << endl;
    }

    trie.erase("hello world");

    vector<string> result1;
    cout << trie.find("hello world", result1, 1) << endl;
    for (auto& v : result1)
    {
        cout << "::::" << v << endl;
    }
}
