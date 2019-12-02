#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System& system, int n = 10);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
void DisplayMenu(WINDOW* window, std::size_t &sort_highlited, std::size_t &sort_processes_by);
std::string ProgressBar(float percent);
void GroupProgressBar(WINDOW* system_window, int row, std::vector<float> percentages);
std::string ShowBars(float percent);
};  // namespace NCursesDisplay

#endif