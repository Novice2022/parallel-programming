#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <algorithm>
#include <set>

using namespace std;

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

void find_lcm_numbers(const vector<int>& sequence, int start, int end, promise<vector<int>>& result_promise) {
    vector<int> lcm_numbers;
    
    for (int i = start; i < end; i++) {
        for (int j = 0; j < sequence.size(); j++) {
            if (i == j) continue;
            
            int lcm_val = lcm(sequence[i], sequence[j]);
            lcm_numbers.push_back(lcm_val);
        }
    }
    
    result_promise.set_value(lcm_numbers);
}

void lab() {
    int n, num_threads;
    
    cout << "Введите количество чисел в последовательности: ";
    cin >> n;
    
    vector<int> sequence(n);
    cout << "Введите последовательность чисел: ";
    for (int i = 0; i < n; i++) {
        cin >> sequence[i];
    }
    
    cout << "Введите количество потоков: ";
    cin >> num_threads;
    
    vector<future<vector<int>>> futures;
    vector<promise<vector<int>>> promises(num_threads);
    
    int elements_per_thread = n / num_threads;
    int remaining_elements = n % num_threads;
    
    int start_index = 0;
    for (int i = 0; i < num_threads; i++) {
        int end_index = start_index + elements_per_thread + (i < remaining_elements ? 1 : 0);
        
        futures.push_back(promises[i].get_future());
        thread t(find_lcm_numbers, ref(sequence), start_index, end_index, ref(promises[i]));
        t.detach();
        
        start_index = end_index;
    }
    
    set<int> all_lcm_numbers;
    for (auto& future : futures) {
        vector<int> thread_result = future.get();
        for (int num : thread_result) {
            all_lcm_numbers.insert(num);
        }
    }
    
    cout << "Числа, являющиеся НОК любых двух чисел в последовательности: ";
    if (all_lcm_numbers.empty()) {
        cout << "нет таких чисел";
    } else {
        for (int num : all_lcm_numbers) {
            cout << num << " ";
        }
    }
    cout << endl;
}
