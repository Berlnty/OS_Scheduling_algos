build:
	g++ clk.cpp -o clock.out
	g++ schedulerHPF.cpp -o sch.out  -std=c++11
	g++ schedulerRR.cpp -o schrr.out -std=c++11
	g++ schedulerSRTN.cpp -o schsrtn.out -std=c++11
	g++ FilesGenerator.cpp -o Files.out
	g++ processGenerator.cpp -o main.out
	g++ process.cpp -o process.out -std=c++11
clean:
	rm -f *.out  processes.txt
	ipcrm -M 300
	ipcrm -Q 777
	@echo "All Items Cleared"

all: clean build

run:
	./main.out
