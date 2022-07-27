"""
Script for removing numbers from word find dictionary file.

Chami Lamelas
Summer 2022
"""

with open("wordfind_5letter_words_original.txt", 'r', encoding='utf-8') as f, \
    open("wordfind_5letter_words.txt", 'w', encoding='utf-8') as w:
    for line in f:
        w.write(line.lstrip().rstrip("\n0123456789") + "\n")
