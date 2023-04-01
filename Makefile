all:
	g++ src/vega.c src/execute.c -I./src -lreadline -o vega
