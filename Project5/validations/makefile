#put any files which when changed require the simulator to be recompiled here
#the structure of the files doesnt matter this will deffer to the other makefile 
#if it detects the need to recompile
CFILES = driver.c student_cache.c student_cache.h

.PHONY:clean bz2_sim cg_sim gcc_sim go_sim mcf_sim parser_sim

check:bz2_sim cg_sim gcc_sim go_sim mcf_sim parser_sim
	echo All validation passed
	
bz2_sim:simulator Traces/bzip2_trace.txt Validation/bz2_sim
	./simulator > $@ < Traces/bzip2_trace.txt -c 10 -b 2 -s 0
	diff Validation/$@ $@ > $@_errors
cg_sim:simulator Traces/cg_trace.txt Validation/cg_sim
	./simulator > $@ < Traces/cg_trace.txt -c 5 -b 3 -s 2 -p
	diff Validation/$@ $@ > $@_errors
gcc_sim:simulator Traces/gcc_trace.txt Validation/gcc_sim
	./simulator > $@ < Traces/gcc_trace.txt -c 7 -b 1 -s 1 -p
	diff Validation/$@ $@ > $@_errors
go_sim:simulator Traces/go_trace.txt Validation/go_sim
	./simulator > $@ < Traces/go_trace.txt -c 10 -b 4 -s 0 -p
	diff Validation/$@ $@ > $@_errors
mcf_sim:simulator Traces/mcf_trace.txt Validation/mcf_sim
	./simulator > $@ < Traces/mcf_trace.txt -c 10 -b 2 -s 8
	diff Validation/$@ $@ > $@_errors
parser_sim:simulator Traces/parser_trace.txt Validation/parser_sim
	./simulator > $@ < Traces/parser_trace.txt -c 6 -b 1 -s 2
	diff Validation/$@ $@ > $@_errors
simulator:Makefile $(CFILES)
	make -f Makefile simulator

clean:
	rm -f bz2_sim bz2_sim_errors
	rm -f cg_sim cg_sim_errors
	rm -f gcc_sim gcc_sim_errors
	rm -f go_sim go_sim_errors
	rm -f mcf_sim mcf_sim_errors
	rm -f parser_sim parser_sim_errors
