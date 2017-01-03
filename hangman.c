#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// gibt Speilstand aus
void print_state(struct game_state *state);

// aktualisiert Speilstand bei gegebener Eingabe
void update_state(struct game_state *state, char *input);

// gibt 1 (true) zurueck, wenn das Speil vorbei ist
int game_over(struct game_state *state);



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
	fprintf(stderr, "Not enough memory!\n");
	exit(1);
}

char *get_random_word()
{
	char *word;

	word = calloc(8, sizeof(char));
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

	state.wrong = calloc(9, sizeof(char));
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

	printf("  Wort: %s\n", state->visible);
	printf("Falsch: %s\n", state->wrong);
	printf("Galgen: %d\n", state->progress);
}
