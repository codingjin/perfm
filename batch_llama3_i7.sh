#!/bin/bash

make
mkdir -p llama3

./openblas_sgemm 4096 4096 128 | tee llama3/llama3_4096_4096_128

./openblas_sgemm 128 4096 8192 | tee llama3/llama3_128_4096_8192

./openblas_sgemm 128 8192 4096 | tee llama3/llama3_128_8192_4096

./openblas_sgemm 4096 4096 4096 | tee llama3/llama3_4096_4096_4096


./openblas_sgemm 4097 4097 129 | tee llama3/llama3_4097_4097_129

./openblas_sgemm 129 4097 8193 | tee llama3/llama3_129_4097_8193

./openblas_sgemm 129 8193 4097 | tee llama3/llama3_129_8193_4097

./openblas_sgemm 4097 4097 4097 | tee llama3/llama3_4097_4097_4097


./openblas_sgemm 4095 4095 127 | tee llama3/llama3_4095_4095_127

./openblas_sgemm 127 4095 8191 | tee llama3/llama3_127_4095_8191

./openblas_sgemm 127 8191 4095 | tee llama3/llama3_127_8191_4095

./openblas_sgemm 4095 4095 4095 | tee llama3/llama3_4095_4095_4095

