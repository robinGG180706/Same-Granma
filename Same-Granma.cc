// Same-Granma.cc
// Guillaume-Gentil Robin
// version 4.1 - English Version
//
// ============================================================================
// LICENSE AND COPYRIGHT
// ============================================================================
// Copyright (c) 2022 Guillaume-Gentil Robin
//
// This code is distributed under an open source license.
//
// You are free to:
//   - Use this code for personal or commercial purposes
//   - Modify and adapt this code to your needs
//   - Distribute this code or modified versions
//
// Conditions:
//   - Attribution is required: you must credit the original author
//     (Guillaume-Gentil Robin) in any use or redistribution
//   - This code is provided "as is", without warranty of any kind
//
// For questions or specific permission requests:
//   Contact: robin_gg@icloud.com
// ============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Error message constants
const string NOT_IN_CAPITAL_LETTERS("The word is not purely in capital letters");
const string DOUBLE_WORD("Each word can be present only once");
const string EMPTY_DICT("The dictionary cannot be empty");
const string NO_ANAGRAM("There is no anagram for this message and this dictionary");

// Structure representing a word with its properties for search optimization
struct Word
{
    string word;  // The original word
    int nbT;      // Total number of letters (Total)
    int nbD;      // Number of distinct letters (Different)
    string alpha; // Letters of the word sorted alphabetically (e.g., "CAB" -> "ABC")
};

typedef vector<Word> Dictionary;

// === FUNCTION PROTOTYPES ===

// Dictionary creation and validation
vector<string> create_dictionary();                          // Reads and validates the dictionary from standard input
bool is_capital_letter(string element);                      // Checks if all characters are uppercase
bool is_duplicate_word(string element, vector<string> list); // Checks if a word is not already present

// Dictionary conversion and preparation
vector<Word> convert(vector<string> list);  // Transforms a list of strings into a dictionary of Word structures
int count_distinct_letters(string element); // Calculates the number of distinct letters in a word
string sort_letters(string element);        // Sorts the letters of a word alphabetically

// Dictionary sorting functions (hierarchical sorting on 4 levels)
vector<Word> sort_by_total(vector<Word> dict);       // Sort by total number of letters
vector<Word> sort_by_distinct(vector<Word> dict);    // Sort by number of distinct letters
vector<Word> sort_by_alpha(vector<Word> dict);       // Sort by sorted letters order
vector<Word> sort_alphabetically(vector<Word> dict); // Alphabetical sort of the original word

// Message management
vector<vector<string>> create_messages();      // Reads messages to analyze from standard input
void display_dictionary(vector<Word> dict);    // Displays all words in the dictionary
bool is_all_uppercase(vector<string> message); // Checks that all words in the message are uppercase

// Letter manipulation for search
string sort_message_letters(string message);                   // Sorts the letters of a message alphabetically
bool message_contains_word(string alpha, Word element);        // Checks if the message contains all letters of a word
string subtract_word_from_message(string alpha, Word element); // Removes the letters of a word from the message

// Anagram search algorithm (recursive backtracking)
bool search_anagram(vector<Word> dict, string alpha_m, vector<string> anagram);
// Recursive function that finds all possible anagrams

void find_anagrams(string message, vector<Word> dict);
// Initializes and launches the anagram search for a message

vector<Word> adapt_dictionary(vector<Word> dict, string alpha);
// Optimizes the dictionary by removing impossible-to-use words

vector<Word> remove_word(vector<Word> dict, int i);
// Removes the i-th word from the dictionary

int main()
{
    // Step 1: Dictionary creation
    vector<string> raw_list;
    raw_list = create_dictionary(); // Reading and validation of words

    // Step 2: Conversion and sorting of the dictionary
    Dictionary dict;
    dict = convert(raw_list);         // Calculation of properties (nbT, nbD, alpha)
    dict = sort_by_total(dict);       // Sort 1: by number of letters
    dict = sort_by_distinct(dict);    // Sort 2: by distinct letters
    dict = sort_by_alpha(dict);       // Sort 3: by sorted letters
    dict = sort_alphabetically(dict); // Sort 4: alphabetical order

    // Step 3: Reading messages to analyze
    vector<vector<string>> message_list;
    message_list = create_messages();

    // Step 4: Display of the sorted dictionary
    display_dictionary(dict);
    if (!message_list.empty())
    {
        cout << endl; // Empty line between dictionary and results
    }

    // Step 5: Processing each message
    string message;
    for (size_t j(0); j < message_list.size(); j++)
    {
        if (is_all_uppercase(message_list[j]))
        { // Check that the message is uppercase
            // Concatenation of all words of the message into a single string
            for (auto element : message_list[j])
            {
                for (auto letter : element)
                {
                    message.push_back(letter);
                }
            }
            // Search and display of anagrams
            find_anagrams(message, dict);
        }
        if (j != message_list.size() - 1)
        {
            cout << endl; // Empty line between results of different messages
        }
        message.clear(); // Reset for next message
    }
    return 0;
}

