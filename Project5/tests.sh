make -B
echo "parser"
./simulator -c 6 -b 1 -s 2 < Traces/parser_trace.txt > output.txt
diff output.txt validations/parser_sim
echo "bzip2"
./simulator -c 10 -b 2 -s 0 < Traces/bzip2_trace.txt > output.txt
diff output.txt validations/bz2_sim
echo "cg"
./simulator -c 5 -b 3 -s 2 -P < Traces/cg_trace.txt > output.txt
diff output.txt validations/cg_sim
echo "gcc"
./simulator -c 7 -b 1 -s 1 -P < Traces/gcc_trace.txt> output.txt
diff output.txt validations/gcc_sim
echo "go"
./simulator -c 10 -b 4 -s 0 -P < Traces/go_trace.txt > output.txt
diff output.txt validations/go_sim
echo "mcf"
./simulator -c 10 -b 2 -s 8 < Traces/mcf_trace.txt > output.txt
diff output.txt validations/mcf_sim
