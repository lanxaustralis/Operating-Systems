/*
 * shelldon interface program

KUSIS ID: 54339 PARTNER NAME: Yüşa Ömer Altıntop
KUSIS ID: 61263 PARTNER NAME: Dursun Bekçi

 */

#include < stdio.h > #include < unistd.h > #include < errno.h > #include < stdlib.h > #include < sys / types.h > #include < string.h >
    // included libraries
    #include < sys / wait.h > #include < fcntl.h > #include < dirent.h > #include < linux / sched.h > #include < sys / stat.h > #include < sys / types.h >

    #define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */ # define HISTORY_SIZE 10# define SENTINEL " "#
define PATH "/bin/"#
define HIST "history.txt"#
define TEMP "tmp.txt"

int parseCommand(char inputBuffer[], char * args[], int * background);
int foundIO(char * args[]);
int foundAmper(char * args[]);
void showHistory(char * history[]);
void updateHistory(char * history[], char * newcomer[]);
void printLine(char * line, int order);
int findLen(char * arr[]);
void getHistory(char * args[], char * line);
int codeSearch(char * str, char * fname);
void trimLeading(char * str);
void traverseDir(const char * name, char * args[]);
void writeToHist(char * args[]);
void readFromHist(char inputBuffer[], int det, char * args[]);
void tokenize(char argBuff[], char * line, char * args[]);
int min(int a, int b);
void regret(int time);
void apocalypse(int courage);

