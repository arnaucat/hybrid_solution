SERIAL_TARGET_DYN=knapsackDYN_serial
HYBRID_TARGET_DYN=knapsackDYN_hybrid
RESULTS=results.csv
RESULTS_SPEEDUPS=results_speedups.csv
RESULTS_TIMES=results_times_per_problem.csv
OUTPUT_FOLER=output
ERROR_FOLDER=errors

all : serial hybrid

bb : $(SERIAL_TARGET_DYN) $(HYBRID_TARGET_DYN)

serial : $(SERIAL_TARGET_DYN)

$(SERIAL_TARGET_DYN) : knapsackDYN_serial.c
	gcc knapsackDYN_serial.c -o $(SERIAL_TARGET_DYN)

hybrid : $(HYBRID_TARGET_DYN)

$(HYBRID_TARGET_DYN) : knapsackDYN_hybrid.c
	mpicc knapsackDYN_hybrid.c -o $(HYBRID_TARGET_DYN)

run-moore : bb
	./run_all.sh

results : $(RESULTS) $(RESULTS_TIMES) $(RESULTS_SPEEDUPS)

$(RESULTS) : $(OUTPUT_FOLER)/*
	./get_results.sh > $(RESULTS)

$(RESULTS_TIMES) : $(RESULTS)
	./get_times_per_problem.py $(RESULTS) > $(RESULTS_TIMES)

$(RESULTS_SPEEDUPS) : $(RESULTS)
	./get_speedups.py $(RESULTS) > $(RESULTS_SPEEDUPS)

