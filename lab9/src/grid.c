#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>

const int grid_width = 30;
const int grid_height = 30;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{
    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void *thread_update(void *arg)
{
    struct thread_data *data = (struct thread_data *)arg;
    for (int i = data->start_row; i < data->end_row; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            data->dst[i * grid_width + j] = is_alive(i, j, data->src);
        }
    }
    pthread_exit(NULL);
}

void update_grid(char *src, char *dst, int n_threads)
{
    pthread_t threads[n_threads];
    struct thread_data thread_args[n_threads];

    int rows_per_thread = grid_height / n_threads;
    int remaining_rows = grid_height % n_threads; // Pozostałe wiersze

    for (int i = 0; i < n_threads; ++i)
    {
        thread_args[i].src = src;
        thread_args[i].dst = dst;
        thread_args[i].start_row = i * rows_per_thread;
        thread_args[i].end_row = (i + 1) * rows_per_thread;

        if (i == n_threads - 1)
        {
            thread_args[i].end_row += remaining_rows; // Ostatni wątek obsługuje pozostałe wiersze
        }

        pthread_create(&threads[i], NULL, thread_update, (void *)&thread_args[i]);
    }

    for (int i = 0; i < n_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }
}
