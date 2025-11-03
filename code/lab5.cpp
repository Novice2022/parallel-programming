#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

vector<int> numbers;
vector<int> results;
vector<bool> task_completed;
vector<thread> workers;
mutex mtx;
condition_variable cv;
int completed_count = 0;
bool all_tasks_done = false;

int find_lcm(int a, int b) {
    int greater = max(a, b);
    int smallest = min(a, b);
    for (int i = greater; ; i += greater) {
        if (i % smallest == 0) {
            return i;
        }
    }
}

void worker_thread(int thread_id, int start_idx, int end_idx) {
    vector<int> local_results;
    
    for (int i = start_idx; i < end_idx; i++) {
        for (int j = i + 1; j < numbers.size(); j++) {
            int lcm_val = find_lcm(numbers[i], numbers[j]);
            local_results.push_back(lcm_val);
        }
    }
    
    {
        lock_guard<mutex> lock(mtx);
        results.insert(results.end(), local_results.begin(), local_results.end());
        task_completed[thread_id] = true;
        completed_count++;
        
        if (completed_count == workers.size()) {
            all_tasks_done = true;
            cv.notify_all();
        }
    }
}

void lab() {
    int n, num_threads;
    
    cout << "Введите количество чисел в последовательности: ";
    cin >> n;
    
    numbers.resize(n);
    cout << "Введите " << n << " натуральных чисел: ";
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }
    
    cout << "Введите количество потоков: ";
    cin >> num_threads;
    
    task_completed.resize(num_threads, false);
    
    int pairs_per_thread = (n * (n - 1)) / (2 * num_threads);
    int current_index = 0;
    
    for (int i = 0; i < num_threads; i++) {
        int start_idx = current_index;
        int end_idx = (i == num_threads - 1) ? n : start_idx + pairs_per_thread;
        if (end_idx > n) end_idx = n;
        
        workers.emplace_back(worker_thread, i, start_idx, end_idx);
        current_index = end_idx;
    }
    
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return all_tasks_done; });
    }
    
    for (auto& worker : workers) {
        worker.join();
    }
    
    cout << "Найдены следующие НОК пар чисел:" << endl;
    for (int lcm_val : results) {
        cout << lcm_val << " ";
    }
    cout << endl;
}
