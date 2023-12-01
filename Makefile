test:
	@for d in $(shell ls -d ${PWD}/*/ | grep day_); do \
		cd $${d} && echo '\nTesting:' && pwd && gcc main_test.c && ./a.out time > output.txt && gcc main.c && ./a.out >> output.txt && gcc main_test.c && ./a.out && rm a.out && rm output.txt; \
	done

.PHONY: test
