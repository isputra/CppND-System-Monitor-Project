#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "ncurses_display.h"
#include "system.h"

using std::string;
using std::to_string;

void NCursesDisplay::SortProcesses(System& system, int& sort_chosen) {

}
// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
std::string NCursesDisplay::ProgressBar(float percent) {
  std::string result{"0%"};
  int size{50};
  float bars{percent * size};

  for (int i{0}; i < size; ++i) {
    result += i <= bars ? '|' : ' ';
  }

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

// Show bars based on its group
std::string NCursesDisplay::ShowBars(float percent) {
    std::string result{};
    int size{50};
    float bars{percent * size};

    for (int i{0}; i < bars; ++i) {
        result += '|';
    }

    return result;
}

// Display Progress bar as groups
void NCursesDisplay::GroupProgressBar(WINDOW* system_window, int row, std::vector<float> percentages) {
    int idx_color_pair = 2; // color pair that'll be assigned to each group of bar
    int start_column = 12; // 10 + length of string(0%)
    float total_percent = 0.0;

    wattron(system_window, COLOR_PAIR(1));
    mvwprintw(system_window, row, 10, "0%%");
    wattroff(system_window, COLOR_PAIR(1));
    for (auto &percent: percentages) {
        wattron(system_window, COLOR_PAIR(++idx_color_pair));
        mvwprintw(system_window, row, start_column, "");
        wprintw(system_window, ShowBars(percent).c_str());
        wattroff(system_window, COLOR_PAIR(idx_color_pair));
        float bars{percent * 50};
        start_column += bars;
        total_percent += percent;
    }
    string display{to_string(total_percent * 100).substr(0, 4)};
    if (total_percent < 0.1 || total_percent == 1.0)
        display = " " + to_string(total_percent * 100).substr(0, 3);
    string label{" " + display + "/100%%"};
    wattron(system_window, COLOR_PAIR(1));
    mvwprintw(system_window, row, 62, label.c_str());
    wattroff(system_window, COLOR_PAIR(1));
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  mvwprintw(window, ++row, 2, "CPU: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.Cpu().Utilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2, "Memory: ");
  GroupProgressBar(window, row, system.VectorMemoryUtilization());
  mvwprintw(window, ++row, 2,
            ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(
      window, ++row, 2,
      ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
  mvwprintw(window, ++row, 2,
            ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,
                                      WINDOW* window, int n) {
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{16};
  int const ram_column{26};
  int const time_column{35};
  int const command_column{46};
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));
  for (int i = 0; i < n; ++i) {
    mvwprintw(window, ++row, pid_column, to_string(processes[i].Pid()).c_str());
    mvwprintw(window, row, user_column, processes[i].User().c_str());
    float cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 4).c_str());
    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, window->_maxx - 46).c_str());
  }
}

void NCursesDisplay::DisplayMenu(System& system, WINDOW* window, int &sort_highlited) {
  int i = 0, key_input;
  std::vector<std::string> sorting_list{"PID", "User", "CPU", "Ram", "Time"};
  mvwprintw(window, 1, 1, "Sort by:");
  for (std::string& sorting : sorting_list) {
    if (i == sort_highlited) {
      wattron(window, A_REVERSE);
      mvwprintw(window, 1, 10+(5*i), sorting.c_str());
      wattroff(window, A_REVERSE);
    } else {
      mvwprintw(window, 1, 10+(5*i), sorting.c_str());
    }
    i++;
  }
  key_input = wgetch(window);
  
  switch(key_input) {
    case KEY_LEFT:
      if (sort_highlited > 0) sort_highlited--;
      break;
    case KEY_RIGHT:
      if (sort_highlited < sorting_list.size()) sort_highlited++;
      break;
    case 10:
      SortProcesses(system, sort_highlited);
      break;
    default:
      break;
  }
}

void NCursesDisplay::Display(System& system, int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color

  int x_max{getmaxx(stdscr)};
  int sort_highlited = 0;
  WINDOW* system_window = newwin(9, x_max - 1, 0, 0);
  WINDOW* process_window =
      newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);
  WINDOW* menu_window = newwin(3, x_max - 1, system_window->_maxy + process_window->_maxy + 2, 0);
  
  keypad(menu_window, true);
  nodelay(menu_window, true);

  while (1) {
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    init_pair(6, COLOR_RED, COLOR_BLACK);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    box(menu_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), process_window, n);
    DisplayMenu(system, menu_window, sort_highlited);
    wrefresh(system_window);
    wrefresh(process_window);
    wrefresh(menu_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  endwin();
}