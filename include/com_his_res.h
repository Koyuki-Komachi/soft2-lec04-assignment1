#ifndef COM_HIS_RES
#define COM_HIS_RES

typedef struct command Command;
struct command{
    char *str;
    size_t bufsize;
    Command *next;
};

typedef struct
{
    Command *begin;
    size_t bufsize; // [*] : この方が効率的ですね。一部の方から指摘ありました。
} History;

typedef enum res{ EXIT, LINE, UNDO, SAVE, UNKNOWN, ERRNONINT, ERRLACKARGS, NOCOMMAND} Result;

Result interpret_command(const char *command, History *his, Canvas *c);
void save_history(const char *filename, History *his);
Command *push_command(History *his, const char *str);
char *strresult(Result res);

#endif