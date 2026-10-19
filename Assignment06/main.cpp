#include <iostream>
#include <variant>
#include <memory>
using namespace std;

struct Node;
using Tree = variant<monostate, shared_ptr<Node>>;

struct Node {
    int value;
    Tree left, right;

    Node(int v) : value(v), left(monostate{}), right(monostate{}) {}
};

void insert(Tree& tree, int value) {
    visit([&](auto& t) {
        using T = decay_t<decltype(t)>;

        if constexpr (is_same_v<T, monostate>) {
            tree = make_shared<Node>(value);
        } else {
            if (value < t->value)
                insert(t->left, value);
            else if (value > t->value)
                insert(t->right, value);
        }
    }, tree);
}

void inorder(const Tree& tree) {
    visit([](const auto& t) {
        using T = decay_t<decltype(t)>;

        if constexpr (!is_same_v<T, monostate>) {
            inorder(t->left);
            cout << t->value << " ";
            inorder(t->right);
        }
    }, tree);
}

void preorder(const Tree& tree) {
    visit([](const auto& t) {
        using T = decay_t<decltype(t)>;

        if constexpr (!is_same_v<T, monostate>) {
            cout << t->value << " ";
            preorder(t->left);
            preorder(t->right);
        }
    }, tree);
}

void postorder(const Tree& tree) {
    visit([](const auto& t) {
        using T = decay_t<decltype(t)>;

        if constexpr (!is_same_v<T, monostate>) {
            postorder(t->left);
            postorder(t->right);
            cout << t->value << " ";
        }
    }, tree);
}

int main() {
    Tree tree = monostate{};

    for (int n : {50, 30, 70, 20, 40, 60, 80})
        insert(tree, n);

    cout << "Inorder: ";
    inorder(tree);

    cout << "\nPreorder: ";
    preorder(tree);

    cout << "\nPostorder: ";
    postorder(tree), cout << "\n";

    return 0;
}

