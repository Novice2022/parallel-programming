#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

vector<int> arr;
vector<vector<int>> local_lcms;

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

void find_lcm_pairs(int thread_id, int num_threads) {
    int chunk_size = arr.size() / num_threads;
    int start = thread_id * chunk_size;
    int end = (thread_id == num_threads - 1) ? arr.size() : start + chunk_size;

    vector<int> lcms;
    for (int i = start; i < end; i++) {
        for (int j = i + 1; j < end; j++) {
            lcms.push_back(lcm(arr[i], arr[j]));
        }
    }

    local_lcms[thread_id] = lcms;
}

void lab() {
    srand(time(0));

    int array_size, num_threads;

    cout << "Введите размер массива: ";
    cin >> array_size;

    cout << "Введите количество потоков: ";
    cin >> num_threads;

    arr.resize(array_size);
    for (int i = 0; i < array_size; i++) {
        arr[i] = rand() % 50 + 1;
    }

    cout << "Сгенерированный массив: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    local_lcms.resize(num_threads);

    vector<thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(find_lcm_pairs, i, num_threads);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    vector<int> all_lcms;
    for (int i = 0; i < num_threads; i++) {
        all_lcms.insert(all_lcms.end(), local_lcms[i].begin(), local_lcms[i].end());
    }

    cout << "Найденные НОК: ";
    for (int num : all_lcms) {
        cout << num << " ";
    }
    cout << endl;
}
