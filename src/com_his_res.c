#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "canvas.h"
#include "com_his_res.h"
#include "display.h"

void save_history(const char *filename, History *his)
{
    const char *default_history_file = "history.txt";
    if (filename == NULL)
	filename = default_history_file;
    
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL) {
	fprintf(stderr, "error: cannot open %s.\n", filename);
	return;
    }
    // [*] 線形リスト版
    for (Command *p = his->begin ; p != NULL ; p = p->next){
	fprintf(fp, "%s", p->str);
    }
    
    fclose(fp);
}

Result interpret_command(const char *command, History *his, Canvas *c)
{
    char buf[his->bufsize];
    strcpy(buf, command);
    buf[strlen(buf) - 1] = 0; // remove the newline character at the end
    
    const char *s = strtok(buf, " ");
    if (s == NULL){ // 改行だけ入力された場合
	return UNKNOWN;
    }
    // The first token corresponds to command
    if (strcmp(s, "line") == 0) {
	int p[4] = {0}; // p[0]: x0, p[1]: y0, p[2]: x1, p[3]: x1 
	char *b[4];
	for (int i = 0 ; i < 4; i++){
	    b[i] = strtok(NULL, " ");
	    if (b[i] == NULL){
		return ERRLACKARGS;
	    }
	}
	for (int i = 0 ; i < 4 ; i++){
	    char *e;
	    long v = strtol(b[i],&e, 10);
	    if (*e != '\0'){
		return ERRNONINT;
	    }
	    p[i] = (int)v;
	}
	
	draw_line(c,p[0],p[1],p[2],p[3]);
	return LINE;
    }
    
    if (strcmp(s, "save") == 0) {
	s = strtok(NULL, " ");
	save_history(s, his);
	return SAVE;
    }
    
    if (strcmp(s, "undo") == 0) {
	reset_canvas(c);
	//[*] 線形リストの先頭からスキャンして逐次実行
	// pop_back のスキャン中にinterpret_command を絡めた感じ
	Command *p = his->begin;
	if (p == NULL){
	    return NOCOMMAND;
	}
	else{
	    Command *q = NULL; // 新たな終端を決める時に使う
	    while (p->next != NULL){ // 終端でないコマンドは実行して良い
		interpret_command(p->str, his, c);
		q = p;
		p = p->next;
	    }
	    // 1つしかないコマンドのundoではリストの先頭を変更する
	    if (q == NULL) {
		his->begin = NULL;
	    }
	    else{
		q->next = NULL;
	    }
	    free(p->str);
	    free(p);	
	    return UNDO;
	}  
    }
    
    if (strcmp(s, "quit") == 0) {
	return EXIT;
    }
    return UNKNOWN;
}


// [*] 線形リストの末尾にpush する
Command *push_command(History *his, const char *str){
    Command *c = (Command*)malloc(sizeof(Command));
    char *s = (char*)malloc(his->bufsize);
    strcpy(s, str);
    
    *c = (Command){ .str = s, .bufsize = his->bufsize, .next = NULL};
    
    Command *p = his->begin;
    
    if ( p == NULL) {
	his->begin = c;
    }
    else{
	while (p->next != NULL){
	    p = p->next;
	}
	p->next = c;
    }
    return c;
}

char *strresult(Result res){
    switch(res) {
    case EXIT:
	break;
    case SAVE:
	return "history saved";
    case LINE:
	return "1 line drawn";
    case UNDO:
	return "undo!";
    case UNKNOWN:
	return "error: unknown command";
    case ERRNONINT:
	return "Non-int value is included";
    case ERRLACKARGS:
	return "Too few arguments";
    case NOCOMMAND:
	return "No command in history";
    }
    return NULL;
}
