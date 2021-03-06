////
//  main.cpp
//  BinaryTree
//
//  Created by Stanislav Martynov on 01.01.2021.
//

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cmath>

class Vertex
{
public:
    int64_t k;
    std::string v;
    Vertex* l, * r, * p;

    Vertex(int64_t k, std::string v) {
        this->k = k;
        this->v = v;
        l = r = p = nullptr;
    }
};

class BinaryTree
{
private:
    Vertex* root = nullptr;

    void postorder_del(Vertex* ver)
    {
        if (ver) {
            postorder_del(ver->l);
            postorder_del(ver->r);
            delete ver;
        }
    }

    Vertex* find_vertex(int64_t k)
    {
        Vertex* found_vertex = root;
        while (found_vertex) {
            if (k > found_vertex->k) {
                found_vertex = found_vertex->r;
            }
            else if (k < found_vertex->k) {
                found_vertex = found_vertex->l;
            }
            else if (k == found_vertex->k) {
                return found_vertex;
            }
        }
        return found_vertex;
    }
public:
    BinaryTree() = default;
    ~BinaryTree()
    {
        if (root) {
            postorder_del(root->l);
            postorder_del(root->r);
            delete root;
            root = nullptr;
        }
    }

    void add(int64_t k, std::string v)
    {
        Vertex* new_vertex = new Vertex(k, v);
        Vertex* parent = root;
        while (parent) {
            if (new_vertex->k > parent->k) {
                if (parent->r) {
                    parent = parent->r;
                }
                else {
                    new_vertex->p = parent;
                    parent->r = new_vertex;
                    return;
                }
            }
            else if (new_vertex->k < parent->k) {
                if (parent->l) {
                    parent = parent->l;
                }
                else {
                    new_vertex->p = parent;
                    parent->l = new_vertex;
                    return;
                }
            }
            else if (new_vertex->k == parent->k) {
                delete new_vertex;
                throw std::logic_error("error");
            }
        }
        root = new_vertex;
    }

    void set(int64_t k, std::string v)
    {
        Vertex* found = find_vertex(k);
        if (!found) {
            throw std::logic_error("error");
        }
        found->v = v;
    }

    void del(int64_t k)
    {
        Vertex* deleting = find_vertex(k);
        if (!deleting) {
            throw std::logic_error("error");
        }

        // no child del
        if (!deleting->l && !deleting->r) {
            if (root == deleting) {
                root = nullptr;
                delete deleting;
                return;
            }
            if (deleting->p->l == deleting) {
                deleting->p->l = nullptr;
            }
            else if (deleting->p->r == deleting) {
                deleting->p->r = nullptr;
            }
            delete deleting;
        }
        // 1 child del
        else if (deleting->l && !deleting->r) {
            if (root == deleting) {
                root = deleting->l;
                root->p = nullptr;
                delete deleting;
                return;
            }
            if (deleting->p->l == deleting) {
                deleting->p->l = deleting->l;
            }
            else if (deleting->p->r == deleting) {
                deleting->p->r = deleting->l;
            }
            deleting->l->p = deleting->p;
            delete deleting;
        }
        else if (!deleting->l && deleting->r) {
            if (root == deleting) {
                root = deleting->r;
                root->p = nullptr;
                delete deleting;
                return;
            }
            if (deleting->p->l == deleting) {
                deleting->p->l = deleting->r;
            }
            else if (deleting->p->r == deleting) {
                deleting->p->r = deleting->r;
            }
            deleting->r->p = deleting->p;
            delete deleting;
        }
        // 2 child del
        else {
            Vertex* l_max = deleting->l; // max element of the left subtree
            while (l_max->r) {
                l_max = l_max->r;
            }
            deleting->k = l_max->k;
            deleting->v = l_max->v;

            if (l_max == l_max->p->l) {
                l_max->p->l = l_max->l;
            }
            else if (l_max == l_max->p->r) {
                l_max->p->r = l_max->l;
            }

            if (l_max->l) l_max->l->p = l_max->p;
            delete l_max;
        }
    }