// Reads and validates the dictionary from standard input
// Format: uppercase words separated by spaces, terminated by "."
// Possible errors: empty dictionary, lowercase word, duplicate word
vector<string> create_dictionary()
{
    string element;
    bool loop(true);
    vector<string> raw_list;
    do
    {
        cin >> element;
        if (element == ".")
        { // End of dictionary
            if (raw_list.empty())
            {
                cout << EMPTY_DICT << endl;
                exit(0);
            }
            else
            {
                loop = false; // Valid dictionary, exit loop
            }
        }
        else
        { // New word to add
            if (is_capital_letter(element))
            { // Check uppercase
                if (is_duplicate_word(element, raw_list))
                { // Check no duplicate
                    raw_list.push_back(element);
                }
                else
                {
                    cout << DOUBLE_WORD << endl;
                    exit(0);
                }
            }
            else
            {
                cout << NOT_IN_CAPITAL_LETTERS << endl;
                exit(0);
            }
        }
    } while (loop);
    return raw_list;
}

// Checks that all characters of a word are uppercase (A-Z: ASCII 65-90)
bool is_capital_letter(string element)
{
    int character;
    for (size_t i(0); i < element.size(); i++)
    {
        character = element[i];
        if ((character < 65) or (character > 90))
        { // Outside the A-Z range
            return false;
        }
    }
    return true;
}

// Checks that a word is not already present in the list (no duplicates)
bool is_duplicate_word(string element, vector<string> list)
{
    for (auto word : list)
    {
        if (word == element)
        {
            return false; // Word found = duplicate
        }
    }
    return true; // Word not yet in the list
}

// Converts a list of strings into a dictionary of Word structures
// Calculates for each word: nbT, nbD, alpha
vector<Word> convert(vector<string> list)
{
    Word w;
    Dictionary dict;

    for (auto element : list)
    {
        w.word = element;                        // Original word
        w.nbT = element.size();                  // Total number of letters
        w.nbD = count_distinct_letters(element); // Number of distinct letters
        w.alpha = sort_letters(element);         // Letters sorted alphabetically
        dict.push_back(w);
    }
    return dict;
}

// Calculates the number of distinct letters in a word
// Example: "HELLO" -> 4 distinct letters (H, E, L, O)
int count_distinct_letters(string element)
{
    vector<char> distinct_chars;
    bool new_character;
    distinct_chars.push_back(element[0]); // First letter always added
    for (auto letter1 : element)
    {
        new_character = true;
        // Check if the letter is already in the list
        for (auto letter2 : distinct_chars)
        {
            if (letter1 == letter2)
            {
                new_character = false;
            }
        }
        if (new_character)
        {
            distinct_chars.push_back(letter1);
        }
    }
    return distinct_chars.size();
}

// Sorts the letters of a word alphabetically
// Example: "CAB" -> "ABC", "HELLO" -> "EHLLO"
// Uses insertion sort
string sort_letters(string element)
{
    string characters;
    characters.push_back(element[0]); // First letter
    bool loop(true);
    size_t j(0);
    // For each following letter, find its position in alphabetical order
    for (size_t i(1); i < element.size(); i++)
    {
        loop = true;
        j = 0;
        while (loop)
        {
            if (j >= characters.size())
            {
                // Larger than all existing letters: add to the end
                characters.push_back(element[i]);
                loop = false;
            }
            else
            {
                if (characters[j] > element[i])
                {
                    // Find insertion position
                    characters.insert(j, 1, element[i]);
                    loop = false;
                }
            }
            j += 1;
        }
    }
    return characters;
}

