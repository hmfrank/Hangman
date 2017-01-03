#include <stdio.h>

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
}
