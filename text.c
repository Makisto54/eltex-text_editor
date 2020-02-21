#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

void file_open(char * filename)
{
    int handle;
    move(0, 0);
    int bytes;
    if((handle = open(filename, O_RDONLY)) == -1)
    {
        printf("OPEN FILE ERROR!.\n");
        getch();
        endwin();
        exit(1);
    }
    int pos = (int)lseek(handle, 0, SEEK_END);
    char buf[pos];
    lseek(handle, 0, SEEK_SET);  
    if((bytes = read(handle, buf, sizeof(buf))) == -1)
    {
        printf("READ FILE ERROR!.\n");
        endwin();
        exit(1);
    }
    for(int i = 0; i < pos; i++)
    {
        printw("%c", buf[i]);
    }
    close(handle);
}

void file_save(char * filename)
{
    FILE* fd = fopen(filename, "w");
    for (int l = 0; l < LINES - 1; l++) 
    {
        int n = len(n);
        for(int i = 0; i < n; i++)
        {
            putc(mvinch(l, i) & A_CHARTEXT, fd);
        }
        //putc('\n', fd);
    }
    move(0, 0);
    fclose(fd);
}

int len(int lineno)
{
    int linelen = COLS - 1;
    while (linelen >= 0 && mvinch(lineno, linelen) == ' ')
    linelen--;
    return linelen + 1;
}

int main(int argc, char **argv)
{
    int x1, y1, ch;
    int handle;
    char name[15 + 1];
    initscr();
    signal(SIGWINCH, sig_winch);
    curs_set(1);
    keypad(stdscr, TRUE);
    mousemask(BUTTON1_CLICKED, NULL);
    MEVENT event;
    while(ch != KEY_F(3))
    {
        ch = wgetch(stdscr);
        getyx(stdscr, x1, y1);
        switch(ch)
        {
            case KEY_UP:
                move(x1 - 1, y1);
                break;
            case KEY_DOWN:
                move(x1 + 1, y1);
                break;
            case KEY_LEFT:
                move(x1, y1 - 1);
                break;
            case KEY_RIGHT:
                move(x1, y1 + 1);
                break;
            case KEY_F(1):
                printw("Input filename\n");
                wgetnstr(stdscr, name, 15);
                file_open(name);
                break; 
            case KEY_F(2):
                file_save(name);
                break; 
            case KEY_BACKSPACE:
                getyx(stdscr, x1, y1);
                mvwaddch(stdscr, x1, y1, ' ');
                refresh();
                move(x1, y1);
                break; 
            case KEY_MOUSE:
                getmouse(&event);
                move(event.y, event.x);  
                break;       
        }
        refresh();
    }
    endwin();
    exit(EXIT_SUCCESS);
}
