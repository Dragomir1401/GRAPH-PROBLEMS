# Exemplu de Makefile pentru soluții scrise în C++.

CC = g++
CCFLAGS = -Wall -Wextra -std=c++17 -O0 -lm

.PHONY: build clean

build: supercomputer ferate teleportare magazin

# Nu compilați aici, nici măcar ca dependențe de reguli.
run-p1:
	./supercomputer
run-p2:
	./ferate
run-p3:
	./teleportare
run-p4:
	./magazin

# Schimbați numele surselor (și, eventual, ale executabilelor - peste tot).
p1: supercomputer.cpp
	$(CC) -o $@ $^ $(CCFLAGS)
p2: ferate.cpp
	$(CC) -o $@ $^ $(CCFLAGS)
p3: teleportare.cpp
	$(CC) -o $@ $^ $(CCFLAGS)
p4: magazin.cpp
	$(CC) -o $@ $^ $(CCFLAGS)

# Vom șterge executabilele.
clean:
	rm -f supercomputer ferate teleportare magazin
