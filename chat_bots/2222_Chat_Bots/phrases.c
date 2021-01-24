#include <stdio.h>
#include "phrases.h"

void print_phrase(char* name, char* phrase) {
	printf("%s: %s\n", name, phrase);
}

_phrases phrases = {
	"Hey",
	"U r such a girl! R u afraid of rats?",
	"I want a dress. Will somebody buy it for me ?",
	"I have a supercar",
	"Of course, baby. I'm the richest boy in the world!",
	"AAAAaaa! No! No rats here, pls?"
};