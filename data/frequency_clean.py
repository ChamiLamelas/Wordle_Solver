"""
Script for extracting frequencies of only 5 letter unigrams.

Chami Lamelas
Summer 2022
"""

first = True
with open("unigram_freq.csv", "r", encoding="utf-8") as rf, open("five_letter_freq.csv", "w+", encoding="utf-8") as wf:
    for line in rf:
        comma = line.index(',')
        if (comma == 5):
            if not first:
                wf.write("\n")
            wf.write(line.rstrip())
            if first:
                first = False
