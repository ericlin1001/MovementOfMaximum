all:drawData

genData:main.cpp
	g++ -std=c++11  main.cpp 
	./a.out|tee data.txt


drawData:genData
	python3 ./drawPath.py