int
main(void) {
    char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
    int background; /* equals 1 if a command is followed by '&' */
    char * args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
    pid_t child; /* process id of the child process */
    int status; /* result from execv system call */
    int shouldrun = 1;
    char histBuffer[MAX_LINE];

    int i, upper, limit, IOStatus, position;
    // Create/refresh the history files
    FILE * file;
    file = fopen(HIST, "w");
    fclose(file);
    file = fopen(TEMP, "w");
    fclose(file);

    while (shouldrun) { /* Program terminates normally inside setup */
        background = 0;

        shouldrun = parseCommand(inputBuffer, args, & background); /* get next command */
        limit = sizeof(args) / sizeof(args[0]);
        position = limit - 1;

        if (strncmp(inputBuffer, "exit", 4) == 0)
            shouldrun = 0; /* Exiting from shelldon */

        if (shouldrun) {
            /*
               After reading user input, the steps are 
               (1) Fork a child process using fork()
               (2) the child process will invoke execv()
               (3) if command included &, parent will invoke wait()
             */
            background = foundAmper(args);
            if (strcmp(args[0], "cd") == 0) {
                if (args[1] == NULL)
                    printf("User must enter at least one valid directory\n");
                else if (chdir(args[1]) == -1) // Shall directory ~ work as expected?
                    printf("%s is not a valid directory\n", args[1]);

            } else {

                child = fork();
                if (child < 0)
                    perror("Illegal PID Number");
                else if (child == 0) { // Child Case        

                    int IOCode = foundIO(args);
                    IOStatus = IOCode % 10;

                    // IO Case
                    int io_check = 0;
                    if (IOStatus > 0) {
                        io_check = 1;
                        position = IOCode / 10;
                        char * fileName = args[position + 1];

                        limit = position - IOStatus + 1; // meaningful limit
                        int file_desc = 0;
                        switch (IOStatus) {
                        case 1:
                            file_desc =
                                open(fileName, O_WRONLY | O_CREAT | O_TRUNC,
                                    S_IRWXU);
                            dup2(file_desc, 1);
                            close(file_desc);
                            break;
                        case 2:
                            file_desc =
                                open(fileName, O_WRONLY | O_APPEND | O_CREAT,
                                    S_IRWXU);
                            dup2(file_desc, 1);
                            close(file_desc);
                            break;
                        default:
                            perror("Illegal Argument");
                        }
                    }
                    // History Case

                    /*char *lineHist;
                    lineHist = (char *) malloc (MAX_LINE * sizeof (char));*/
                    int history_check = 0;
                    if (strcmp(args[0], "history") == 0) {
                        history_check = 1;
                        // Case 1: Only display the history
                        if (args[1] == NULL) {
                            readFromHist(histBuffer, 0, NULL);
                            continue;
                        }
                        if (strcmp(args[1], "!") == 0) //take next commands
                        {
                            if (args[2] != NULL)
                                readFromHist(histBuffer, atoi(args[2]), args);

                            else
                                printf("The ! command must be followed by a valid integer between 1 and 10.\n");
                        } else if (strcmp(args[1], "!!") == 0)
                            readFromHist(histBuffer, 11, args);
                        else
                            printf("The history command must be immediately followed by either ! or !!.");
                    }

                    // Code Search
                    if (strcmp(inputBuffer, "codesearch") == 0) {
                        if (strncmp(args[1], "-r", 2) == 0)
                            traverseDir(".", args);
                        else if (args[2] == NULL) {
                            DIR * d;
                            struct dirent * dir;
                            d = opendir(".");
                            if (d) {
                                while ((dir = readdir(d)) != NULL) {
                                    codeSearch(strtok(args[1], "\""), dir - > d_name);
                                }
                                closedir(d);
                            }
                        } else if ((strncmp(args[2], "-f", 2) == 0) & args[3] != NULL) {
                            codeSearch(strtok(args[1], "\""), args[3]);

                        }
                        continue;

                    }

                    if (strcmp(inputBuffer, "regret") == 0) {
                        if (args[1] != NULL)
                            regret(atoi(args[1]));
                        else
                            printf("The function regret must have one integer argument\n");
                        continue;
                    }

                    if (strcmp(inputBuffer, "apocalypse") == 0) {
                        if (args[1] != NULL)
                            apocalypse(atoi(args[1]));
                        else
                            printf("The function apocalypse must have one integer argument\nWithout giving a constant time to apocalypse, it would be boring\n");
                        continue;
                    }

                    // path creation
                    char fullPath[50];
                    strcpy(fullPath, PATH);
                    // io handling
                    if (io_check) {
                        char ** updatedArgs; // for truncated IO commands (before >)
                        updatedArgs = (char ** ) malloc(limit * sizeof(args[0]));
                        memcpy(updatedArgs, & args[0], limit * sizeof(args[0]));

                        strcat(fullPath, updatedArgs[0]);

                        if (!history_check) writeToHist(updatedArgs);
                        status = execv(fullPath, updatedArgs); // executing
                        perror("execv");
                        return 0;
                    }

                    if (!history_check)
                        writeToHist(args);

                    strcat(fullPath, args[0]);
                    status = execv(fullPath, args); // executing
                    perror("execv");

                } else if (background)
                    wait(NULL); // Parent Case
            }
        }
    }
    return 0;
}

/** 
 * The parseCommand function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */

int
parseCommand(char inputBuffer[], char * args[], int * background) {
    int length, /* # of characters in the command line */
    i, /* loop index for accessing inputBuffer array */
    start, /* index where beginning of next command parameter is */
    ct, /* index of where to place the next parameter into args[] */
    command_number; /* index of requested command number */

    ct = 0;

    /* read what the user enters on the command line */
    do {
        printf("shelldon>");
        fflush(stdout);
        length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
    }
    while (inputBuffer[0] == '\n'); /* swallow newline characters */

    /**
     *  0 is the system predefined file descriptor for stdin (standard input),
     *  which is the user's screen in this case. inputBuffer by itself is the
     *  same as &inputBuffer[0], i.e. the starting address of where to store
     *  the command that is read, and length holds the number of characters
     *  read in. inputBuffer is not a null terminated C-string. 
     */
    start = -1;
    if (length == 0)
        exit(0); /* ^d was entered, end of user command stream */

    /** 
     * the <control><d> signal interrupted the read system call 
     * if the process is in the read() system call, read returns -1
     * However, if this occurs, errno is set to EINTR. We can check this  value
     * and disregard the -1 value 
     */

    if ((length < 0) && (errno != EINTR)) {
        perror("error reading the command");
        exit(-1); /* terminate with error code of -1 */
    }

    /**
     * Parse the contents of inputBuffer
     */

    for (i = 0; i < length; i++) {
        /* examine every character in the inputBuffer */

        switch (inputBuffer[i]) {
        case ' ':
        case '\t':
            /* argument separators */
            if (start != -1) {
                args[ct] = & inputBuffer[start]; /* set up pointer */
                ct++;
            }
            inputBuffer[i] = '\0'; /* add a null char; make a C string */
            start = -1;
            break;

        case '\n':
            /* should be the final char examined */
            if (start != -1) {
                args[ct] = & inputBuffer[start];
                ct++;
            }
            inputBuffer[i] = '\0';
            args[ct] = NULL; /* no more arguments to this command */
            break;

        default:
            /* some other character */
            if (start == -1)
                start = i;
            if (inputBuffer[i] == '&') {
                * background = 1;
                inputBuffer[i - 1] = '\0';
            }
        } /* end of switch */
    } /* end of for */

    /**
     * If we get &, don't enter it in the args array
     */

    if ( * background)
        args[--ct] = NULL;

    args[ct] = NULL; /* just in case the input line was > 80 */

    return 1;

} /* end of parseCommand routine */

