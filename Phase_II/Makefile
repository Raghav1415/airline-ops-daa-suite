CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2

SRC = sorting_searching.c divide_conquer.c greedy_graphs.c \
      dynamic_programming.c backtracking_bnb.c \
      string_number_theory.c test_suite.c main.c

OBJ = $(SRC:.c=.o)
TARGET = aviation_suite

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f *.o $(TARGET) $(TARGET).exe benchmark_report.csv recovery_schedule.csv recovery_plan.json