// Sort level 1: by total number of letters (ascending)
// Uses bubble sort
vector<Word> sort_by_total(vector<Word> dict)
{
    Word temp_element;
    for (size_t j(0); j < (dict.size() - 1); j++)
    {
        for (size_t i(0); i < (dict.size() - 1); i++)
        {
            if (dict[i].nbT > dict[i + 1].nbT)
            {
                // Swap if current word is longer
                temp_element = dict[i];
                dict[i] = dict[i + 1];
                dict[i + 1] = temp_element;
            }
        }
    }
    return dict;
}

// Sort level 2: by number of distinct letters (ascending)
// Only sorts if nbT is identical (stable sort)
vector<Word> sort_by_distinct(vector<Word> dict)
{
    Word temp_element;
    for (size_t j(0); j < (dict.size() - 1); j++)
    {
        for (size_t i(0); i < (dict.size() - 1); i++)
        {
            if ((dict[i].nbD > dict[i + 1].nbD) and (dict[i].nbT == dict[i + 1].nbT))
            {
                // Swap only if same length
                temp_element = dict[i];
                dict[i] = dict[i + 1];
                dict[i + 1] = temp_element;
            }
        }
    }
    return dict;
}

// Sort level 3: by sorted letters order (alpha)
// Compares character by character the alphabetized versions
// Only sorts if nbT and nbD are identical
vector<Word> sort_by_alpha(vector<Word> dict)
{
    Word temp_element;
    int alpha1(0);
    int alpha2(0);
    int h(0);
    for (size_t j(0); j < (dict.size() - 1); j++)
    {
        for (size_t i(0); i < (dict.size() - 1); i++)
        {
            alpha1 = 0;
            alpha2 = 0;
            h = 0;
            // Find the first different character
            while (alpha1 == alpha2)
            {
                alpha1 = dict[i].alpha[h];
                alpha2 = dict[i + 1].alpha[h];
                h += 1;
            }
            // Swap if same nbT and nbD, but different alpha
            if (((alpha1 > alpha2) and (dict[i].nbD == dict[i + 1].nbD)) and
                (dict[i].nbT == dict[i + 1].nbT))
            {
                temp_element = dict[i];
                dict[i] = dict[i + 1];
                dict[i + 1] = temp_element;
            }
        }
    }
    return dict;
}

// Sort level 4: alphabetical order of the original word
// Classic lexicographic comparison
// Only sorts if nbT, nbD and alpha are identical
vector<Word> sort_alphabetically(vector<Word> dict)
{
    Word temp_element;
    int char1(0);
    int char2(0);
    int h(0);
    for (size_t j(0); j < (dict.size() - 1); j++)
    {
        for (size_t i(0); i < (dict.size() - 1); i++)
        {
            char1 = 0;
            char2 = 0;
            h = 0;
            // Find the first different character in the original words
            while (char1 == char2)
            {
                char1 = dict[i].word[h];
                char2 = dict[i + 1].word[h];
                h += 1;
            }
            // Swap if all previous properties are identical
            if (((char1 > char2) and (dict[i].nbD == dict[i + 1].nbD)) and
                ((dict[i].nbT == dict[i + 1].nbT) and (dict[i].alpha == dict[i + 1].alpha)))
            {
                temp_element = dict[i];
                dict[i] = dict[i + 1];
                dict[i + 1] = temp_element;
            }
        }
    }
    return dict;
}

// Reads messages to analyze from standard input
// Format: words separated by spaces, "." separates messages, "*" terminates
// Returns a vector of messages (each message = vector of words)
vector<vector<string>> create_messages()
{
    string element;
    vector<vector<string>> message_list;
    vector<string> message;
    do
    {
        cin >> element;
        if (element == "*")
        { // End of all messages
            if (!message.empty())
            {
                message_list.push_back(message); // Add last message
                message.clear();
            }
            return message_list;
        }
        else if (element == ".")
        { // End of a message
            message_list.push_back(message);
            message.clear();
        }
        else
        { // New word of current message
            message.push_back(element);
        }
    } while (true);
}

// Displays all words in the dictionary (one per line)
void display_dictionary(vector<Word> dict)
{
    for (auto element : dict)
    {
        cout << element.word << endl;
    }
}

// Sorts the letters of a message alphabetically (like sort_letters but for messages)
// Uses bubble sort
string sort_message_letters(string message)
{
    char temp;
    for (size_t i(0); i < message.size() - 1; i++)
    {
        for (size_t j(0); j < message.size() - 1; j++)
        {
            if (message[j] > message[j + 1])
            {
                temp = message[j];
                message[j] = message[j + 1];
                message[j + 1] = temp;
            }
        }
    }
    return message;
}

