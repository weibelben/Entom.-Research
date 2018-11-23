concatQuery: concatQuery.c concatGapQuery.c concatReads.c alignReads.c alignFromList.c concatQualQuery.c
	gcc -Wall -m32 -std=gnu99 concatQuery.c -o concatQuery
	gcc -Wall -m32 -std=gnu99 concatGapQuery.c -o concatGap
	gcc -Wall -m32 -std=gnu99 concatReads.c -o concatReads
	gcc -Wall -m32 -std=gnu99 alignReads.c -o alignReads
	gcc -Wall -m32 -std=gnu99 alignFromList.c -o alignFromList
	gcc -Wall -m32 -std=gnu99 concatQualQuery.c -o concatQual
cat:
	./concatQuery *.fasta
gap:
	./concatGap *.fasta Ldec_lengths.list
qual:
	./concatQual *.fasta Ldec_lengths.list
read:
	./concatReads *.out Ldec_lengths.list concatReads_out.txt
align:
	./alignFromList *.out Ldec_lengths.list
clean:
	rm -f *.out
cleanest:
	rm -f *.out #removes concatQuery outputs
	rm -f *.phy #removes alignReads outputs
	rm -f *.txt #this will be changed. deps on output of concatReads
help:
	cat Helpfile
