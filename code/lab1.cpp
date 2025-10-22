#include <iostream>
#include <vector>
#include <pthread.h>
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

struct ThreadData {
    int thread_id;
    int num_threads;
};

void* find_lcm_pairs(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int thread_id = data->thread_id;
    int num_threads = data->num_threads;

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
    pthread_exit(NULL);
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

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = num_threads;
        pthread_create(&threads[i], NULL, find_lcm_pairs, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
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
