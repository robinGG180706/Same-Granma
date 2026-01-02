# Same-Granma

Same-Granma is an anagram search algorithm developed in C++. It allows you to find all possible combinations of words in a dictionary that form an anagram of a given sentence.

The algorithm uses an optimized recursive backtracking technique with a hierarchical sorting of the dictionary on four levels to improve search performance.

The aim of this project is to put into practice the programming concepts related to algorithm creation as part of the "Information, Computation, Communication" course (CSS-119(c)) at EPFL.

## Features

* **Recursive Backtracking**: Efficiently explores all possible word combinations
* **4-Level Hierarchical Sorting**: Optimizes dictionary search by sorting words based on:
  1. Number of unique letters (`nbT`)
  2. Number of duplicate letters (`nbD`)
  3. Alphabetical value sum (`alpha`)
  4. Alphabetical order
* **Dictionary Optimization**: Dynamically filters the dictionary at each recursion level to eliminate impossible words
* **Multiple Format Support**: Available in C++ (original) and JavaScript (web version)
* **Multi-Message Processing**: Process multiple anagram searches in a single run

## How It Works

1. **Dictionary Loading**: Reads a dictionary of words from standard input
2. **Word Analysis**: Each word is analyzed and classified by:
   - Number of unique letters
   - Number of duplicate letters
   - Sum of alphabetical values
3. **Hierarchical Sorting**: Dictionary is sorted on 4 levels for optimal search
4. **Recursive Search**: For each message:
   - Converts the message to a searchable format
   - Adapts the dictionary to contain only possible words
   - Recursively searches for valid anagram combinations
   - Backtracks when a path is invalid

## Installation

### Requirements
- C++ compiler with C++11 support or later (g++, clang++, MSVC)
- Standard C++ library

### Compilation

g++ -std=c++11 -o Same-Granma.cc

## Usage

### Input Format

The program reads from standard input with the following format:

\`\`\`
word1
word2
word3
.
message to find anagrams.
another message.
*
\`\`\`

* **Dictionary**: List of words, one per line, terminated by a single dot (.)
* **Messages**: Sentences to find anagrams for, each ending with a dot (.)
* **End Marker**: A single asterisk (*) signals the end of input

## Web Version
An interactive JavaScript version is available at: [[Same-Granma](https://robingg180706.github.io/same-granma.html)]

The web interface allows you to:

- Enter a dictionary directly in your browser
- Search for anagrams in real-time

## Algorithm Complexity
* **Time Complexity**: O(n! × m) in worst case, where n is the number of words and m is the dictionary size
* **Space Complexity**: O(n + m) for storing the dictionary and recursion stack
* **Optimization**: The 4-level sorting and dictionary filtering significantly reduce the search space in practice
## Version History
**v4.1** (December 2022): Current version with optimized performance

## License
This code is open source and free to use. If you use this code in your projects, please provide attribution:

Copyright © 2026 Guillaume-Gentil Robin

## Author
**Guillaume-Gentil Robin**

* **Email**: robin_gg@icloud.com
* **GitHub**: robinGG180706

## Acknowledgments
Developed as part of the CSS-119(c) course at EPFL (École Polytechnique Fédérale de Lausanne).

## Contributing
Contributions, issues, and feature requests are welcome! Feel free to check the issues page or submit a pull request.
