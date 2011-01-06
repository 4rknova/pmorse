#include <map>
#include <string>

std::map<const char, std::string> morsecode;

void init()
{
	/* Numericals */
	morsecode['0'] = "-----";
	morsecode['1'] = ".----";
	morsecode['2'] = "..---";
	morsecode['3'] = "...--";
	morsecode['4'] = "....-";
	morsecode['5'] = ".....";
	morsecode['6'] = "-....";
	morsecode['7'] = "--...";
	morsecode['8'] = "---..";
	morsecode['9'] = "----.";
	
	/* English alphabet */
	morsecode['A'] = ".-";
	morsecode['B'] = "-...";
	morsecode['C'] = "-.-.";
	morsecode['D'] = "-..";
	morsecode['E'] = ".";
	morsecode['F'] = "..-.";
	morsecode['G'] = "--.";
	morsecode['H'] = "....";
	morsecode['I'] = "..";
	morsecode['J'] = ".---";
	morsecode['K'] = "-.-";
	morsecode['L'] = ".-..";
	morsecode['M'] = "--";
	morsecode['N'] = "-.";
	morsecode['O'] = "---";
	morsecode['P'] = ".--.";
	morsecode['Q'] = "--.-";
	morsecode['R'] = ".-.";
	morsecode['S'] = "...";
	morsecode['T'] = "-";
	morsecode['U'] = "..-";
	morsecode['V'] = "...-";
	morsecode['W'] = ".--";
	morsecode['X'] = "-..-";
	morsecode['Y'] = "-.--";
	morsecode['Z'] = "--..";

	/* Punctuation */
	morsecode['.'] = ".-.-.-";
	morsecode[','] = "..-..";
	morsecode[':'] = "---...";
	morsecode[';'] = "-.-.-";
	morsecode['?'] = "..--..";
	morsecode['/'] = "--.-.";
	morsecode['-'] = "-....-";
	morsecode['/'] = ".----.";
	morsecode['('] = "-.--.-";
	morsecode[')'] = "-.--.-";
	morsecode['_'] = "..--.-";
}

#include <cstring>
#include <cstdio>


unsigned int mode_interactive = 0;
unsigned int mode_use_bell = 0;
unsigned int mode_use_pipe = 0;
unsigned int mode_no_sound = 0;
unsigned int mode_no_script = 0;
unsigned int mode_freq = 80000;

int parsecl(int argc, char **argv, std::string& msg)
{
	if( argc == 2 && ((!strcmp(argv[1], "-version")) || (!strcmp(argv[1], "-v")) || (!strcmp(argv[1], "-ver"))))
	{
		printf("PMorse v0.9\nby Papadopoulos Nikos 2010\nusage: %s [option]... message\n", argv[0]);
		return 1;
	}

	for (int i = 1; i<argc; i++)
	{
		if (!strcmp(argv[i], "-i"))
	  	{
			if (mode_use_pipe)
			{
				fprintf(stderr,"Options -i and -p are incompatible\n");
				return -1;
			}
			mode_interactive = 1;
		}
		else if(!strcmp(argv[i], "-b"))
		{
			mode_use_bell = 1;
		}
		else if(!strcmp(argv[i], "-p"))
		{
			if (mode_interactive)
			{
				fprintf(stderr,"Options -i and -p are incompatible\n");
				return -1;
			}
			mode_use_pipe = 1;
		}
		else if(!strcmp(argv[i], "-noscript"))
		{
			mode_no_script = 1;
		}
		else if(!strcmp(argv[i], "-nosound"))
		{
			mode_no_sound = 1;
		}
		else if(!strcmp(argv[i], "-f"))
		{
			i++;
			if(!argv[i])
			{
				fprintf(stderr, "No value was provided for %s.\n", argv[i-1]);
			}
			int v = 0;
			if ((argv[i][0] == '-') || sscanf(argv[i], "%d", &v) < 1)
			{
				fprintf(stderr, "Invalid %s value.\n", argv[i-1]);
				return -1;
			}
			mode_freq = v;
		}
		else if (argv[i][0] == '-')
		{
			fprintf(stderr, "Invalid option '%s'.\n", argv[i]);
			return -1;
		}
		else
		{
			msg += " ";
			msg += argv[i];
		}

	}

	if(msg.empty() && !mode_interactive && !mode_use_pipe)
	{
		printf("No message was given.\n");
	}

	return 0;
}

