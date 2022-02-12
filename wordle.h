#ifndef WORDLE_H_
#define WORDLE_H_

#include <array>
#include <string>

#define WORD_LENGTH 5
#define MAXIMUM_GUESSES 6
#define NUMBER_OF_POSSIBLE_ANSWERS 2315
#define NUMBER_OF_ALLOWED_GUESSES NUMBER_OF_POSSIBLE_ANSWERS + 10657

typedef enum { CORRECT, ELSEWHERE, WRONG, UNDECIDED } Feedback;

class Wordle {
   public:
	void playGame();
	Wordle();

   private:
	std::string correctAnswer;
	std::array<std::string, NUMBER_OF_POSSIBLE_ANSWERS> possibleAnswers;
	std::array<std::string, NUMBER_OF_ALLOWED_GUESSES> allowedGuesses;

	std::array<Feedback, WORD_LENGTH> getFeedback(std::string guess);
};

#endif