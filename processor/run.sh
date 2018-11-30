g++ -g processor.cpp -o proc --std=c++14
g++ -g compiler.cpp -o cmpl --std=c++14
g++ -g decompiler.cpp -o dcmpl --std=c++14
./cmpl sqr2 bin
./dcmpl bin source_example_decompile
./proc bin
