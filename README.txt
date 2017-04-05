This is the code for a New Mexico supercomputing challenge project.
The code is devided up in the following fashion:
../
|- TwitterCorpa
|   + Collect a random sample of tweets, and output it to /data
|- AccumulateTwitterDatabase
|   + Process the random tweets, calculating the frequency of 1-grams
|- PrecompileLiterature
|   + Pre-compiles the massive quantity of Google data, making later computations faster
|- CompileLiterature
|   + Processes the pre-compiled data, eliminates all non-words, and outputs n-grams in order from least frequent to most frequent
|- 
|