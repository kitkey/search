#include <chrono>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <filesystem>
#include <map>
#include <unordered_set>

template<class T>
std::vector<T> linear_search(T* arr, int size, const std::string& search_value) {
    std::vector<T> result;
    for (int i = 0; i < size; ++i) {
        if (arr[i].full_name == search_value)
            result.push_back(arr[i]);
    }
    return result;
}

/// @brief Структура гостя
struct Guest {
    int room_number;          /// Номер комнаты
    int cost;                 /// Стоимость проживания
    std::string full_name;    /// Полное имя гостя
    std::string arrival_time; /// Дата и время прибытия
    std::string departure_time; /// Дата и время отбытия


    /// @brief Перегрузка оператора сравнения: меньше
    bool operator<(const Guest& other) const {
        if (arrival_time != other.arrival_time)
            return arrival_time < other.arrival_time;
        if (room_number != other.room_number)
            return room_number < other.room_number;
        return full_name < other.full_name;
    }

    /// @brief Перегрузка оператора сравнения: меньше или равно
    bool operator<=(const Guest& other) const {
        if (arrival_time != other.arrival_time)
            return arrival_time < other.arrival_time;
        if (room_number != other.room_number)
            return room_number < other.room_number;
        return full_name <= other.full_name;
    }

    /// @brief Перегрузка оператора сравнения: больше
    bool operator>(const Guest& other) const {
        if (arrival_time != other.arrival_time)
            return arrival_time > other.arrival_time;
        if (room_number != other.room_number)
            return room_number > other.room_number;
        return full_name > other.full_name;
    }

    /// @brief Перегрузка оператора сравнения: больше или равно
    bool operator>=(const Guest& other) const {
        if (arrival_time != other.arrival_time)
            return arrival_time > other.arrival_time;
        if (room_number != other.room_number)
            return room_number > other.room_number;
        return full_name >= other.full_name;
    }

};

/// @brief Чтение данных гостей из CSV файла
/// @param filename Путь к файлу CSV
/// @return Вектор гостей
std::vector<Guest> read_data(const std::string& filename) {
    std::vector<Guest> guests;
    std::ifstream file(filename);

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        Guest g;

        std::getline(ss, g.full_name, ',');
        std::getline(ss, g.arrival_time, ',');

        std::getline(ss, item, ',');
        g.room_number = std::stoi(item);

        std::getline(ss, g.departure_time, ',');

        std::getline(ss, item, ',');
        g.cost = std::stoi(item);

        guests.push_back(g);
    }

    return guests;
}


/// @brief бинарное дерево поиска
class BinarySearchTree {
public:
    /// @brief Узел BST.
    class Node {
    public:
        Guest data;
        Node* left;
        Node* right;

        /// @brief Создаёт листовой узел.
        /// @param value объект Guest
        explicit Node(const Guest& value) {
            data = value;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    /// @brief Построить дерево последовательной вставкой массива.
    /// @param array исходный массив госчтей
    /// @param size  количество элементов
    BinarySearchTree(Guest* array, size_t size) {
        root = nullptr;
        for (size_t i = 0; i < size; ++i) {
            insert(array[i]);
        }
    }

    /// @brief Вставка нового элемента
    void insert(const Guest& value) {
        Node* newNode = new Node(value);
        if (root == nullptr) {
            root = newNode;
            return;
        }

        Node* current = root;
        while (true) {
            if (value.full_name < current->data.full_name) {
                if (current->left == nullptr) {
                    current->left = newNode;
                    return;
                }
                current = current->left;
            } else {
                if (current->right == nullptr) {
                    current->right = newNode;
                    return;
                }
                current = current->right;
            }
        }
    }

    /// @brief Поиск по ключу
    /// @param key ключ
    std::vector<Guest> search_all(const std::string& key) const {
        std::vector<Guest> results;
        search(root, key, results);
        return results;
    }

    void search(Node* node, const std::string& key, std::vector<Guest>& results) const {
        if (!node) return;

        if (key < node->data.full_name) {
            search(node->left, key, results);
        } else if (key > node->data.full_name) {
            search(node->right, key, results);
        } else {
            results.push_back(node->data);

            // search(node->left, key, results);
            search(node->right, key, results);
        }
    }
};

/// @brief Красно-чёрное дерево поиска по полю Guest::full_name.
class RBTree {
    public:
        /// @brief Узел красно-черного дерева.
        class Node {
        public:
            Guest data;
            bool red = true;
            Node* left = nullptr;
            Node* right = nullptr;
            Node* parent = nullptr;
            explicit Node(Guest  v) : data(v) {}
        };

