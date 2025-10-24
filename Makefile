#
# Simple Makefile, here to run the test-cases.
#



# Build the application
build:
	gcc main.c 


# Run the test-cases with both backends
test: build test-c

# Test the C-backend.
test-c:
	@BACKEND=c make test-implementation


# Actual test cases run here.
# - For each file "examples/*.bf"
# - Run the compiler with that input, and record the output
# - Compare the output to that expected within the corresponding ".out" file.
test-implementation:
	@for i in examples/*.bf; do \
		echo -n "Running test [backend: $$BACKEND] $$i " ; \
		nm=$$(basename $$i .bf) ;\
		./bfcc -backend=${BACKEND} $$i ; \
		if [ -e examples/$${nm}.in ] ; then  \
                       cat examples/$${nm}.in | ./a.out > x ; \
                else  \
		       ./a.out > x ;\
		fi ;\
		diff examples/$${nm}.out x || { echo "Example failed: $$i"; rm x; exit 1; } ;\
		echo "OK" ; \
	done

time:
	make
	./bfcc -backend=c examples/mandelbrot.bf ; bash -c "time ./a.out" >/dev/null
