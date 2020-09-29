#include <vector>
#include <iostream>
#include <math.h>
#include <queue>
using namespace std;
typedef pair<string, int> pr;

#define null ""
#define Cm 6
#define Rm 4
#define Lm 3

class Node
{
public:
    string type;
    int dist;
    vector<Node *> child;
    int Cskip, Addr;
    Node(){};
    Node(string type, int dist)
    {
        this->type = type;
        this->dist = dist;
    };

    void countCskip(int dist)
    {
        if (Rm == 1)
        {
            this->Cskip = 1 + Cm * (Lm - dist - 1);
        }
        else
        {
            this->Cskip = (1 + Cm - Rm - Cm * pow(Rm, Lm - dist - 1)) / (1 - Rm);
        }
    }

    void rootAddr(int addr)
    {
        this->Addr = addr;
    }

    void countAddr(string type, int AddrP, int nth, int Cskip)
    {
        if (type[0] == 'r')
        {
            this->Addr = AddrP + (nth - 1) * Cskip + 1;
        }

        else
        {
            this->Addr = AddrP + Rm * Cskip + nth;
        }
    }
};

class Help
{
public:
    Node *BuildTree(vector<pr> &nodes)
    {
        if (nodes.empty() || nodes.size() == 0)
            return 0;
        queue<Node *> treenodequeue;
        queue<pr> q;
        Node *root = new Node(nodes[0].first, nodes[0].second);
        treenodequeue.push(root);
        for (int i = 1; i != nodes.size(); ++i)
        {
            q.push(make_pair(nodes[i].first, nodes[i].second));
        }
        while (!q.empty())
        {
            Node *curr = treenodequeue.front();
            treenodequeue.pop();
            if (q.front().first == null)
                q.pop();
            while (q.front().first != null)
            {
                pr temp = q.front();
                q.pop();
                Node *tempnode = new Node(temp.first, temp.second);
                curr->child.push_back(tempnode);
                treenodequeue.push(tempnode);
                if (q.empty())
                    break;
            }
        }
        return root;
    }

    int getTreeSize(Node *root)
    {
        if (!root)
            return 0;
        int totalcount = 1;
        queue<Node *> nodequeue;
        nodequeue.push(root);
        while (!nodequeue.empty())
        {
            Node *curr = nodequeue.front();
            nodequeue.pop();
            if (curr->child.size() != 0)
            {
                totalcount += curr->child.size();
                for (int i = curr->child.size() - 1; i >= 0; --i)
                    nodequeue.push(curr->child[i]);
            }
        }
        return totalcount;
    }
    void printResult(Node *root)
    {
        if (!root)
            return;
        queue<Node *> treenodequeue;
        int size = getTreeSize(root);
        treenodequeue.push(root);
        root->countCskip(root->dist), root->rootAddr(0);
        cout << "I am coordinator at depth " << root->dist << ", my Cskipvalue is " << root->Cskip << " and my Addr is " << root->Addr << "\n";
        --size;
        while (size != 0)
        {
            int Rindex = 1, Eindex = 1;
            Node *curr = treenodequeue.front();
            treenodequeue.pop();
            for (int i = 0; i < curr->child.size(); ++i)
            {
                if (curr->child[i]->type[0] != 'e') //router
                {

                    curr->child[i]->countCskip(curr->child[i]->dist);
                    curr->child[i]->countAddr(curr->child[i]->type, curr->Addr, Rindex, curr->Cskip);
                    cout << "I am " << curr->child[i]->type << " at depth " << curr->child[i]->dist << ", " << Rindex << "th child of "
                         << curr->type
                         << " , my Cskipvalue is "
                         << curr->child[i]->Cskip
                         << " ,and my Address is "
                         << curr->child[i]->Addr
                         << "\n";
                    ++Rindex;
                }
                else
                { //endDevices //doesn't have to count Cskip value

                    curr->child[i]->countAddr(curr->child[i]->type, curr->Addr, Eindex, curr->Cskip);
                    cout << "I am " << curr->child[i]->type << " at depth " << curr->child[i]->dist << ", " << Eindex << "th child of "
                         << curr->type << " my Address is " << curr->child[i]->Addr << "\n";

                    ++Eindex;
                }
                --size;
                treenodequeue.push(curr->child[i]);
            }
        }
    }
};

int main()
{
    string type[] = {"coordinator",
                     null,
                     "router1",
                     "router2",
                     "router3",
                     "enddevice1",
                     "enddevice2",
                     null,
                     "enddevice3",
                     "enddevice4",
                     null,
                     "router4",
                     "router5",
                     null,
                     "router6",
                     "enddevice5",
                     null,
                     null,
                     null,
                     null,
                     null,
                     "enddevice6",
                     "enddevice7",
                     null,
                     "enddevice8"};
    int dist[] = {0, 0, 1, 1, 1, 1, 1, 0, 2, 2, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 3, 3, 0, 3};
    int len = sizeof(dist) / sizeof(dist[0]);
    vector<pr> nodes;
    for (int i = 0; i < len; ++i)
        nodes.push_back(make_pair(type[i], dist[i]));
    Help h;
    Node *root = h.BuildTree(nodes);
    h.printResult(root);
    return 0;
}