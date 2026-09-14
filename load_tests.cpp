// load_tests.cpp
#include "load_tests.h"
#include "unq_ptr.h"
#include "shrd_ptr.h"

#include <iostream>
#include <chrono>
#include <memory>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdio>

// ===================== Утилиты для замеров =====================

// Чтение текущей резидентной памяти процесса (RSS) в килобайтах.
// Работает на Linux и WSL. На других ОС вернёт 0.
size_t get_rss_kb() {
    size_t rss = 0;
    FILE* fp = fopen("/proc/self/status", "r");
    if (!fp) return 0;
    
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line + 6, "%zu", &rss);
            break;
        }
    }
    fclose(fp);
    return rss;
}

// Структура для хранения результатов одного замера
struct BenchResult {
    double time_ms;
    size_t memory_kb;
};

// ===================== Функции замеров =====================

// --- 1. Сырой указатель ---
BenchResult bench_raw(size_t N) {
    BenchResult res;
    
    // Замер памяти: выделяем все объекты и держим их живыми
    size_t mem_before = get_rss_kb();
    std::vector<int*> ptrs(N);
    for (size_t i = 0; i < N; ++i) {
        ptrs[i] = new int(static_cast<int>(i));
    }
    size_t mem_after = get_rss_kb();
    res.memory_kb = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
    for (size_t i = 0; i < N; ++i) delete ptrs[i];
    
    // Замер времени: создаём и сразу уничтожаем
    volatile int sink = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        int* p = new int(static_cast<int>(i));
        sink += *p;
        delete p;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    res.time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    
    return res;
}

// --- 2. Ваш UniquePtr ---
BenchResult bench_my_unique(size_t N) {
    BenchResult res;
    
    size_t mem_before = get_rss_kb();
    std::vector<UniquePtr<int>> ptrs;
    ptrs.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        ptrs.emplace_back(new int(static_cast<int>(i)));
    }
    size_t mem_after = get_rss_kb();
    res.memory_kb = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
    ptrs.clear();
    
    volatile int sink = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        UniquePtr<int> p(new int(static_cast<int>(i)));
        sink += *p;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    res.time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    
    return res;
}

// --- 3. STL unique_ptr ---
BenchResult bench_stl_unique(size_t N) {
    BenchResult res;
    
    size_t mem_before = get_rss_kb();
    std::vector<std::unique_ptr<int>> ptrs;
    ptrs.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        ptrs.emplace_back(new int(static_cast<int>(i)));
    }
    size_t mem_after = get_rss_kb();
    res.memory_kb = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
    ptrs.clear();
    
    volatile int sink = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        std::unique_ptr<int> p(new int(static_cast<int>(i)));
        sink += *p;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    res.time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    
    return res;
}

// --- 4. Ваш SharedPtr ---
BenchResult bench_my_shared(size_t N) {
    BenchResult res;
    
    size_t mem_before = get_rss_kb();
    std::vector<SharedPtr<int>> ptrs;
    ptrs.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        ptrs.emplace_back(new int(static_cast<int>(i)));
    }
    size_t mem_after = get_rss_kb();
    res.memory_kb = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
    ptrs.clear();
    
    volatile int sink = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        SharedPtr<int> p(new int(static_cast<int>(i)));
        sink += *p;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    res.time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    
    return res;
}

// --- 5. STL shared_ptr ---
BenchResult bench_stl_shared(size_t N) {
    BenchResult res;
    
    size_t mem_before = get_rss_kb();
    std::vector<std::shared_ptr<int>> ptrs;
    ptrs.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        ptrs.emplace_back(new int(static_cast<int>(i)));
    }
    size_t mem_after = get_rss_kb();
    res.memory_kb = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
    ptrs.clear();
    
    volatile int sink = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        std::shared_ptr<int> p(new int(static_cast<int>(i)));
        sink += *p;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    res.time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    
    return res;
}

// ===================== Основная функция =====================

void run_load_tests() {
    std::cout << "\n========== STARTING LOAD TESTS ==========\n\n";
    
    // Размеры тестов: малый и большой
    size_t test_sizes[] = {10000, 100000, 1000000};
    
    // Открываем CSV для графиков
    std::ofstream csv("benchmark_results.csv");
    csv << "N"
        << ";Raw_Time_ms;Raw_Mem_KB"
        << ";MyUnq_Time_ms;MyUnq_Mem_KB"
        << ";STLUnq_Time_ms;STLUnq_Mem_KB"
        << ";MyShr_Time_ms;MyShr_Mem_KB"
        << ";STLShr_Time_ms;STLShr_Mem_KB"
        << "\n";

    for (size_t N : test_sizes) {
        std::cout << "--- N = " << N << " ---\n";
        
        BenchResult raw      = bench_raw(N);
        BenchResult my_unq   = bench_my_unique(N);
        BenchResult stl_unq  = bench_stl_unique(N);
        BenchResult my_shr   = bench_my_shared(N);
        BenchResult stl_shr  = bench_stl_shared(N);

        // Красивый табличный вывод в консоль
        printf("  %-18s %12s %12s\n", "Type", "Time (ms)", "Memory (KB)");
        printf("  %-18s %12.2f %12zu\n", "Raw Pointer",      raw.time_ms,     raw.memory_kb);
        printf("  %-18s %12.2f %12zu\n", "My UniquePtr",     my_unq.time_ms,  my_unq.memory_kb);
        printf("  %-18s %12.2f %12zu\n", "STL unique_ptr",   stl_unq.time_ms, stl_unq.memory_kb);
        printf("  %-18s %12.2f %12zu\n", "My SharedPtr",     my_shr.time_ms,  my_shr.memory_kb);
        printf("  %-18s %12.2f %12zu\n", "STL shared_ptr",   stl_shr.time_ms, stl_shr.memory_kb);
        std::cout << "\n";

        // Запись в CSV (разделитель — точка с запятой для корректного открытия в Excel)
        csv << N
            << ";" << raw.time_ms     << ";" << raw.memory_kb
            << ";" << my_unq.time_ms  << ";" << my_unq.memory_kb
            << ";" << stl_unq.time_ms << ";" << stl_unq.memory_kb
            << ";" << my_shr.time_ms  << ";" << my_shr.memory_kb
            << ";" << stl_shr.time_ms << ";" << stl_shr.memory_kb
            << "\n";
    }
    
    csv.close();
    std::cout << "Results saved to 'benchmark_results.csv'\n";
    std::cout << "Use this file to build comparison charts.\n";
    std::cout << "\n========== LOAD TESTS COMPLETED ==========\n\n";
}