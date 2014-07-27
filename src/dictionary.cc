#include "dictionary.h"

#include <map>

std::map<const char, const char *> g_dictionary;

void load_dictionary()
{
	/* Numericals */
	g_dictionary['0'] = "-----";
	g_dictionary['1'] = ".----";
	g_dictionary['2'] = "..---";
	g_dictionary['3'] = "...--";
	g_dictionary['4'] = "....-";
	g_dictionary['5'] = ".....";
	g_dictionary['6'] = "-....";
	g_dictionary['7'] = "--...";
	g_dictionary['8'] = "---..";
	g_dictionary['9'] = "----.";

	/* English alphabet */
	g_dictionary['A'] = ".-";
	g_dictionary['B'] = "-...";
	g_dictionary['C'] = "-.-.";
	g_dictionary['D'] = "-..";
	g_dictionary['E'] = ".";
	g_dictionary['F'] = "..-.";
	g_dictionary['G'] = "--.";
	g_dictionary['H'] = "....";
	g_dictionary['I'] = "..";
	g_dictionary['J'] = ".---";
	g_dictionary['K'] = "-.-";
	g_dictionary['L'] = ".-..";
	g_dictionary['M'] = "--";
	g_dictionary['N'] = "-.";
	g_dictionary['O'] = "---";
	g_dictionary['P'] = ".--.";
	g_dictionary['Q'] = "--.-";
	g_dictionary['R'] = ".-.";
	g_dictionary['S'] = "...";
	g_dictionary['T'] = "-";
	g_dictionary['U'] = "..-";
	g_dictionary['V'] = "...-";
	g_dictionary['W'] = ".--";
	g_dictionary['X'] = "-..-";
	g_dictionary['Y'] = "-.--";
	g_dictionary['Z'] = "--..";

	/* Punctuation */
	g_dictionary['.'] = ".-.-.-";
	g_dictionary[','] = "..-..";
	g_dictionary[':'] = "---...";
	g_dictionary[';'] = "-.-.-";
	g_dictionary['?'] = "..--..";
	g_dictionary['/'] = "--.-.";
	g_dictionary['-'] = "-....-";
	g_dictionary['/'] = ".----.";
	g_dictionary['('] = "-.--.-";
	g_dictionary[')'] = "-.--.-";
	g_dictionary['_'] = "..--.-";
}

const char* get_symbol(const char c)
{
	if (g_dictionary.find(c) != g_dictionary.end()) {
		return g_dictionary[c];
	}

	return 0;
}
