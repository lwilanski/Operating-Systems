#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        exit(1);
    }

    int n_threads = atoi(argv[1]);
    if (n_threads <= 0)
    {
        fprintf(stderr, "Number of threads must be greater than 0\n");
        exit(1);
    }

    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode

    char *foreground = create_grid();
    char *background = create_grid();
    char *tmp;

    init_grid(foreground);

    while (true)
    {
        draw_grid(foreground);
        usleep(500 * 1000);

        // Step simulation
        update_grid(foreground, background, n_threads);
        tmp = foreground;
        foreground = background;
        background = tmp;
    }

    endwin(); // End curses mode
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}
