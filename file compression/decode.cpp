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
    node(char d)
    {
        left = NULL;
        right = NULL;
        val = d;
    }
};

/////// Check if the input file is '.huf' format ////////
bool dothuf(string s)
{
    reverse(s.begin(), s.end());
    if (s.substr(0, 4) != "fuh.")
    {
        return false;
    }
    return true;
}

////////Building the Huffman Tree//////////
node *buildTree(string &huffmanTree, ll hlen, ll &p)
{
    if (p >= hlen)
    {
        return NULL;
    }

    if (huffmanTree[p] == '-')
    {
        p++;
        node *root = new node(huffmanTree[p]);
        p++;
        return root;
    }
    else
    {
        node *root = new node('\0');

        if (huffmanTree[p] == '0')
        {
            p++;
            root->left = buildTree(huffmanTree, hlen, p);
        }
        if (huffmanTree[p] == '1')
        {
            p++;
            root->right = buildTree(huffmanTree, hlen, p);
        }
        return root;
    }
}

/////// Returns binary string of 8 bit size corresponding to a number ////////
string toBinary(ll x)
{
    string s(8, '0');
    for (ll i = 0; i < 8; i++)
    {
        if (x & (1ll << i))
        {
            s[i] = '1';
        }
    }
    return s;
}

int main(int argv, char *argc[])
{
    if (argv == 1)
    {
        cout << "Invalid Input file" << endl;
        return 0;
    }

    string inputFileName = argc[1];
    string outputFilename = "output.txt";
    if (argv == 3)
        outputFilename = argc[2];

    if (!dothuf(inputFileName))
    {
        cout << "Invalid Input file" << endl;
        return 0;
    }

    //////Reading Encoded Input////////
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

    ///////length of input////////
    ll n = data.length();

    //////Extracting padding and huffman tree length////////
    string s = "";
    ll p = 0;
    while (p < n and data[p] != '\n')
    {
        s.push_back(data[p]);
        p++;
    }
    ll padding = stoll(s);

    p++;
    s = "";
    while (p < n and data[p] != '\n')
    {
        s.push_back(data[p]);
        p++;
    }
    ll hlen = stoll(s);
    p++;

    /////////Extracting huffman tree/////////
    string huffmanTree = "";
    for (ll i = p; i < p + hlen; i++)
    {
        huffmanTree.push_back(data[i]);
    }

    /////// Extracting Encoded data //////////
    data = data.substr(p + hlen, n - p - hlen);

    //////////Building the Huffman Tree ////////////
    p = 0;
    node *root = buildTree(huffmanTree, hlen, p);

    ////////// Finding the resultant string ////////
    string res = "";

    n = data.length();
    s = "";
    node *cur = root;

    for (ll i = 0; i < n; i++)
    {
        s = toBinary(data[i]);
        // at max each char in original will have code of 8 bit size
        // so makes sense to check every 8 bit string

        if (i == n - 1)
        {
            s = s.substr(0, 8 - padding);
        }

        for (ll j = 0; j < s.size(); j++)
        {
            if (s[j] == '0')
            {
                cur = cur->left;
            }
            else
            {
                cur = cur->right;
            }

            if (cur->left == NULL and cur->right == NULL)
            {
                res.push_back(cur->val);
                cur = root;
            }
        }
    }

    ofstream newFile(outputFilename);
    newFile << res;
    newFile.close();

    cout << "File Restored Successfully" << endl;
    cout << "Written to output file, named " + outputFilename << endl;
}