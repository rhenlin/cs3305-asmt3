asmt3: asn3.c
	gcc -pthread -o asmt3 asn3.c -I.

.PHONY: run
run: 	asmt3 assignment_3_input_file.txt.
	./asmt3 assignment_3_input_file.txt

clean:
	rm -f asmt3*.o core asmt3
