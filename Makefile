SERIAL_TARGET_DYN=knapsackDYN_serial
HYBRID_TARGET_DYN=knapsackDYN_hybrid

all : serial hybrid

bb : $(SERIAL_TARGET_DYN) $(HYBRID_TARGET_DYN)

serial : $(SERIAL_TARGET_DYN)

$(SERIAL_TARGET_DYN) : knapsackDYN_serial.c
	gcc knapsackDYN_serial.c -o $(SERIAL_TARGET_DYN)

hybrid : $(HYBRID_TARGET_DYN)

$(HYBRID_TARGET_DYN) : knapsackDYN_hybrid.c
	mpicc knapsackDYN_hybrid.c -o $(HYBRID_TARGET_DYN)

run : bb
	./run_all.sh
