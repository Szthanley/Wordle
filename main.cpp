#include <array>
#include <iostream>

#include "wordle.h"

using namespace std;

int main() {
	// some code to test if the feedback function works

	Wordle word;

	array<Feedback, 5> feedback = word.getFeedback("saree");

	for (Feedback letter : feedback) {
		// 0 -> correct
		// 1 -> elsewhere
		// 2 -> wrong
		cout << letter;
	}
	return 0;
}