#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>

#define CLOCK_TICK_RATE 1193180

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
	{
		return -1;
	}
	
	int f = freq > 0 ? CLOCK_TICK_RATE/(int)freq : 0;

	ioctl(console_fd, KIOCSOUND, f);

	close(console_fd);
	return 0;
}

void getmsg(std::string &str)
{
	std::string temp;
	char c = 0;
	while (c!='\n')
	{
		scanf("%c", &c);
		temp+=c;
		fflush(stdin);
	}
	str = temp;
}

#include <signal.h>

void terminate(int param)
{
	if(!mode_no_sound)
	{
		beep(0);
	}
	printf("\n");
	signal(SIGINT, SIG_DFL);
}

void play(std::string msg)
{
	if(msg.empty())
	{
		return;
	}
	std::transform(msg.begin(), msg.end(), msg.begin(), ::toupper);

	for (unsigned int i=0; i < msg.length(); i++)
 	{
    	usleep((msg[i] == ' ' || msg[i] == '\t')? mode_freq * 7 : mode_freq * 3);
		for(unsigned int j=0; j < morsecode[msg[i]].length(); j++)
		{
			if(!mode_no_script)
			{
				printf("%c", morsecode[msg[i]][j]);
			}
			
			switch (mode_use_bell)
			{
				case 0:
					if(!mode_no_sound)
					{
						beep(500);
					}
					break;
				default: 
					bell();
			}

			switch (morsecode[msg[i]][j]) 
			{
				case '-': usleep(mode_freq * 2); /* Pauses for 3 * mode_freq */
				case '.': usleep(mode_freq);	
			}
		
			fflush(stdout);

			if(!mode_no_sound)
			{
				beep(0);
			}

			usleep(mode_freq);
		}
	}
	printf("\n");
}

int done = 0;

void procom(std::string &proc)
{
	std::string msg = proc;
	proc.clear();

	if (!msg.compare(0, 2, ":q"))
	{
		printf("Shutting down..\n");
		done++;
		mode_interactive = 0;
		mode_use_pipe = 0;
	}
	else if (!msg.compare(0, 5, ":bell"))
	{
		if(!msg.compare(6, 3, "off"))
		{	
			printf("Bell is now off\n");
			mode_use_bell=1;
		}
		else
		{
			printf("Bell is now on\n");
			mode_use_bell=0;
		}
	}
	else if (!msg.compare(0, 7, ":script"))
	{
		if(!msg.compare(8, 3, "off"))
		{	
			printf("Script is now on\n");
			mode_no_script=1;
		}
		else
		{
			printf("Script is now off\n");
			mode_no_script=0;
		}
	}
	else if (!msg.compare(0, 6, ":sound"))
	{
		if(!msg.compare(7, 3, "off"))
		{
			printf("Sound is now off\n");
			mode_no_sound=1;
		}
		else
		{
			printf("Sound is now on\n");
			mode_no_sound=0;
		}
	}
	else if (!msg.compare(0, 5, ":freq"))
	{
		std::string num = msg.substr(msg.find_last_of(' '));
		mode_freq = atoi(num.c_str());	
	}
	else
	{
		proc = msg;
	}
}

int main(int argc, char **argv)
{
	std::string msg;
	if (parsecl(argc, argv, msg))
	{
		return -1;
	}

	signal(SIGINT, &terminate);
	init();
	
	while (!done)
	{
		procom(msg);
		
		if(!msg.empty())
		{
			play(msg);
		}

		if (mode_interactive)
		{
			/* Receive new input */
			printf("pmorse > ");
			getmsg(msg);
		}
		else if (mode_use_pipe)
		{
			char line[1000];
			read(STDIN_FILENO, line, 999);
			msg = line;
			printf("Message: %s\n", msg.c_str());
		}
		else
		{
			done++;
		}
	}
	return 0;
}
