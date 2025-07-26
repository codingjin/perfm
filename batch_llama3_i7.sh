#!/bin/bash

make 

modelname="llama3"
dirname="llama3_i7"


mkdir -p ${modelname}/${dirname}

./openblas_sgemm 4096 4096 128 | tee ${modelname}/${dirname}/${modelname}_4096_4096_128

./openblas_sgemm 128 4096 8192 | tee ${modelname}/${dirname}/${modelname}_128_4096_8192

./openblas_sgemm 128 8192 4096 | tee ${modelname}/${dirname}/${modelname}_128_8192_4096

./openblas_sgemm 4096 4096 4096 | tee ${modelname}/${dirname}/${modelname}_4096_4096_4096


./openblas_sgemm 4097 4097 129 | tee ${modelname}/${dirname}/${modelname}_4097_4097_129

./openblas_sgemm 129 4097 8193 | tee ${modelname}/${dirname}/${modelname}_129_4097_8193

./openblas_sgemm 129 8193 4097 | tee ${modelname}/${dirname}/${modelname}_129_8193_4097

./openblas_sgemm 4097 4097 4097 | tee ${modelname}/${dirname}/${modelname}_4097_4097_4097


./openblas_sgemm 4095 4095 127 | tee ${modelname}/${dirname}/${modelname}_4095_4095_127

./openblas_sgemm 127 4095 8191 | tee ${modelname}/${dirname}/${modelname}_127_4095_8191

./openblas_sgemm 127 8191 4095 | tee ${modelname}/${dirname}/${modelname}_127_8191_4095

./openblas_sgemm 4095 4095 4095 | tee ${modelname}/${dirname}/${modelname}_4095_4095_4095

