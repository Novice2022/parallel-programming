#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

vector<int> A;
vector<int> results;
mutex results_mutex;

int gcd(int a, int b) {
    while (b) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

void worker(int start, int end) {
    for (int i = start; i < end; ++i) {
        for (int j = i + 1; j < A.size(); ++j) {
            int lcm_val = lcm(A[i], A[j]);
            lock_guard<mutex> lock(results_mutex);
            results.push_back(lcm_val);
        }
    }
}

void lab() {
    int n, thread_count;
    
    cout << "Введите количество чисел в последовательности: ";
    cin >> n;
    
    A.resize(n);
    cout << "Введите " << n << " натуральных чисел:\n";
    for (int i = 0; i < n; ++i) {
        cin >> A[i];
    }
    
    cout << "Введите количество потоков: ";
    cin >> thread_count;
    
    if (thread_count <= 0 || thread_count > n) {
        cout << "Некорректное количество потоков!\n";
        return;
    }
    
    vector<thread> threads;
    
    int chunk_size = n / thread_count;
    int remainder = n % thread_count;
    int start = 0;
    
    for (int i = 0; i < thread_count; ++i) {
        int end = start + chunk_size + (i < remainder ? 1 : 0);
        threads.emplace_back(worker, start, end);
        start = end;
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    cout << "\nНайденные НОК пар чисел: ";
    for (int val : results) {
        cout << val << " ";
    }
    cout << endl;
    
    cout << "Всего найдено " << results.size() << " значений НОК\n";
}
