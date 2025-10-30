#include <iostream>
#include <vector>
#include <thread>
#include <unordered_set>
#include <chrono>

using namespace std;
using namespace std::chrono;

unordered_set<int> A, B, C;

bool is_union_AB = true;
bool is_intersection_AB = true;
bool is_diff_AB = true;
bool is_diff_BA = true;

void check_union() {
    unordered_set<int> union_set;
    union_set.insert(A.begin(), A.end());
    union_set.insert(B.begin(), B.end());
    is_union_AB = (C == union_set);
}

void check_intersection() {
    unordered_set<int> intersection_set;
    for (int element : A) {
        if (B.find(element) != B.end()) {
            intersection_set.insert(element);
        }
    }
    is_intersection_AB = (C == intersection_set);
}

void check_diff_AB() {
    unordered_set<int> diff_AB_set;
    for (int element : A) {
        if (B.find(element) == B.end()) {
            diff_AB_set.insert(element);
        }
    }
    is_diff_AB = (C == diff_AB_set);
}

void check_diff_BA() {
    unordered_set<int> diff_BA_set;
    for (int element : B) {
        if (A.find(element) == A.end()) {
            diff_BA_set.insert(element);
        }
    }
    is_diff_BA = (C == diff_BA_set);
}

void sequential_check() {
    unordered_set<int> union_set;
    union_set.insert(A.begin(), A.end());
    union_set.insert(B.begin(), B.end());
    is_union_AB = (C == union_set);

    unordered_set<int> intersection_set;
    for (int element : A) {
        if (B.find(element) != B.end()) {
            intersection_set.insert(element);
        }
    }
    is_intersection_AB = (C == intersection_set);

    unordered_set<int> diff_AB_set;
    for (int element : A) {
        if (B.find(element) == B.end()) {
            diff_AB_set.insert(element);
        }
    }
    is_diff_AB = (C == diff_AB_set);

    unordered_set<int> diff_BA_set;
    for (int element : B) {
        if (A.find(element) == A.end()) {
            diff_BA_set.insert(element);
        }
    }
    is_diff_BA = (C == diff_BA_set);
}

void lab() {
    int size_A, size_B, size_C;

    cout << "Введите размер множества A: ";
    cin >> size_A;
    cout << "Введите элементы множества A: ";
    for (int i = 0; i < size_A; i++) {
        int element;
        cin >> element;
        A.insert(element);
    }

    cout << "\nВведите размер множества B: ";
    cin >> size_B;
    cout << "Введите элементы множества B: ";
    for (int i = 0; i < size_B; i++) {
        int element;
        cin >> element;
        B.insert(element);
    }

    cout << "\nВведите размер множества C: ";
    cin >> size_C;
    cout << "Введите элементы множества C: ";
    for (int i = 0; i < size_C; i++) {
        int element;
        cin >> element;
        C.insert(element);
    }

    auto start_parallel = high_resolution_clock::now();
    
    thread thread_union(check_union);
    thread thread_intersection(check_intersection);
    thread thread_diff_AB(check_diff_AB);
    thread thread_diff_BA(check_diff_BA);

    thread_union.join();
    thread_intersection.join();
    thread_diff_AB.join();
    thread_diff_BA.join();
    
    auto end_parallel = high_resolution_clock::now();
    auto parallel_time = duration_cast<microseconds>(end_parallel - start_parallel).count();

    auto start_sequential = high_resolution_clock::now();
    sequential_check();
    auto end_sequential = high_resolution_clock::now();
    auto sequential_time = duration_cast<microseconds>(end_sequential - start_sequential).count();

    double speedup = (double)sequential_time / parallel_time;
    double efficiency = speedup / 4.0;

    cout << "Множество C:\n";
    cout << "- является объединением A и B (A ∪ B): " << (is_union_AB ? "Да" : "Нет") << endl;
    cout << "- является пересечением A и B (A ∩ B): " << (is_intersection_AB ? "Да" : "Нет") << endl;
    cout << "- является разностью A и B (A \\ B): " << (is_diff_AB ? "Да" : "Нет") << endl;
    cout << "- является разностью B и A (B \\ A): " << (is_diff_BA ? "Да" : "Нет") << endl;

    cout << "\nВремя выполнения (параллельно): " << parallel_time << " мкс" << endl;
    cout << "Время выполнения (последовательно): " << sequential_time << " мкс" << endl;
    cout << "Ускорение: " << speedup << endl;
    cout << "Эффективность: " << efficiency << " (" << (efficiency * 100) << "%)" << endl;
}
