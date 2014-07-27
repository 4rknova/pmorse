#include <string>
#include <cstring>
#include <cstdio>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <signal.h>

#include "dictionary.h"

#define CLOCK_TICK_RATE 1193180
#define BUFFER_SZ       32

unsigned int mode_use_bell = 0;
unsigned int mode_use_pipe = 0;
unsigned int mode_no_sound = 0;
unsigned int mode_no_script = 0;
unsigned int mode_freq = 80000;

int parse_args(int argc, char **argv, std::string& msg)
{
	if (argc == 2
		&& ((!strcmp(argv[1], "-version"))
		 || (!strcmp(argv[1], "-v"))
		 || (!strcmp(argv[1], "-ver")))) {
		printf("PMorse v%s\nby Papadopoulos Nikos 2010, 2012\nusage: %s [option]... message\n",
			PMORSE_VERSION, argv[0]);

		return 1;
	}

	for (int i = 1; i<argc; i++) {
		if      (!strcmp(argv[i], "-b"))        mode_use_bell  = 1;
		else if (!strcmp(argv[i], "-p"))	    mode_use_pipe  = 1;
		else if (!strcmp(argv[i], "-noscript")) mode_no_script = 1;
		else if (!strcmp(argv[i], "-nosound"))  mode_no_sound  = 1;
		else if (!strcmp(argv[i], "-f")) {
			++i;
			if(!argv[i])
				fprintf(stderr, "No value was provided for %s.\n", argv[i-1]);

			int v = 0;
			if ((argv[i][0] == '-') || sscanf(argv[i], "%d", &v) < 1) {
				fprintf(stderr, "Invalid %s value.\n", argv[i-1]);
				return -1;
			}
			mode_freq = v;
		}
		else if (argv[i][0] == '-')	{
			fprintf(stderr, "Invalid option '%s'.\n", argv[i]);
			return -1;
		}
		else {
			msg += " ";
			msg += argv[i];
		}
	}

	if (msg.empty() && !mode_use_pipe) printf("No message was given.\n");

	return 0;
}

int bell()
{
	printf("\a");
	fflush(stdout);
	return 0;
}

int beep(unsigned int freq)
{
	int console_fd = -1;
	if((console_fd = open("/dev/console", O_WRONLY)) == -1)
		return -1;

	int f = freq > 0 ? CLOCK_TICK_RATE/(int)freq : 0;

	ioctl(console_fd, KIOCSOUND, f);

	close(console_fd);

	return 0;
}

void terminate(int param)
{
	if(!mode_no_sound)
		beep(0);

	printf("\n");
	signal(SIGINT, SIG_DFL);
}

void play(std::string &msg)
{
	if(msg.empty())	return;

	std::transform(msg.begin(), msg.end(), msg.begin(), ::toupper);

	for (unsigned int i = 0; i < msg.length(); ++i) {
		if (msg[i] == ' ' || msg[i] == '\t' || msg[i] == '\n') {
			usleep(mode_freq * 7);
			continue;
		}

		const char *val = get_symbol(msg[i]);

		if (!val) continue;

		std::string code = val;

		usleep(mode_freq * 3);

		for(unsigned int j = 0; j < code.length(); ++j) {
			if(!mode_no_script)	printf("%c", code[j]);

			if (mode_use_bell) {
				bell();
			}
			else {
				if(!mode_no_sound) beep(500);
			}

			switch (code[j])
			{
				case '-': usleep(mode_freq * 2);
				case '.': usleep(mode_freq);
			}

			fflush(stdout);

			if(!mode_no_sound) beep(0);

			usleep(mode_freq);
		}
	}

	printf("\n");
}

int main(int argc, char **argv)
{
	std::string msg;

	if (parse_args(argc, argv, msg)) return -1;

	load_dictionary();

	signal(SIGINT, &terminate);

	char line[BUFFER_SZ];

	while (1) {
		if (mode_use_pipe) {
			memset(line, 0, BUFFER_SZ);
			if (read(STDIN_FILENO, line, BUFFER_SZ-1) > 0) msg = line;
		}
		else break;

		play(msg);
	}

	return 0;
}
