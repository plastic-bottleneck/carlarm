/*
 * This Software falls under the 'Clear BSD3' License.
 * (c)2025 Plastic Bottleneck
 * https://plastic-bottleneck.github.io
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>

const char *digits[10][7] = {
    {
        "  .----.   ",
        " /  ..  \\  ",
        ".  /  \\  . ",
        "|  |  '  | ",
        "'  \\  /  ' ",
        " \\  `'  /  ",
        "  `---''   "
    },
    {
        "   .---.   ",
        "  /_   |   ",
        "   |   |   ",
        "   |   |   ",
        "   |   |   ",
        "   |   |   ",
        "   `---'   "
    },
    {
        "  .-----.  ",
        " / ,-.   \\ ", 
        " '-'  |  | ", 
        "    .'  /  ", 
        "  .'  /__  ",
        " |       | ", 
        " `-------' " 
    },
    {
        "  .-----.  ", 
        " /  -.   \\ ", 
        " '-' _'  | ", 
        "    |_  <  ", 
        " .-.  |  | ", 
        " \\ `-'   / ", 
        "  `----''  " 
    },
    {
        "    .---.  ", 
        "   / .  |  ", 
        "  / /|  |  ", 
        " / / |  |_ ", 
        "/  '-'    |", 
        "`----|  |-'", 
        "     `--'  "
    },
    {
        " .------.  ", 
        " |   ___|  ", 
        " |  '--.   ", 
        " `---.  '. ", 
        " .-   |  | ", 
        " | `-'   / ", 
        "  `----''  " 
    },
    {
        "   ,--.    ", 
        "  /  .'    ", 
        " .  / -.   ", 
        " | .-.  '  ", 
        " ' \\  |  | ", 
        " \\  `'  /  ", 
        "  `----'   "
    },
    {
        ".---------.", 
        "|   __   / ", 
        "`--' .  /  ", 
        "    /  /   ", 
        "   .  /    ", 
        "  /  /     ", 
        " `--'      " 
    },
    {
        "  .-----.  ", 
        " /  .-.  \\ ", 
        "|   \\_.' / ", 
        " /  .-. '. ", 
        "|  |   |  |", 
        " \\  '-'  / ", 
        "  `----''  " 
    },
    {
        "  .----.   ", 
        " /  ,.  \\  ", 
        "|  |  \\  | ", 
        " '  `-'  ' ", 
        "  `- /  '  ", 
        "   ,'  /   ", 
        "  `---'    "
    }
};

/*Change if you want to use ASCII or UNICODE.*/
//const char symbols[] = "!@#$%^&*(){}[];:,.<>/?~";				// ASCII
const char symbols[] = "!@#$%^&*(){}[];:,.<>/?~ÆØÅæøå█▓▒░▄▀▬┼§¶±©®™✓✗✠♠♥♦♣";	// UNICODE

// Function prototypes
void center_print(const char *str, int row);
void print_number(int num);
void convert_time(int seconds, int *min, int *sec);
void print_random_chars();

// Centered output
void center_print(const char *str, int row) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int col = (w.ws_col - strlen(str)) / 2;
    printf("\033[%d;%dH%s", row, col, str);
}

// Numbers
void print_number(int num) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%d", num);

    int start_row = (w.ws_row - 7) / 2;

    for (int line = 0; line < 7; line++) {
        char full_line[256] = "";
        for (int i = 0; buffer[i]; i++) {
            int digit = buffer[i] - '0';
            strcat(full_line, digits[digit][line]);
            strcat(full_line, "  ");
        }
        full_line[strlen(full_line)-2] = '\0';
        center_print(full_line, start_row + line);
    }
    fflush(stdout);
}

// Time calculation
void convert_time(int seconds, int *min, int *sec) {
    *min = seconds / 60;
    *sec = seconds % 60;
}

// Random charachters
void print_random_chars() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    printf("\033[H");
    for (int y = 0; y < w.ws_row; y++) {
        for (int x = 0; x < w.ws_col; x++) {
            printf("\033[38;5;%dm\033[%d;%dH%c", 
                 rand() % 231 + 16,
                 y, x,
                 symbols[rand() % (sizeof(symbols)-1)]);
        }
    }
    fflush(stdout);
}

int main() {
    printf("\033[?25l");
    int count;
    printf("Countdown in seconds: ");
    scanf("%d", &count);

    if (count >= 60) {
        int min, sec;
        convert_time(count, &min, &sec);
        printf("\nSet time: %d min %d sec\n", min, sec);
        sleep(3);
    }

    for (int i = count; i >= 0; i--) {
        printf("\033[H\033[J");
        print_number(i);
        sleep(1);
    }

    printf("\033[H\033[J");
    printf("\033[?25l");

    // Underscore
    for (int i = 0; i < 6; i++) {
        printf("$ %s", (i % 2 == 0) ? "_" : " ");
        fflush(stdout);
        usleep(500000);
        printf("\r");
    }

    // Funny
    printf("$ ");
    char cmd[] = "sudo rm -rf --no-preserve-root /";
    for (size_t i = 0; i < strlen(cmd); i++) {
        putchar(cmd[i]);
        fflush(stdout);
        usleep(100000);
    }

    // Make cursor blink
    for (int i = 0; i < 4; i++) {
        printf("%s", (i % 2 == 0) ? "_" : " ");
        fflush(stdout);
        usleep(500000);
        printf("\b \b");
    }

    printf("\033[?25h\n");

    system("ffplay -nodisp -autoexit -loglevel quiet danger.mp3 &");

    // Random animation
    srand(time(NULL));
    while(1) {
        print_random_chars();
        usleep(100000);
    }

    printf("\033[?25h");
    return 0;
}
