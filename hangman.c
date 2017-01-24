#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// representiert Zustand eines Spiels
struct game_state
{
	// zu ratendes Wort
	char *to_guess;

	// sichtbarer Teil des Wortes
	char *visible;

	// alle schon falsch geratenen Buchstaben
	char *wrong;

	// wie weit der Galgen aufgebaut ist
	unsigned int progress;
};

// initialisiert Anfangsspielstand
struct game_state init();

// free't die von malloc allokierten Strings in 'state'
void free_state(struct game_state *state);

// gibt Spielstand aus
void print_state(struct game_state *state);

// aktualisiert Spielstand bei gegebener Eingabe
void update_state(struct game_state *state, char *input);

// gibt 1 (true) zurueck, wenn das Spiel vorbei ist
int game_over(struct game_state *state);



char *gallows[11] = {
	"             \n"
	"             \n"
	"             \n"
	"             \n"
	"             \n"
	"             \n"
	"             \n"
	"             \n"
	"=============\n",

	"             \n"
	"             \n"
	"             \n"
	"             \n"
	"             \n"
	"    _____    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n",

	"             \n"
	"      |      \n"
	"      |      \n"
	"      |      \n"
	"      |      \n"
	"    __|__    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n",

	"      _____  \n"
	"      |      \n"
	"      |      \n"
	"      |      \n"
	"      |      \n"
	"    __|__    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n",

	"      _____  \n"
	"      |/     \n"
	"      |      \n"
	"      |      \n"
	"      |      \n"
	"    __|__    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n",

	"      _____  \n"
	"      |/  O  \n"
	"      |      \n"
	"      |      \n"
	"      |      \n"
	"    __|__    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n",

	"      _____  \n"
	"      |/  O  \n"
	"      |   W  \n"
	"      |      \n"
	"      |      \n"
	"    __|__    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n",

	"      _____  \n"
	"      |/  O  \n"
	"      |  ^W  \n"
	"      |      \n"
	"      |      \n"
	"    __|__    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n",

	"      _____  \n"
	"      |/  O  \n"
	"      |  ^W^ \n"
	"      |      \n"
	"      |      \n"
	"    __|__    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n",

	"      _____  \n"
	"      |/  O  \n"
	"      |  ^W^ \n"
	"      |  /   \n"
	"      |      \n"
	"    __|__    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n",

	"      _____  \n"
	"      |/  O  \n"
	"      |  ^W^ \n"
	"      |  / \\ \n"
	"      |      \n"
	"    __|__    \n"
	"   /     \\   \n"
	"  /       \\  \n"
	"=============\n"
};



int main()
{
	struct game_state state;
	char input[1024];

	state = init();

	while (1)
	{
		print_state(&state);

		if (game_over(&state))
			break;

		fgets(input, 1024, stdin);
		update_state(&state, input);
	}

	free_state(&state);
}



void malloc_fail()
{
	fprintf(stderr, "Nicht genug Speicher!\n");
	exit(1);
}

char *get_random_word()
{
	char *word;

	word = calloc(11, sizeof(char));
	if (word == NULL)
		malloc_fail();

	strcpy(word, "hangman");

	return word;
}

struct game_state init()
{
	struct game_state state;
	size_t length;

	state.to_guess = get_random_word();
	length = strlen(state.to_guess);

	state.visible = calloc(length + 1, sizeof(char));
	if (state.visible == NULL)
		malloc_fail();
	memset(state.visible, '_', length);

	state.wrong = calloc(8, sizeof(char));
	if (state.wrong == NULL)
		malloc_fail();

	state.progress = 0;

	return state;
}

void free_state(struct game_state *state)
{
	if (state == NULL)
		return;

	free(state->to_guess);
	free(state->visible);
	free(state->wrong);
}

void print_state(struct game_state *state)
{
	if (state == NULL)
		return;

	printf("%s", gallows[state->progress]);
	printf("  Wort: %s\n", state->visible);
	printf("Falsch: %s\n", state->wrong);
}

void update_state(struct game_state *state, char *input)
{
	if (state == NULL)
		return;
	if (input == NULL)
		return;

	size_t length = strlen(input);

	// Zeilenumbruch entfernen
	if (input[length - 1] == '\n')
	{
		input[length - 1] = '\0';
		length--;
	}

	if (length == 1)
	{
		char c = tolower(input[0]);
		int fail = 1;

		length = strlen(state->to_guess);
		for (size_t i = 0; i < length; i++)
		{
			if (tolower(state->to_guess[i]) == c)
			{
				state->visible[i] = state->to_guess[i];
				fail = 0;
			}
		}

		if (fail && strchr(state->wrong, c) == NULL)
		{
			state->progress++;
			state->wrong[strlen(state->wrong)] = c;
		}
	}
	else if (length > 1)
	{
		if (strcasecmp(state->to_guess, input) == 0)
		{
			strcpy(state->visible, state->to_guess);
		}
		else
		{
			state->progress++;
		}
	}
}

int game_over(struct game_state *state)
{
	if (state == NULL)
		return 1;

	if (state->progress >= 10)
	{
		return 1;
	}
	else
	{
		return strchr(state->visible, '_') == NULL;
	}
}
