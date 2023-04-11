#include <algorithm>
#include <iostream>
#include <regex>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, unsigned> capital_letters;
std::unordered_map<std::string, std::unordered_map<std::string, unsigned>>
word_transitions, capital_word_transitions;

std::string
weighted_random_choice(const std::unordered_map<std::string, unsigned>& map) {
	unsigned total_weight = 0;

	for (const auto& pair : map) {
		total_weight += pair.second;
	}

	const unsigned random_weight = rand() % total_weight;
	unsigned current_weight = 0;
	for (const auto& pair : map) {
		current_weight += pair.second;

		if (current_weight > random_weight) {
			return pair.first;
		}
	}

	return map.begin()->first;
}

void delete_symbols(std::string& word) {
	static const std::regex symbols("[\"();]");
	word = std::regex_replace(word, symbols, "");
}

void generate_sentence(const unsigned number_of_sentences) {

	for (int index = 0; index < number_of_sentences; ++index) {
		const std::string random_key = weighted_random_choice(capital_letters);
		if (random_key.back() == '.' || random_key.back() == '!' ||
			random_key.back() == '?') {
			std::cout << index + 1 << ": " << random_key << '\n';
			continue;
		}
		std::string random_value =
			weighted_random_choice(capital_word_transitions[random_key]);

		std::cout << index + 1 << ": " << random_key << ' ' << random_value;
		while (true) {
			if (random_value.back() == '.' || random_value.back() == '?' ||
				random_value.back() == '!') {
				break;
			}
			random_value = weighted_random_choice(word_transitions[random_value]);
			std::cout << ' ' << random_value;
		}
		std::cout << '\n';
	}
}

void process_input() {
	std::string previous_word, current_word;
	std::cin >> previous_word;
	bool check = true;
	while (std::cin >> current_word) {
		bool last_symbol_check = previous_word.back() != '.' &&
			previous_word.back() != '?' &&
			previous_word.back() != '!';
		delete_symbols(current_word);
		if (check) {
			++capital_letters[previous_word];

			if (last_symbol_check) {

				++capital_word_transitions[previous_word][current_word];
				check = false;
			}
		}
		else if (last_symbol_check) {
			++word_transitions[previous_word][current_word];
		}
		else {
			check = true;
		}
		previous_word = current_word;
	}
}

int main() {

	srand(time(NULL));

	process_input();

	generate_sentence(100);

	return 0;
}