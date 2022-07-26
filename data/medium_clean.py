with open("medium_wordle_words_todate_original.txt", "r", encoding="utf-8") as of, \
        open("medium_wordle_words_todate.txt", "w+", encoding="utf-8") as f:
    first_line = True
    for line in of:
        idx = line.rindex(" ")
        if not first_line:
            f.write("\n")
        f.write(line[idx+1:].rstrip().lower())
        if first_line:
            first_line = False
