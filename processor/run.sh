g++ -g processor.cpp -o proc --std=c++14
g++ -g compiler.cpp -o cmpl --std=c++14
./cmpl source_example bin
./proc bin
