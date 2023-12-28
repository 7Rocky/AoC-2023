test:
	@for d in $(shell ls -d ${PWD}/*/ | grep day_); do \
		cd $${d} && echo '\nTesting:' && pwd && gcc -O3 main_test.c -o test && gcc -O3 main.c && ./test time > output.txt && ./a.out >> output.txt && ./test && rm a.out test output.txt; \
	done

.PHONY: test
