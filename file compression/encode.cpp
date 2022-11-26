#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

string data;

class node
{
public:
    node *left;
    node *right;
    char val;
    ll freq;
    node(ll f, char d)
    {
        left = NULL;
        right = NULL;
        val = d;
        freq = f;
    }
};

class comp
{
public:
    bool operator()(node *n1, node *n2)
    {
        return n1->freq > n2->freq;
    }
};

// finding codes
void getMappings(node *root, map<char, string> &table, string s)
{
    if (root->left == NULL and root->right == NULL)
    {
        table[root->val] = s;
        return;
    }

    getMappings(root->left, table, s + '0');
    getMappings(root->right, table, s + '1');
}

// flattening a tree
string convertToString(node *root)
{
    if (root->left == NULL and root->right == NULL)
    {
        string toReturn;
        toReturn.push_back('-');
        toReturn.push_back(root->val);
        return toReturn;
    }

    return ('0' + convertToString(root->left) + '1' + convertToString(root->right));
}

int main(int argv, char *argc[])
{

    if (argv == 1)
    {
        cout << "Invalid Input file" << endl;
        return 0;
    }

    // Read text from file
    string inputFileName = argc[1];

    ifstream file(inputFileName);
    if (file)
    {
        ostringstream buffer;
        buffer << file.rdbuf();
        data = buffer.str();
    }
    else
    {
        cout << "File not found" << endl;
        return 0;
    }
    file.close();
    string outputFilename = "compressed_" + inputFileName.substr(0, inputFileName.length() - 4) + ".huf";
    // cout<<"Read the Input file"<<endl;

    // Count frequencies of all chars
    map<char, ll> freqmap;
    ll n = data.length();
    for (ll i = 0; i < n; i++)
    {
        freqmap[data[i]]++;
    }

    // Push nodes into min Heap
    priority_queue<node *, vector<node *>, comp> minHeap;
    for (auto it = freqmap.begin(); it != freqmap.end(); it++)
    {
        node *n1 = new node(it->second, it->first);
        minHeap.push(n1);
    }
    // cout<<"Constructed min Heap"<<endl;

    // Form huffman tree
    while (minHeap.size() > 1)
    {
        node *n1 = minHeap.top();
        minHeap.pop();
        node *n2 = minHeap.top();
        minHeap.pop();

        node *root = new node(n1->freq + n2->freq, '\0');
        root->left = n1;
        root->right = n2;
        minHeap.push(root);
    }
    // cout<<"Built Huffman tree"<<endl;

    // huffman root
    node *root = minHeap.top();
    minHeap.pop();

    // conversion table
    map<char, string> table;
    getMappings(root, table, "");
    // cout<<"Code mappings are done. Table is ready"<<endl;

    // construct the resultant ascii string by reading 8 bits
    string res = "";
    string s = "";
    for (ll i = 0; i < n; i++)
    {
        s += table[data[i]];
        while (s.length() > 8)
        {
            ll x = 0;
            for (ll j = 7; j >= 0; j--)
            {
                x = x * 2 + (s[j] - '0');
            }
            s = s.substr(8);
            res += x;
        }
    }

    //////// Padding ////////
    ll rem = 8 - s.length();
    for (ll i = 0; i < rem; i++)
        s.push_back('0');
    ll x = 0;
    for (ll j = 7; j >= 0; j--)
    {
        x = x * 2 + (s[j] - '0');
    }
    res += x;

    // cout<<"Compressed string is ready"<<endl;

    string huffmanString = convertToString(root);
    ll hlen = huffmanString.length();

    string toWrite = to_string(rem) + '\n' + to_string(hlen) + '\n' + huffmanString + res;

    ofstream newFile(outputFilename);
    newFile << toWrite;
    newFile.close();
    cout << "Compression Successful" << endl;

    double compRatio = double(n) / double(toWrite.length());
    cout << "Compression ratio is: " << compRatio << endl;

    cout << "Written to output file named " + outputFilename << endl;
}