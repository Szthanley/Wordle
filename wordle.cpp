#include "wordle.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

// Function that provides feedback for each letter of a guess
std::array<Feedback, WORD_LENGTH> Wordle::getFeedback(std::string guess) {
	// Array to store the feedback for each letter
	std::array<Feedback, WORD_LENGTH> feedback;

	// Looping through each letter of the guess and the correct answer
	for (int i = 0; i < WORD_LENGTH; i++) {
		if (guess[i] == correctAnswer[i]) {
			// If the letter is correct and in the correct place...
			feedback[i] = CORRECT;
		} else if (correctAnswer.find(guess[i]) == std::string::npos) {
			// If the letter is not in the word at all...
			feedback[i] = WRONG;
		} else {
			// If the letter is in the word but not at this place...
			feedback[i] = UNDECIDED;
		}
	}

	// Looping through each letter of the guess and the correct answer again
	for (int i = 0; i < WORD_LENGTH; i++) {
		// Checking if the feedback for this letter is undecided so far
		if (feedback[i] == UNDECIDED) {
			/* Getting the number of occurrences of this letter in the correct
			answer */
			int numberOfOccurrences = std::count(correctAnswer.begin(),
												 correctAnswer.end(), guess[i]);

			/* Variable to store the number of decided feedbacks that have been
			given for this letter */
			int numberOfFeedbacks = 0;
			/* Looping through each letter of the guess and correct answer (this
			time with j instead of i), stopping early if the number of
			feedbacks reaches the number of occurrences */
			for (int j = 0;
				 (j < WORD_LENGTH) && (numberOfFeedbacks < numberOfOccurrences);
				 j++) {
				// Checking if at this index, the same letter was given feedback
				if ((guess[i] == guess[j]) && (feedback[j] != UNDECIDED)) {
					numberOfFeedbacks++;
				}
			}

			/* The logic here is that the number of times a CORRECT or ELSEWHERE
			feedback is given for a single letter should never exceed the
			number of occurrences of that letter in the correct answer.
			For example, if a guess contains 2 e's and one e is in the correct
			place, and the correct answer has only one e, the other e should not
			say ELSEWHERE. */
			if (numberOfFeedbacks < numberOfOccurrences) {
				feedback[i] = ELSEWHERE;
			} else {
				feedback[i] = WRONG;
			}
		}
	}

	return feedback;
}

void Wordle::playGame() {
	for (int numberOfUserGuesses = 1; numberOfUserGuesses <= MAXIMUM_GUESSES;
		 numberOfUserGuesses++) {
		std::string userGuess;
		// Looping until a valid guess is given
		while (true) {
			std::cout << "Guess a word: ";

			std::cin >> userGuess;

			// Making sure that the guess is the correct length
			if (userGuess.size() != WORD_LENGTH) {
				std::cout << "Please enter a word that is exactly "
						  << WORD_LENGTH << " letters long. ";
				continue;
			}

			/* Making sure that the guess is a word that can be guessed
			 * (std::find returns the end of the array if the value is not
			 * found) */
			if (std::find(allowedGuesses.begin(), allowedGuesses.end(),
						  userGuess) == allowedGuesses.end()) {
				std::cout << "That is not a valid word. ";
				continue;
			}

			// If the code reaches this point, the guess is valid
			break;
		}

		std::array<Feedback, WORD_LENGTH> feedback = getFeedback(userGuess);

		for (Feedback letterFeedback : feedback) {
			std::cout << "[ ";

			// Using a switch statement to convert the feedback into a string
			switch (letterFeedback) {
				case CORRECT:
					std::cout << "CORRECT";
					break;
				case ELSEWHERE:
					std::cout << "ELSEWHERE";
					break;
				case WRONG:
					std::cout << "WRONG";
					break;
			}

			std::cout << " ] ";
		}

		std::cout << "\n";

		// Checking if the guess was correct
		if (userGuess == correctAnswer) {
			std::cout
				<< "Congratulations, you got the right answer! It took you "
				<< numberOfUserGuesses << " guesses.";
			return;
		}
	}

	/* If the code gets to this point, the user did not guess the correct word
	in few enough guesses */
	std::cout << "Unfortunately, you did not guess the word in "
			  << MAXIMUM_GUESSES << " guesses. The correct word was "
			  << correctAnswer << ".";
}

Wordle::Wordle() {
	// Opening the text files
	std::ifstream answersFile("wordle_answers.txt");
	std::ifstream guessesFile("wordle_guesses.txt");

	// Checking if the files were opened without any errors
	if (!answersFile || !guessesFile) {
		std::cout << "Could not open files";
		return;
	}

	// Looping through the possible answers
	for (int i = 0; i < NUMBER_OF_POSSIBLE_ANSWERS; i++) {
		// Reading a line from the answers file
		std::string possibleAnswer;
		std::getline(answersFile, possibleAnswer);

		/* Storing the word to both arrays (since a possible answer is also an
		allowed guess) */
		possibleAnswers[i] = possibleAnswer;
		allowedGuesses[i] = possibleAnswer;
	}

	// Closing the answers file
	answersFile.close();

	// Looping through the allowed guesses (not including the possible answers)
	for (int i = NUMBER_OF_POSSIBLE_ANSWERS; i < NUMBER_OF_ALLOWED_GUESSES;
		 i++) {
		// Reading a line from the guesses file into the allowed guesses array
		std::getline(guessesFile, allowedGuesses[i]);
	}

	// Closing the guesses file
	guessesFile.close();

	// Device that seeds the random number generator
	std::random_device rd;
	// The actual random number generator
	std::mt19937 gen(rd());
	// Distribution that chooses a random index of the possible answers array
	std::uniform_int_distribution<std::mt19937::result_type> dist(
		0, NUMBER_OF_POSSIBLE_ANSWERS - 1);
	// Chooses the word at the randomly-generated index to be the correct answer
	correctAnswer = possibleAnswers[dist(gen)];
}