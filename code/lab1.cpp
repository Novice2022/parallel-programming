#include <iostream>
#include <vector>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

using namespace std;


vector<int> arr;
vector<int> results;
pthread_mutex_t mutex;

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

    for (int i = start; i < end; i++) {
        for (int j = i + 1; j < end; j++) {
            int current_lcm = lcm(arr[i], arr[j]);

            pthread_mutex_lock(&mutex);
            results.push_back(current_lcm);
            pthread_mutex_unlock(&mutex);
        }
    }
    
    pthread_exit(NULL);
}

void lab() {
    srand(time(0));
    int arr_size, num_threads;

    cout << "Введите размер массива: ";
    cin >> arr_size;
    cout << "Введите количество потоков: ";
    cin >> num_threads;

    arr.resize(arr_size);
    for (int i = 0; i < arr_size; i++) {
        arr[i] = rand() % 100 + 1;
    }

    cout << "Сгенерированный массив: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    pthread_mutex_init(&mutex, NULL);

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

    cout << "Найденные НОК: ";
    for (int num : results) {
        cout << num << " ";
    }
    cout << endl;

    pthread_mutex_destroy(&mutex);
}
