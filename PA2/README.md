# Algorithm PA2

## Goal

algorithm to implement multiple sequence alignment with k DNA sequences.

## TODO

- four-letter alphabet {Adenine (A), Thymine (T), Guanine (G), Cytosine (C)}
- measure the similarity of genetic sequences by the frequency of the exactly matched alphabets
- align the k sequences, but we are permitted to insert gaps in either any sequence
- C programming language to print out the sequence alignment result into the output file named ‘hw2 output.txt after finding the best sequence alignment from k DNA sequences in the input file named ‘hw2 input.txt’

## Input file consists of

- fisrt part : the number (k) of DNA sequences to be aligned
- second part : the k DNA sequences to be aligned (each sequence appears on a separate line of text)
- Each part is separated from the next part by a character $
- 2 ≤ k ≤ 5, and 1 ≤ n ≤ 120 where n is the maximum length of each DNA sequence

## Output file consist of

- the sequence alignment results with marks representing matched alphabets
- In the last line, mark “\*” on the columns containing identical alphabets across all sequences

## Example of input / output file

```
[Input file: hw2_input.txt]
3
$
ATTGCCATT
ATGGCCATT
ATCCAAT

[Output file: hw2_output.txt]
ATTGCCA-TT
ATGGCCA-TT
AT--CCAAT-
**  *** *
```

## Judgement

- the number of identical alphabets across all sequences returned by your submitted program
- the actual running time
- well-written document to explain your source code and the performance analysis of your algorithm

## referece

- [wiki docs for LCS](https://en.wikipedia.org/wiki/Longest_common_subsequence)
- [Align sequences](https://www.biostars.org/p/485136/)
- [LCS of 3 strings](https://www.geeksforgeeks.org/lcs-longest-common-subsequence-three-strings/)
- [Printing Longest Common Subsequence](https://www.geeksforgeeks.org/printing-longest-common-subsequence/)
- [Computing LCS set of Strings](https://ir.nctu.edu.tw/bitstream/11536/4866/1/A1984SR65400005.pdf)