// Checks if a message (sorted) contains all letters of a word
// Uses a copy to mark already used letters with '0'
bool message_contains_word(string alpha, Word element)
{
    int pos;
    for (int i(0); i < element.nbT; i++)
    {
        pos = alpha.find(element.alpha[i]); // Search for the letter
        if (pos != -1)
        {
            alpha[pos] = '0'; // Mark as used
        }
        else
        {
            return false; // Missing letter
        }
    }
    return true; // All letters are present
}

// Removes the letters of a word from the message (sorted version)
// Used after adding a word to the anagram
string subtract_word_from_message(string alpha, Word element)
{
    for (int i(0); i < element.nbT; i++)
    {
        // Remove first occurrence of each letter
        alpha.replace(alpha.find(element.alpha[i]), 1, "");
    }
    return alpha;
}

// Checks that all words in a message are uppercase
bool is_all_uppercase(vector<string> message)
{
    bool uppercase(true);
    for (size_t i(0); i < message.size(); i++)
    {
        if (!is_capital_letter(message[i]))
        {
            cout << NOT_IN_CAPITAL_LETTERS << endl;
            uppercase = false;
        }
    }
    return uppercase;
}

// Recursive anagram search function (backtracking algorithm)
// Parameters:
//   - dict: dictionary of still usable words
//   - alpha_m: remaining message letters (sorted)
//   - anagram: words already selected for the current anagram
// Returns true if at least one anagram was found
bool search_anagram(vector<Word> dict, string alpha_m, vector<string> anagram)
{
    if (dict.empty())
    {
        return false; // No more words available, failure
    }
    bool success(false);
    // Try each word in the dictionary
    for (size_t i(0); i < dict.size(); i++)
    {
        // Check if the word can be formed with remaining letters
        if (message_contains_word(alpha_m, dict[i]))
        {
            // Create a new branch: add this word to the anagram
            vector<string> next_anagram = anagram;
            next_anagram.push_back(dict[i].word);
            // Remove used letters from the message
            string next_alpha_m = subtract_word_from_message(alpha_m, dict[i]);

            if (next_alpha_m.empty())
            {
                // No remaining letters = complete anagram found!
                for (size_t j(0); j < next_anagram.size(); j++)
                {
                    cout << next_anagram[j];
                    if (j != next_anagram.size() - 1)
                    {
                        cout << " "; // Space between words
                    }
                }
                cout << "\n";
                success = true;
            }
            else
            {
                // Letters remain: continue search recursively
                // Remove current word from dictionary to avoid duplicates
                vector<Word> next_dict = remove_word(dict, i);
                if (search_anagram(next_dict, next_alpha_m, next_anagram))
                {
                    success = true;
                }
            }
        }
    }
    return success;
}

// Prepares and launches anagram search for a message
// Initializes structures and optimizes dictionary before recursive call
void find_anagrams(string message, vector<Word> dict)
{
    vector<string> anagram; // Empty vector to start recursion
    string alpha_m;
    alpha_m = sort_message_letters(message); // Sort message letters
    dict = adapt_dictionary(dict, alpha_m);  // Optimization: remove impossible words

    // Launch recursive search
    if (!search_anagram(dict, alpha_m, anagram))
    {
        cout << NO_ANAGRAM << endl; // No anagram found
    }
}

// Optimizes the dictionary by removing all words that cannot be formed
// with the message letters (improves performance)
vector<Word> adapt_dictionary(vector<Word> dict, string alpha_m)
{
    int size(dict.size());
    for (int i(0); i < size; i++)
    {
        if (!message_contains_word(alpha_m, dict[i]))
        {
            // Shift all following elements to the left
            for (size_t j(i); j < (dict.size() - 1); j++)
            {
                dict[j] = dict[j + 1];
            }
            dict.pop_back(); // Remove last element
        }
    }
    return dict;
}

// Removes the i-th word from the dictionary
// Used to avoid reusing a word already in the anagram
vector<Word> remove_word(vector<Word> dict, int i)
{
    // Shift all following elements to the left
    for (size_t j(i); j < dict.size() - 1; j++)
    {
        dict[j] = dict[j + 1];
    }
    dict.pop_back(); // Remove last element
    return dict;

}
