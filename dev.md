# Development Plan

-- ranker file

Interface BaseRanker

virtual int SetUp(string temp_fp): Called before Rank() will be called on each of the remaining options in temp_fp by WordleSolver. This allows the ranker to adjust how ranks are calculated if some of the words have been removed from the dataset based on feedback from previous guesses.

virtual int Rank(string word): Given a word, returns a rank for the word.

virtual string Name(): Gets a name for this ranker (should be unique).

-- solver file

Class GuessFailureException: Thrown when a guess cannot be made.

Class WordleSolver

int guesses
BaseRanker* ranker
string temp_fp
string PLACEHOLDER
string prev_guess
set<int> guessed
string dataset_fp

WordleSolver(string dataset_fp, BaseRanker* ranker): Initializes a WordleSolver

void KeepOnPredicate(predicate): Keeps all lines in a file that match a certain predicate

    create temp2 file
    write all lines in temp_fp to temp2 that predicate(line) is true
    write temp2 lines to temp_fp
    delete temp2

void RebuildTemp(string feedback): Rebuilds temporary file based on feedback

    set<char> yellow
    for fc in feedback
        i = idx of fc
        if fc == 'g' and !guessed.find(i)
            KeepOnPredicate(
                predicate that returns true on a word if word[i] == prev_guess[i]
            )
            guessed.insert(i)
    for fc in feedback
        i = idx of fc
        if fc == 'y'
            yellow.insert(prev_guess[i])
            KeepOnPredicate(
                predicate that returns true on a word if prev_guess[i] is in word but not at i or at any index in guessed
            )
        if fc == 'b'
            KeepOnPredicate(
                predicate that works as follows: 
                    if yellow.find(prev_guess[i]) then return false if word[i] == prev_guess[i] 
                    else then return false if prev_guess[i] is anywhere in word besides guessed indices
                    return true
            )


string Guess(): Makes an initial guess. If no guess can be made, throw a GuessFailureException.

    Guess(PLACEHOLDER)

string Guess(string feedback): Makes a guess based on feedback and the previous guesses. Overloaded version of Guess(). If no guess can be made, throw a GuessFailureException.

    if feedback == PLACEHOLDER
        write contents of dataset_fp to temp_fp
        guessed.clear()
        guesses = 0
    else
        RebuildTemp(feedback)

    ranker.SetUp(temp_fp)
    string current_guess
    int current_min_rank
    int current_rank
    open(temp_fp)
    for line in file
        current_rank = ranker.Rank(line)
        if current_rank < current_min_rank
            current_guess = line
            current_min_rank = current_rank
    if current_guess not set
        throw GuessFailureException
    guesses++
    prev_guess = current_guess
    return current_guess

-- evaluation file

string GetFeedback(string guess, string word): Get the feedback (represented as a string of y g and b) to emulate the yellow, green, and black square emojis in the wordle online game given a guess and a word.

    string feedback(5, 'b')
    map<string, int> counts
    for wc in word
        counts[wc]++
        i = idx of wc
        if guess[i] == wc
            feedback[i] = 'g'
            counts[wc]--
    for gc in guess
        i = idx of gc
        if gc != word[i] and counts[gc] > 0
            counts[gc]--
            feedback[i] = 'y'
    return feedback

int Evaluate(WordleSolver solver, string word): Sees how well a WordleSolver does at guessing a particular word. It will return the number of guesses it takes to guess the word or FAILED (some integer not 1-6 to signify that solver failed to find word).

    string guess{solver.Guess()}
    string feedback
    for i = 1 ... 6
        feedback = GetFeedback(guess, word)
        if feedback == "yyyyy"
            return i
        guess = solver.Guess(feedback)
    return FAILED

void GetStatistics(vector<int> guess_counts, double& mean, double& std_dev, int& fail_count): Calculautes the mean, standard deviation,
and failure count and stores them in output parameters (can't return multiple things).

    int total
    for c in guess_counts
        if c == FAILED
            fail_count++
        else
            total += c
    mean = total/guess_counts.size()
    int dev_sq_sum
    for c in guess_counts
        if c != failed
            dev_sq_sum += pow(c-mean,2)
    std_dev = sqrt(dev_sq_sum/n)

void EvaluateDataset(vector<string> dictionary_fps, vector<BaseRanker\*> rankers, string_view words_fp, string_view output_fp): Sees how well a variety of rankers does at guessing a bunch of words in a dataset specified by a filename. The output is displayed in some nice table format to an output file. For each BaseRanker, the table will report for a WordleSolver using the BaseRanker the mean number of guesses it took to guess correct words, the standard deviation in that number of guesses, and lastly how many of the words were guessed within 6 guesses. dictionary_fps supply filepaths to the 5 letter words that the solver starts with, words_fp is a list of words to guess.

    Open output_fp
    vector<string> words
    Read contents of words_fp into words
    vector<int> guess_counts(words)
    double mean
    double std_dev
    int fail_count
    for dfp in dictionary_fps
        for ranker in rankers
            WordleSolver solver(dfp, ranker)
            for word in words
                i = idx of word in words
                guess_counts[i] = Evaluate(solver, word)
            GetStatistics(guess_counts, mean, std_dev, fail_count)
            Write dfp, ranker.Name(), mean, std_dev, fail_count as new line in output_fp
    Close output_fp

-- main file

string ReedFeedback(): Reads feedback from user (from them looking at Wordle website). Keeps asking till the input is valid. Returns lowercase representation.

    string feedback
    bool reading{true}
    do
        Print enter feedback from Wordle
        getline(cin, feedback)
        Trim(feedback)
        If feedback doesn't have 5 chars
            Print invalid length (with the feedback)
            continue
        Lower(feedback)
        If feedback has character besides b, y, or g
            Print invalid feedback (with the lowercased feedback)
        Else
            reading = false
    while reading
    return feedback

void RunUserMode(BaseRanker ranker): Runs the user mode version. This will be akin to:

    WordleSolver solver(ranker)
    string guess = solver.Guess()
    string feedback
    for i = 1 ... 6:
        Print guess
        feedback = ReadFeedback()
        if feedback == "yyyyy"
            Print that solver guessed word in i attempts
            Return
        guess = solver.Guess(feedback)
    Print that solver failed to guess word

int main(): One of the two:

    1) Instantiate vector of BaseRankers and use EvaluateDataset
    2) Instantiate a BaseRanker and use RunUserMode