    Vertex* search(int64_t k)
    {
        return find_vertex(k);
    }

    Vertex* min()
    {
        if (!root) {
            throw std::logic_error("error");
        }
        Vertex* min = root;
        while (min->l) {
            min = min->l;
        }
        return min;
    }

    Vertex* max()
    {
        if (!root) {
            throw std::logic_error("error");
        }
        Vertex* max = root;
        while (max->r) {
            max = max->r;
        }
        return max;
    }

    Vertex* get_root()
    {
        return root;
    }
};

std::ostream& operator<<(std::ostream& output, const Vertex* vertex)
{
    output << vertex->k << ' ' << vertex->v;
    return output;
}

std::string make_out_string(std::pair<Vertex*, unsigned int> p) {
    std::string str;
    if (p.first) {
        str = '[' + std::to_string(p.first->k) + ' ' + p.first->v + ' ' + std::to_string(p.first->p->k) + ']';
    }
    else {
        str = "_ ";
        for (unsigned int i = 0; i < p.second; i++) {
            str += str;
        }
        str.erase(str.end() - 1);
    }
    return str;
}

std::ostream& operator<<(std::ostream& output, BinaryTree& tree)
{
    Vertex* root = tree.get_root();
    if (!root) {
        output << "_";
        return output;
    }
    output << "[" << root->k << " " << root->v << "]";

    std::queue<std::pair<Vertex*, unsigned int>> q;
    int d = 1;
    int i = 0;
    int max_i = 2;
    bool was_vertex = false;

    if (root->l) {
        q.push({ root->l, 0 });
        was_vertex = true;
    }
    else q.push({ nullptr, 0 });

    if (root->r) {
        q.push({ root->r, 0 });
        was_vertex = true;
    }
    else q.push({ nullptr, 0 });

    if (!was_vertex) return output;
    else {
        output << std::endl;
    }

    while (true) {
        if (i != 0) output << " ";
        else was_vertex = false;

        auto pp = q.front();
        output << make_out_string(pp);
        q.pop();

        if (pp.first) {
            i++;
            if (pp.first->l || pp.first->r) {
                was_vertex = true;
            }
        }
        else i += (int)std::pow(2, pp.second);

        if (i == max_i) {
            if (!was_vertex) break;
            output << std::endl;
            d++;
            max_i = (int)std::pow(2, d);
            i = 0;
        }

        if (!pp.first) {
            q.push({ nullptr, pp.second + 1 });
            continue;
        }

        if (!pp.first->l && !pp.first->r) {
            q.push({ nullptr, 1 });
            continue;
        }

        if (pp.first->l) {
            q.push({ pp.first->l, 0 });
        }
        else q.push({ nullptr, 0 });

        if (pp.first->r) {
            q.push({ pp.first->r, 0 });
        }
        else q.push({ nullptr, 0 });
    }
    return output;
}

int main(int argc, char* argv[])
{
    BinaryTree tree;
    std::string line;
    while (std::cin >> line) {
        try
        {
            if (line.empty()) {
                continue;
            }
            else if (line == "add") {
                int64_t k;
                std::string v;
                std::cin >> k >> v;
                tree.add(k, v);
            }
            else if (line == "set") {
                int64_t k;
                std::string v;
                std::cin >> k >> v;
                tree.set(k, v);
            }
            else if (line == "delete") {
                int64_t k;
                std::cin >> k;
                tree.del(k);
            }
            else if (line == "search") {
                int64_t k;
                std::cin >> k;
                Vertex* vertex = tree.search(k);
                if (!vertex) std::cout << "0" << std::endl;
                else std::cout << "1 " << vertex->v << std::endl;
            }
            else if (line == "min") {
                Vertex* vertex = tree.min();
                std::cout << vertex << std::endl;
            }
            else if (line == "max") {
                Vertex* vertex = tree.max();
                std::cout << vertex << std::endl;
            }
            else if (line == "print") {
                std::cout << tree << std::endl;
            }
            else {
                std::cout << "error" << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}