        /// @brief Построить дерево из массива.
        /// @param array  исходный массив
        /// @param size   количество элементов
        RBTree(Guest* array, size_t size) {
            root = nullptr;
            for (size_t i = 0; i < size; ++i) {
                insert(array[i]);
            }
        }
        Node* root;

        /// @brief Левое вращение вокруг узла x.
        void leftRotate(Node* x) {
            Node* y = x->right;
            x->right = y->left;
            if (y->left)
                y->left->parent = x;
            y->parent = x->parent;
            if (!x->parent)
                root = y;
            else if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
            y->left = x;
            x->parent = y;
        }
        /// @brief Правое вращение вокруг узла x.
        void rightRotate(Node* y) {
                Node* x = y->left;
                y->left = x->right;
                if (x->right)
                    x->right->parent = y;
                x->parent = y->parent;
                if (!y->parent)
                    root = x;
                else if (y == y->parent->left)
                    y->parent->left = x;
                else
                    y->parent->right = x;
                x->right = y;
                y->parent = x;
            }

        /// @brief Восстановление свойств после вставки узла
        void fixInsertion(Node* z) {
            while (z != root && z->parent->red) {
                if (z->parent == z->parent->parent->left) {
                    Node* u = z->parent->parent->right;
                    if (u && u->red) {
                        z->parent->red = false;
                        u->red = false;
                        z->parent->parent->red = true;
                        z = z->parent->parent;
                    } else {
                        if (z == z->parent->right) {
                            z = z->parent;
                            leftRotate(z);
                        }
                        z->parent->red = false;
                        z->parent->parent->red = true;
                        rightRotate(z->parent->parent);
                    }
                } else {
                    Node* u = z->parent->parent->left;
                    if (u && u->red) {
                        z->parent->red = false;
                        u->red = false;
                        z->parent->parent->red = true;
                        z = z->parent->parent;
                    } else {
                        if (z == z->parent->left) {
                            z = z->parent;
                            rightRotate(z);
                        }
                        z->parent->red = false;
                        z->parent->parent->red = true;
                        leftRotate(z->parent->parent);
                    }
                }
            }
            root->red = false;
        }

        /// @brief Вставка узла
        void insert(const Guest& value) {
            Node* newNode = new Node(value);
            if (!root) {
                root = newNode;
                root->red = false;
                return;
            }
            Node* p = root;
            Node* q = nullptr;
            while (p) {
                q = p;
                if (value.full_name < p->data.full_name)
                    p = p->left;
                else
                    p = p->right;
            }
            newNode->parent = q;
            if (value.full_name < q->data.full_name)
                q->left = newNode;
            else
                q->right = newNode;
            fixInsertion(newNode);
        }

        /// @brief Поиск по узлам
        std::vector<Guest> search_all(const std::string& key) const {
                std::vector<Guest> results;
                search(root, key, results);
                return results;
            }

        void search(Node* node, const std::string& key, std::vector<Guest>& results) const {
                if (!node) return;

                if (key < node->data.full_name) {
                    search(node->left,  key, results);
                } else if (key > node->data.full_name) {
                    search(node->right, key, results);
                } else {
                    results.push_back(node->data);
                    search(node->left,  key, results);
                    search(node->right, key, results);
                }
            }
};

/// @brief Класс хеш таблицы
class HashTable {
    public:
        std::vector<std::vector<Guest>> hash_table;


