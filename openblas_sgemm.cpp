#include <cblas.h>
#include <stdlib.h>
#include <vector>
#include <numeric>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <filesystem>
#include <string>
#include <format>
#include <random>
#include <cmath>

const int WARMUP = 10;
const int RUNS = 10;
const int MAXTHREADNUM = 20;
const float ERR = 0.1;

void compareMat(float *A, float *B, int N, int M);
void initInputs(float *A, float *B, int N, int K, int M);
double blasMM(float *A, float *B, float *C, int M, int N, int K);
void printResults(const std::string& name, const std::vector<double>& results, const double FLOPs);

int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cerr << "Invalid Usage!\n";
        std::cout << "Usage: ./openblas_sgemm N K M" << std::endl;
        exit(1);
    }

    int N = std::stoi(argv[1]);
    int K = std::stoi(argv[2]);
    int M = std::stoi(argv[3]);

    std::cout << "Input is " << "N=" << N << ", K=" << K << ", M=" << M << "\n" <<std::endl;
    float *A = (float*)malloc(sizeof(float) * N * K);
    float *B = (float*)malloc(sizeof(float) * K * M);
    float *C = (float*)malloc(sizeof(float) * N * M);
    float *D = (float*)malloc(sizeof(float) * N * M);
    initInputs(A, B, N, K, M);
    for (int i = 0; i < N * M; i++) {
        C[i] = 0.0f;
        D[i] = 0.0f;
    }
    // check correctness first
    
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            for (int k = 0; k < K; ++k)
                D[i*M+j] += A[i*K+k] * B[k*M+j];
    blasMM(A, B, C, M, N, K);
    compareMat(D, C, M, N);
    // check correctness done

    for (int tn = 1; tn <= MAXTHREADNUM; ++tn) {
        openblas_set_num_threads(tn);
        for (int i = 0; i < WARMUP; ++i)    blasMM(A, B, C, M, N, K);
        std::vector<double> results;
        for (int i = 0; i < RUNS; ++i) {
            results.push_back(blasMM(A, B, C, M, N, K));
        }
        std::sort(results.begin(), results.end());
        double FLOPs = 2.0 * M * N * K;
        printResults("OpenBLAS ThreadNum=" + std::to_string(tn), results, FLOPs);
    }
    // check correctness in the end
    compareMat(D, C, M, N);
    // check correctness in the end done

    free(A);
    free(B);
    free(C);
    free(D);
    return 0;
}


void compareMat(float *A, float *B, int N, int M) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (std::abs(A[i*M+j] - B[i*M+j]) > ERR) {
        std::cerr << "Correctness check failed!\n" << "N=" << N << " M=" << M << std::endl;
        std::cerr << "i=" << i << ", j=" << j << " A=" << A[i*M+j] << ", B=" << B[i*M+j] << std::endl;
        exit(1);
      }
    }
  }
}

void initInputs(float *A, float *B, int N, int K, int M) {
    std::mt19937 engine{137};
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (int i = 0; i < N * K; ++i) A[i] = dist(engine);
    for (int i = 0; i < K * M; ++i) B[i] = dist(engine);
    /*
    for (int i = 0; i < N * K; ++i) A[i] = static_cast<float>(i % 10);
    for (int i = 0; i < K * M; ++i) B[i] = static_cast<float>(i % 10);
    */

}

double blasMM(float *A, float *B, float *C, int M, int N, int K) {
    auto start = std::chrono::high_resolution_clock::now();
    ::cblas_sgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans,
        N, M, K,  // M N K
        1,      // Alpha
        A, K,   // A and strides between rows
        B, M,   // B and strides between rows
        0,      // Beta
        C, M    // C and strides between rows
    );
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

void printResults(const std::string& name, const std::vector<double>& results, const double FLOPs)
{
    double total = std::accumulate(results.begin(), results.end(), 0.0);
    double avg = total/results.size();
    double median = results[results.size()/2];
    double min = results[0];
    double dev = 0.0;

    for (const auto re : results)
        dev += (re - avg) * (re - avg);
    dev /= results.size();

    std::cout << "=== " << name << " ===" << std::endl;
    std::cout << "Took " << total << " seconds for " << RUNS << " runs.\n";
    std::cout << avg << " Avg.\t(" << FLOPs/1.0e9/avg << " GFLOPS)\n";
    std::cout << median << " Med.\t(" << FLOPs/1.0e9/median << " GFLOPS)\n";
    std::cout << min << " Min.\t(" << FLOPs/1.0e9/min << " GFLOPS)\n";
    std::cout << dev << " Dev\n" << std::endl;
}