int
foundIO(char * args[]) {
    int ct = 0;
    while (args[ct] != NULL) {
        if (strcmp(args[ct], ">") == 0)
            return 1 + ct * 10; // Case 1: >
        if (strcmp(args[ct], ">>") == 0)
            return 2 + ct * 10; // Case 2: >>
        ct++;
    }
    return 0; // Case 0: no >
}

int
foundAmper(char * args[]) {
    int ct = 0;
    while (args[ct] != NULL) {
        if (strcmp(args[ct], "&") == 0)
            return 1;
        ct++;
    }
    return 0;
}

void writeToHist(char * args[]) {
    FILE * file = fopen(HIST, "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    int argNum = findLen(args);
    for (int i = 0; i < argNum; i++) {
        fprintf(file, "%s ", args[i]);
    }
    fprintf(file, "\n");

    fclose(file);
}

void readFromHist(char argBuff[], int det, char * args[]) { // det 0: read all det 1-10: read that line 
    FILE * hist = fopen(HIST, "r");
    FILE * temp;
    char str[MAX_LINE];

    if (hist == NULL) {
        printf("Error opening history file!\n");
        exit(1);
    }

    int line_num = 0;
    while (fgets(str, MAX_LINE, hist) != NULL) {
        line_num++;
    }

    if (line_num != 10 && det > line_num) {
        printf("The second argument cannot be higher than total history size of %d\n", line_num);
        exit(-1);
    }
    fclose(hist);
    hist = fopen(HIST, "r");
    if (line_num > HISTORY_SIZE) {
        // Transfer Part (Hist->Temp)
        temp = fopen(TEMP, "w");
        if (temp == NULL) {
            printf("Error opening temp file!\n");
            exit(1);
        }
        int current_index = 0;

        while (fgets(str, MAX_LINE, hist) != NULL) {
            current_index++;
            if (current_index >= line_num - HISTORY_SIZE + 1)
                fprintf(temp, "%s", str);

        }

        fclose(temp);

        // Read Part
        temp = fopen(TEMP, "r");
        if (temp == NULL) {
            printf("Error opening temp file!\n");
            exit(1);
        }
        if (det == 0) {

            int tmp_order = 1;
            printf("\n");
            while (fgets(str, MAX_LINE, temp) != NULL) {
                printf("%d %s", tmp_order, str);
                tmp_order++;
            }

        } else if (det > 11) printf("%d is not a valid history index, it must be between 1 and 10, inclusively.\n", det);
        else if (det == 11) {
            readFromHist(argBuff, min(line_num, 10), args);
            printf("compare %d\n", min(line_num, 10));
        } else {
            int index = 0;
            while (fgets(str, MAX_LINE, temp) != NULL) {
                index++;
                if (index == det) tokenize(argBuff, str, args);
            }
        }

        fclose(temp);
    } else {

        if (det == 0) {
            int hist_order = 1;
            printf("\n");
            while (fgets(str, MAX_LINE, hist) != NULL) {
                printf("%d %s", hist_order, str);
                hist_order++;
            }
        } else if (det > 11) printf("%d is not a valid history index, it must be between 1 and 10, inclusively.\n", det);
        else if (det == 11) {
            readFromHist(argBuff, min(line_num, 10), args);
            printf("compare %d\n", min(line_num, 10));
        } else {
            int index = 0;
            while (fgets(str, MAX_LINE, hist) != NULL) {
                index++;
                if (index == det) tokenize(argBuff, str, args);
            }
        }

    }
    fclose(hist);
}

int findLen(char * arr[]) {
    int i = 0;
    while (arr[i] != NULL) i++;
    return i;
}

void tokenize(char argBuff[], char * line, char * args[]) {
    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
        args[i] = NULL;

    int len = strlen(line);
    if (len == 0) {
        printf("The empty string cannot be tokenized.\n");
        exit(-1);
    }
    int argNum = 0;
    int buff_index = 0;
    for (int i = 0; i < len; i++) {
        if (line[i] == ' ') {
            argBuff[i] = '\0';
            args[argNum] = & argBuff[buff_index];
            argNum++;
            buff_index = i + 1;
        } else {
            argBuff[i] = line[i];
        }
        argBuff[len] = '\0';
    }
}
// Part 3
int codeSearch(char * str, char * fname) {
    FILE * fp;
    int line_num = 1;
    int find_result = 0;
    char temp[512];

    if ((fp = fopen(fname, "r")) == NULL) {
        return (-1);
    }

    while (fgets(temp, 512, fp) != NULL) {
        if ((strstr(temp, str)) != NULL) {
            trimLeading(temp);
            printf("%d: %s -> %s", line_num, fname, temp);
            find_result++;
        }
        line_num++;
    }

    if (find_result == 0) {}

    //Close the file if still open.
    if (fp) {
        fclose(fp);
    }
    return (0);
}

void trimLeading(char * s) {
    int point, a;
    point = 0;
    while (s[point] == '\t' || s[point] == ' ' || s[point] == '\n') {
        point++;
    }
    if (point != 0) {
        a = 0;
        while (s[a + point] != '\0') {
            s[a] = s[a + point];
            a++;
        }
        s[a] = '\0';
    }
}

void traverseDir(const char * name, char * args[]) {
    DIR * dir;
    struct dirent * entry;
    char * key = strtok(args[2], "\"");

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        char * tmp;
        if (entry - > d_type == DT_DIR) {
            if (strcmp(entry - > d_name, ".") == 0 || strcmp(entry - > d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry - > d_name);
            traverseDir(path, args);
        } else {
            snprintf(path, sizeof(path), "%s/%s", name, entry - > d_name);
            codeSearch(key, path);
        }
    }
    closedir(dir);
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

void regret(int time) {
    FILE * file = fopen("shelldon.c", "w");
    int cooefficient_of_regret = time * time;

    if (file == NULL) {
        printf("Weird, how come! It is the source code\n");
        exit(1);
    }

    for (int i = 0; i < cooefficient_of_regret; i++) {
        fprintf(file, "We will start the next project as soon as possible. The number %d represents our increasing regret.\n", i);
    }
    fprintf(file, "************By the way there is a very dangerous command which is called -apocalypse- that you would not want to give a try. If you really wonder how it works, be prepare for the apocalypse (laughs in binary)*********\n");
    fclose(file);
}

void apocalypse(int courage) {

    char str[15];
    char ints[10];
    strcpy(str, "");
    char dir[256];

    for (int i = 0; i < courage; i++) {
        strcpy(str, ".apocalypse");
        getcwd(dir, sizeof(dir));
        sprintf(ints, "%d", i);
        strcat(str, ints);
        puts(str);
        mkdir(str, 0755);
        strcat(dir, "/");
        chdir(strcat(dir, str));
    }
    FILE * file = fopen(".congrats.null", "w");
    fprintf(file, "The Great Emu War is the only war that animals beat humans in a conflict that they are not the attacker, circa 1932.\n");
    fclose(file);
}