        HashTable(Guest* array, size_t size) {
            for (size_t i = 0; i < size; ++i) {
                insert(array[i]);
            }
        }

        /// @brief хеш-функция
        long long int hash(const std::string& key) const {
            long long MOD = 2^64;
            const int p = 53;
            long long hash = 0, p_pow = 1;
            for (size_t i = 0; i < key.length(); ++i)
            {
                hash  = (hash + ((key[i] - 'a' + 1) * p_pow) % MOD) % MOD;;
                p_pow = (p_pow * p) % MOD;

            }
            return hash;
        }

        /// @brief Функция вставки элемента в хеш таблицу
        void insert(const Guest& g) {
                if (hash_table.empty()) hash_table.resize(1000000);
                auto idx = hash(g.full_name) % hash_table.size();
                hash_table[idx].push_back(g);
            }

        /// @brief функция поиска по ключу в хеш таблице
        std::vector<Guest> search_all(const std::string& key) {
            std::vector<Guest> results;

            std::size_t idx = hash(key) % hash_table.size();
            const std::vector<Guest>& group = hash_table[idx];

            for (std::size_t i = 0; i < group.size(); ++i) {
                if (group[i].full_name == key)
                    results.push_back(group[i]);
            }
            return results;
        }
        /// @brief подсчет коллизий
    std::size_t collisions() const {
            std::size_t total = 0;
            for (const auto& bucket : hash_table) {
                if (bucket.size() <= 1) continue;
                std::unordered_set<std::string> uniq;
                for (const auto& g : bucket) uniq.insert(g.full_name);

                if (uniq.size() > 1)
                    total += uniq.size() - 1;
            }
            return total;
        }
};

int main() {
    using clock = std::chrono::high_resolution_clock;

    std::vector<int> sizes = {100, 1000, 10000, 20000, 70000, 120000, 200000, 350000, 600000, 1000000};


    const std::string out_dir  = "D:/searchCtest/output";
    const std::string csv_path = out_dir + "/search_time.csv";
    std::filesystem::create_directories(out_dir);
    std::ofstream csv(csv_path, std::ios::trunc);
    csv << "algorithm,size,time,collision\n";

    for (int n : sizes) {
        std::string file = "D:/searchCtest/data_lab2/data_hostel_" +
                           std::to_string(n) + ".csv";
        std::vector<Guest> guests = read_data(file);

        std::string key = guests[guests.size() / 3].full_name;

        auto t0 = clock::now();
        auto res_lin = linear_search(guests.data(), guests.size(), key);
        auto t1 = clock::now();
        csv << "linear," << n << ',' << std::chrono::duration<double>(t1 - t0).count() << ",0" << std::endl;


        BinarySearchTree bst(guests.data(), guests.size());
        t0 = clock::now();
        auto res_bst = bst.search_all(key);
        t1 = clock::now();
        csv << "bst," << n << ',' << std::chrono::duration<double>(t1 - t0).count() << ",0" << std::endl;


        RBTree rbt(guests.data(), guests.size());
        t0 = clock::now();
        auto res_rbt = rbt.search_all(key);
        t1 = clock::now();
        csv << "rbt," << n << ',' << std::chrono::duration<double>(t1 - t0).count() << ",0" << std::endl;


        HashTable ht(guests.data(), guests.size());
        t0 = clock::now();
        auto res_hash = ht.search_all(key);
        t1 = clock::now();
        csv << "hash," << n << ',' << std::chrono::duration<double>(t1 - t0).count() << ',' << ht.collisions() << std::endl;

        std::multimap<std::string, Guest> mm;
        for (const auto& g : guests) mm.emplace(g.full_name, g);

        t0 = clock::now();
        auto range = mm.equal_range(key);
        std::size_t mm_cnt = std::distance(range.first, range.second);
        t1 = clock::now();
        csv << "multimap," << n << ',' << std::chrono::duration<double>(t1 - t0).count() << ",0" << std::endl;
    }
    return 0;
}
