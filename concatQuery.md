concatQuery is a program written in the C languge to concatenate queries into legible reads. 
There are __ parts to this program:
  1. It reads the header and respective query of each linting in a .fasta file (2 lines per query)
    - the first line is in a format as follows:
```
>scaffold5976_size9010.3 scaffold5976_size9010 + [1:9010]  ( 1384 - 5266 ) Ldec\Orco + 1:1440  ( 1127 - 1440 ) N    627.00;C join(1384..1490,5002..5157,5216..526) ```

    NOTE the placement of parenthases, pluses, minuses, colons, and numbers as syntax is essential.
    - the second line is in a format like the following:
    
```
AGATCAACGGAGTTACCGTGTATGCTGCTACTGTGATAGGTTACCTGGTGTATTCTTTGGCCCAGGTATTCCATTTCTGCATTTTTGGGAACAGGCTGATAGAGGAGAGTTCATCTGTTATGGAAGCAGCTTACAGCTGTCACTGGTATGATGGTTCAGAGGAAGCGAAAACATTCGTCCAGATTGTATGTCAACAATGTCAAAAAGCCTTGTCGATATCTGGGGCGAAGTTTTTCACTATTTCTCTAGATCTTTTTGCCTCGGTACTTGGTGCAGTAGTTACATATTTCATGGTACTGGTACAACTCAAATAA  
```
    The length of the query is stated by the second set of parenthases, 1440 - 1127 = the length of the example query.
    
