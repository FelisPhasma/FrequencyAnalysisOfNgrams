Frequency Analysis of Ngrams
Copyright (c) Jeremy Rifkin 2017

This is the code for a New Mexico supercomputing challenge project.
The code is devided up in the following fashion:

../
|- data
|
|  + All the raw and precompiled data
|
|- DataInstall
|
|  + Automates the installation process of the Google N-gram data. 
|
|- output
|
|  + Final results from the project
|
|- TwitterCorpa
|
|  + Collect a random sample of tweets, and output it to /data
|
|- AccumulateTwitterDatabase
|
|  + Process the random tweets, calculating the frequency of 1-grams
|
|- PrecompileLiterature
|
|  + Pre-compiles the massive quantity of Google data, making later 
|    computations faster
|
|- CompileLiterature
|
|  + Processes the pre-compiled data, eliminates all non-words, and 
|    outputs n-grams in order from least frequent to most frequent
|
|- Analysis
|
|  +  R code for analyzing the data
|
♦