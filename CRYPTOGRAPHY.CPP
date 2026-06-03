#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ================= UTILITY =================

ll mod_exp(ll base, ll exp, ll mod) {
    ll res = 1;
    base %= mod;

    while (exp) {
        if (exp & 1)
            res = (res * base) % mod;

        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

ll gcd(ll a, ll b) {
    return b ? gcd(b, a % b) : a;
}

ll mod_inverse(ll a, ll m) {
    ll m0 = m, y = 0, x = 1;

    while (a > 1) {
        ll q = a / m;

        ll t = m;
        m = a % m;
        a = t;

        t = y;
        y = x - q * y;
        x = t;
    }

    return (x + m0) % m0;
}

// ================= HASH =================

string simple_hash(const string &input) {
    long long hash = 0;

    for (char c : input)
        hash = (hash * 131 + c) % 1000000007;

    return to_string(hash);
}

// ================= PRIME CHECK =================

bool isPrime(ll n) {
    if (n < 2) return false;

    for (ll i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;

    return true;
}

// ================= RSA =================

struct RSA {

    ll p, q, n, phi, e, d;

    ll generate_prime() {

        while (true) {
            ll num = rand() % 10000 + 1000;

            if (isPrime(num))
                return num;
        }
    }

    void generate_keys() {

        p = generate_prime();

        do {
            q = generate_prime();
        }
        while (q == p);

        n = p * q;
        phi = (p - 1) * (q - 1);

        e = 3;

        while (gcd(e, phi) != 1)
            e += 2;

        d = mod_inverse(e, phi);
    }

    ll encrypt(ll msg) {
        return mod_exp(msg, e, n);
    }

    ll decrypt(ll cipher) {
        return mod_exp(cipher, d, n);
    }
};

// ================= DIFFIE HELLMAN =================

struct DiffieHellman {

    ll p = 1000000007;
    ll g = 5;

    ll generate_public(ll private_key) {
        return mod_exp(g, private_key, p);
    }

    ll compute_secret(ll received, ll private_key) {
        return mod_exp(received, private_key, p);
    }
};

// ================= GRAPH =================

struct Graph {

    int V;
    vector<vector<pair<int,int>>> adj;

    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v, int w) {

        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }

    vector<int> dijkstra(int src) {

        vector<int> dist(V, INT_MAX);

        priority_queue<
            pair<int,int>,
            vector<pair<int,int>>,
            greater<pair<int,int>>
        > pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {

            auto temp = pq.top();
            pq.pop();

            int d = temp.first;
            int u = temp.second;

            if (d > dist[u]) continue;

            for (auto &edge : adj[u]) {

                int v = edge.first;
                int w = edge.second;

                if (dist[v] > d + w) {

                    dist[v] = d + w;
                    pq.push({dist[v], v});
                }
            }
        }

        return dist;
    }
};

// ================= MERKLE TREE =================

string buildMerkleRoot(vector<string> transactions) {

    vector<string> hashes;

    for (auto t : transactions)
        hashes.push_back(simple_hash(t));

    while (hashes.size() > 1) {

        vector<string> newLevel;

        for (int i = 0; i < hashes.size(); i += 2) {

            if (i + 1 < hashes.size())
                newLevel.push_back(
                    simple_hash(hashes[i] + hashes[i+1])
                );
            else
                newLevel.push_back(
                    simple_hash(hashes[i] + hashes[i])
                );
        }

        hashes = newLevel;
    }

    return hashes[0];
}

// ================= HILL CIPHER =================
// Yahan apni complete HillCipher class paste kar dena
// Jo tune mujhe bheji thi

// ================= MAIN =================

int main() {

    srand(time(0));

    while (true) {

        cout << "\n===== MENU =====\n";
        cout << "1. RSA\n";
        cout << "2. Diffie-Hellman\n";
        cout << "3. Hash\n";
        cout << "4. Graph\n";
        cout << "5. Merkle Tree\n";
        cout << "6. Hill Cipher\n";
        cout << "7. Exit\n";

        int ch;
        cin >> ch;

        // RSA
        if (ch == 1) {

            RSA rsa;
            rsa.generate_keys();

            ll msg;

            cout << "Enter message: ";
            cin >> msg;

            ll enc = rsa.encrypt(msg);

            cout << "Encrypted: " << enc << endl;
            cout << "Decrypted: "
                 << rsa.decrypt(enc) << endl;
        }

        // Diffie Hellman
        else if (ch == 2) {

            DiffieHellman dh;

            ll a, b;

            cout << "Enter private key A: ";
            cin >> a;

            cout << "Enter private key B: ";
            cin >> b;

            ll A = dh.generate_public(a);
            ll B = dh.generate_public(b);

            cout << "Public Key A: " << A << endl;
            cout << "Public Key B: " << B << endl;

            cout << "Shared Secret A: "
                 << dh.compute_secret(B, a)
                 << endl;

            cout << "Shared Secret B: "
                 << dh.compute_secret(A, b)
                 << endl;
        }

        // Hash
        else if (ch == 3) {

            string s;

            cout << "Enter string: ";
            cin >> s;

            cout << "Hash: "
                 << simple_hash(s)
                 << endl;
        }

        // Graph
        else if (ch == 4) {

            Graph g(5);

            g.addEdge(0,1,2);
            g.addEdge(1,2,3);
            g.addEdge(2,4,1);
            g.addEdge(0,3,6);
            g.addEdge(3,4,2);

            vector<int> dist = g.dijkstra(0);

            for (int d : dist)
                cout << d << " ";

            cout << endl;
        }

        // Merkle Tree
        else if (ch == 5) {

            int n;

            cout << "Enter number of transactions: ";
            cin >> n;

            vector<string> tx(n);

            cout << "Enter transactions:\n";

            for (int i = 0; i < n; i++)
                cin >> tx[i];

            cout << "Merkle Root: "
                 << buildMerkleRoot(tx)
                 << endl;
        }

        // Hill Cipher
        else if (ch == 6) {

            cout << "Paste your Hill Cipher code here.\n";
        }

        else {
            break;
        }
    }

    return 0;
}