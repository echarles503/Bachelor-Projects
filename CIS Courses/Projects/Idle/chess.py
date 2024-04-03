import sys,os
import curses
from curses.textpad import Textbox, rectangle


def draw_menu(stdscr):
    k = 0
    cursor_x = 0
    cursor_y = 0

    # Clear and refresh the screen for a blank canvas
    stdscr.clear()
    stdscr.refresh()

    # Start colors in curses
    curses.start_color()
    curses.init_pair(1, curses.COLOR_CYAN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_BLACK, curses.COLOR_WHITE)

    while (k != ord('q')):
        stdscr.clear()
        height, width = stdscr.getmaxyx()

        
        rectangle(stdscr, 2, 0, 20, 64)

        stdscr.vline(2, 3, 124, 19)
        whstr = "Width: {}, Height: {}".format(width, height)
        stdscr.addstr(0, 0, whstr, curses.color_pair(1))

        stdscr.refresh()
        k = stdscr.getch()


def main():
    curses.wrapper(draw_menu)

if __name__ == "__main__":
    main()