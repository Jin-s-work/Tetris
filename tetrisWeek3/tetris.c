#include "tetris.h"

static struct sigaction act, oact;
int B, count;

int main(){
    int exit=0;

    initscr();
    noecho();
    keypad(stdscr, TRUE);

    srand((unsigned int)time(NULL));
    createRankList();
    while(!exit){
        clear();
        switch(menu()){
        case MENU_PLAY: play(); break;
        case MENU_RANK: rank(); break;
            case MENU_REC_PLAY: recommendedPlay(); break;
        case MENU_EXIT: exit=1; break;
        default: break;
        }
    }

    endwin();
    system("clear");
    return 0;
}
// 6주차 과제 수정 //
void InitTetris(){
    int i,j;

    for(j=0;j<HEIGHT;j++)
        for(i=0;i<WIDTH;i++)
            field[j][i]=0;

    nextBlock[0]=rand()%7;
    nextBlock[1]=rand()%7;
    nextBlock[2]=rand()%7;
    
    blockRotate=0;
    blockY=-1;
    blockX=WIDTH/2-2;
    score=0;
    gameOver=0;
    timed_out=0;
    
    recommend(field, 0);
    DrawOutline();
    DrawField();
    DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
    DrawNextBlock(nextBlock);
    PrintScore(score);
}

void DrawOutline(){
    int i,j;
    /* 블럭이 떨어지는 공간의 태두리를 그린다.*/
    DrawBox(0,0,HEIGHT,WIDTH);

    /* next block을 보여주는 공간의 태두리를 그린다.*/
    move(2,WIDTH+10);
    printw("NEXT BLOCK");
    DrawBox(3,WIDTH+10,4,8);
    DrawBox(10, WIDTH+10,4,8);

    /* score를 보여주는 공간의 태두리를 그린다.*/
    move(19,WIDTH+10);
    printw("SCORE");
    DrawBox(20,WIDTH+10,1,8);
}

int GetCommand(){
    int command;
    command = wgetch(stdscr);
    switch(command){
    case KEY_UP:
        break;
    case KEY_DOWN:
        break;
    case KEY_LEFT:
        break;
    case KEY_RIGHT:
        break;
    case ' ':
        /* space key*/
        /*fall block*/
        break;
    case 'q':
    case 'Q':
        command = QUIT;
        break;
    default:
        command = NOTHING;
        break;
    }
    return command;
}

int ProcessCommand(int command){
    int ret=1;
    int drawFlag=0;
    switch(command){
    case QUIT:
        ret = QUIT;
        break;
    case KEY_UP:
        if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
            blockRotate=(blockRotate+1)%4;
        break;
    case KEY_DOWN:
        if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
            blockY++;
        break;
    case KEY_RIGHT:
        if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
            blockX++;
        break;
    case KEY_LEFT:
        if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
            blockX--;
        break;
    default:
        break;
    }
    if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
    return ret;
}

void DrawField(){
    int i,j;
    for(j=0;j<HEIGHT;j++){
        move(j+1,1);
        for(i=0;i<WIDTH;i++){
            if(field[j][i]==1){
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            }
            else printw(".");
        }
    }
}


void PrintScore(int score){
    move(21,WIDTH+11);
    printw("%8d",score);
}

//6주차 과제//
void DrawNextBlock(int *nextBlock){
    int i, j;
    for( i = 0; i < 4; i++ ){
        move(4+i,WIDTH+13);
        for( j = 0; j < 4; j++ ){
            if( block[nextBlock[1]][0][i][j] == 1 ){
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            }
            else printw(" ");
        }
    }
    for(i=0;i<4;i++){
        move(11 + i, WIDTH + 13);
        for(j=0;j<4;j++){
            if(block[nextBlock[2]][0][i][j] == 1){
                     attron(A_REVERSE);
                     printw(" ");
                     attroff(A_REVERSE);
                }
                     else printw(" ");
        }
    }
}
                     // 2번째 next블록 그리는 과정 추가//
//6주차 수정//
                     
void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
    int i,j;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++){
            if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
                
                move(i+y+1,j+x+1);
                attron(A_REVERSE);
                printw("%c",tile);
                attroff(A_REVERSE);
            }
        }

    move(HEIGHT,WIDTH+10);
}
//6주차 수정//
void DrawBox(int y,int x, int height, int width){
    int i,j;
    move(y,x);
    addch(ACS_ULCORNER);
    for(i=0;i<width;i++)
        addch(ACS_HLINE);
    addch(ACS_URCORNER);
    for(j=0;j<height;j++){
        move(y+j+1,x);
        addch(ACS_VLINE);
        move(y+j+1,x+width+1);
        addch(ACS_VLINE);
    }
    move(y+j+1,x);
    addch(ACS_LLCORNER);
    for(i=0;i<width;i++)
        addch(ACS_HLINE);
    addch(ACS_LRCORNER);
}

void play(){
    int command;
    clear();
    act.sa_handler = BlockDown;
    sigaction(SIGALRM,&act,&oact);
    InitTetris();
    recommend(field, 0);
    do{
        if(timed_out==0){
            alarm(1);
            timed_out=1;
        }

        command = GetCommand();
        if(ProcessCommand(command)==QUIT){
            alarm(0);
            DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
            move(HEIGHT/2,WIDTH/2-4);
            printw("Good-bye!!");
            refresh();
            getch();
            
            return;
        }
    }while(!gameOver);
    
    alarm(0);
    getch();
    DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
    move(HEIGHT/2,WIDTH/2-4);
    printw("GameOver!!");
    refresh();
    getch();
    newRank(score);
}

char menu(){
    printw("1. play\n");
    printw("2. rank\n");
    printw("3. recommended play\n");
    printw("4. exit\n");
    return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
    int i, j;
    for(i = 0; i < 4; i++){
        for(j = 0 ; j < 4; j++){
            if(block[currentBlock][blockRotate][i][j] ==1){
                if(f[blockY + i][blockX + j]) return 0;
                if(blockY + i >= HEIGHT) return 0;
                if(blockX + j < 0) return 0;
                if(blockX + j >= WIDTH) return 0;
            }
        }
    }
    return 1;
}
//6주차 과제//
void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
    int pRotate = blockRotate,
        pX = blockX,
        pY = blockY;
    int mark = 1;
                     
    switch(command) {
        case KEY_UP :
            pRotate = (pRotate + 3) % 4;
            break;
        case KEY_DOWN :
            pY--;
            break;
        case KEY_RIGHT :
            pX--;
            break;
        case KEY_LEFT :
            pX++;
            break;
    }
    
    for(int i=0;i<4;i++){
        for(int j=0; j<4;j++){
            if(block[currentBlock][pRotate][i][j] == 1 && i + pY >= 0) {
                move(i + pY + 1, j + pX + 1);
                printw(".");
            }
        }
    }
                     
    while(mark){
        mark = CheckToMove(field, currentBlock, pRotate, ++pY, pX);
    }
        pY--;
        for(int i=0;i<4;i++){
                     for(int j =0; j<4;j++){
                     if(block[currentBlock][pRotate][i][j] == 1 && i + pY >=0){
                     move(i + pY + 1,j + pX + 1);
                     printw(".");
                     }
                     }
                     }
    
    DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
}
// 6주차 과제 수정 //
void BlockDown(int sig){
    if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX) == 1) {
            blockY++;
            DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
        }
        else {
            if (blockY == -1) {
                gameOver = 1;
                return;
            }

            score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
            score += DeleteLine(field);
            
            nextBlock[0] = nextBlock[1];
            nextBlock[1] = rand() % 7;
            nextBlock[2] = rand() % 7;
                     
            blockRotate = 0;
            blockY = -1;
            blockX = (WIDTH / 2) - 2;

            recommend(field, 0);
            DrawField();
            DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
            DrawNextBlock(nextBlock);
            PrintScore(score);
        }

        timed_out = 0;
    }

	//강의자료 p26-27의 플로우차트를 참고한다.

                     //6주차 과제 수정//
// 이 밑부분 함수가 void로 되어있었는데 되지 않아 .h파일과 이걸 int로 수정//
int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
    int touched = 0;
	for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
                     if (block[currentBlock][blockRotate][i][j] == 1){
                f[i + blockY][j + blockX] = 1;
                if (f[i + blockY + 1][j + blockX] == 1)
                     touched++;
        }
    }

	//Block이 추가된 영역의 필드값을 바꾼다.
}
            return touched * 10;
                     }

int DeleteLine(char f[HEIGHT][WIDTH]){
	int m_mark = 0, count = 0;

    for (int i = 1; i < HEIGHT; i++) {
        m_mark = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (f[i][j] == 0) {
                m_mark = 0;
                break;
            }
        }
        if (m_mark == 1) {
            for (int j = i; j > 0; j--) {
                for (int k = 0; k < WIDTH; k++) {
                    f[j][k] = f[j - 1][k];
                }
            }
            count += 1;
        }
    }
    
    return 100 * count * count;
}
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.

///////////////////////////////////////////////////////////////////////////
//6주차 과제 추가 함수 DrawBlockWithFeatures//
void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
    DrawRecommend(recommendY, recommendX, nextBlock[0], recommendR);
                     DrawShadow(y,x,blockID,blockRotate);
                     DrawBlock(y,x,blockID,blockRotate, ' ');
                     }
                     
//  6주차 과제 부분 //
void DrawShadow(int y, int x, int blockID,int blockRotate){
    int mark = 1;
    while(mark) {
        mark = CheckToMove(field, blockID, blockRotate, ++y, x);
    }
    y--;
    DrawBlock(y,x,blockID, blockRotate, '/');
    
	// user code
}
///////////////////////////////////////////////////

// 7주차 실습 부분 //

void createRankList(){
    // 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
    // 1. "rank.txt"열기
    // 2. 파일에서 랭킹정보 읽어오기
    // 3. LinkedList로 저장
    // 4. 파일 닫기
    FILE *fp;
    int i, j, num;
    ptr newprofile, TAIL;
    HEAD = (ptr)malloc(sizeof(profile));
    TAIL = HEAD;
    
    fp = fopen("rank.txt", "r");
    fscanf(fp, "%d%*c", &rank_c);
    
    for(i = 0; i < rank_c; i++) {
        newprofile = (ptr)malloc(sizeof(profile));
        fscanf(fp, "%s %d%*c", newprofile->name, &newprofile->score);
        newprofile->next = NULL;
        TAIL->next = newprofile;
        TAIL = newprofile;
    }

    
    // 2. 정보읽어오기
    /* int fscanf(FILE* stream, const char* format, ...);
    stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
    format: 형식지정자 등등
    변수의 주소: 포인터
    return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
    // EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
    
    // 4. 파일닫기
    fclose(fp);
}

void rank(){
    //목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
    //1. 문자열 초기화
    int X=1, Y, i, j, deletenum;
    char ch, cx, cy;
    int name_s=0, Er;
    char name[NAMELEN];
    ptr temp = HEAD, pr;
    clear();
    //2. printw()로 3개의 메뉴출력
    printw("1. list ranks from X to Y\n");
    printw("2. list ranks by a specific name\n");
    printw("3. delete a specific rank\n");

    //3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
    ch = wgetch(stdscr);
    //4. 각 메뉴에 따라 입력받을 값을 변수에 저장
    //4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
    if (ch == '1') {
        echo();
        printw("X: ");
        scanw("%c", &cx);
        if('0'<=cx && cx<=rank_c+'0')
            X = cx-'0';
        else
            X = 1;
        printw("Y: ");
        scanw("%c", &cy);
        if('0'<=cy && cy<=rank_c+'0')
            Y = cy-'0';
        else
            Y = rank_c;
        noecho();
        printw("     name     |   score   \n");
        printw("--------------------------\n");

        if(X > Y) {
            printw("\nsearch failure: no rank in the list");
            wgetch(stdscr);
            return;
        }

        for(i = 0; i < X; i++) {
            temp = temp->next;
        }

        while(i++ <= Y) {
            printw(" %-13s| %d\n", temp->name, temp->score);
            temp = temp->next;
        }

        wgetch(stdscr);
        
    }

    else if ( ch == '2') {
        echo();
        printw("input the name: ");
        scanw("%s", name);
        noecho();
        
        printw("     name     |   score   \n");
        printw("--------------------------\n");

        temp = temp->next;
            while(temp) {
                if(strcmp(name, temp->name) == 0) {
                    printw(" %-13s| %d\n", temp->name, temp->score);
                    name_s = 1;
                }
                temp = temp->next;
            }

            if(!name_s)
                printw("\nsearch failure: no name in the list");
            
            wgetch(stdscr);
    }

    else if ( ch == '3') {
        echo();
        printw("input the rank: ");
        scanw("%d", &Er);
        noecho();

        if(!(Er >= 1 && Er <= rank_c)) {
            printw("\nsearch failure: the rank not in the list");
            return ;
        }

        printw("\nresult: the rank deleted");

        for(i = 0; i < Er; i++) {
            pr = temp;
            temp = temp->next;
        }

        pr->next = temp->next;
        free(temp);
        rank_c--;

        writeRankFile();

        wgetch(stdscr);
    }

}

void writeRankFile(){
    // 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
    int sn, i;
    //1. "rank.txt" 연다
    FILE *fp = fopen("rank.txt", "w");
    
    ptr temp = HEAD->next;
    //2. 랭킹 정보들의 수를 "rank.txt"에 기록

    //3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
    fprintf(fp, "%d\n", rank_c);
    while(temp){
        fprintf(fp, "%s %d\n", temp->name, temp->score);
        temp = temp->next;
    }
    
    fclose(fp);
}

void newRank(int score){
    // 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
    ptr new_profile, temp = HEAD, tempo;
    new_profile = (ptr)malloc(sizeof(profile));
    char name[NAMELEN];
    int i, j;
    clear();
    //1. 사용자 이름을 입력받음
    echo();
    printw("your name: ");
    
    
    getstr(name);
    strcpy(new_profile->name, name);
    noecho();

    new_profile->score = score;
    new_profile->next = NULL;

    if(rank_c == 0) HEAD->next = new_profile;
    else {
        while(temp->next) {
            tempo = temp;
            temp = temp->next;
            if(score > temp->score) {
                tempo->next = new_profile;
                new_profile->next = temp;
                break;
            }
        }
        if(!(temp->next)) {
            if(score > temp->score) {
                tempo->next = new_profile;
                new_profile->next = temp;
            }
            else {
                temp->next = new_profile;
            }
        }
    }

    rank_c++;
    
    
    writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
    DrawBlock(y, x, blockID, blockRotate, 'R');
    
    // user code
}

int recommend(char rec[HEIGHT][WIDTH], int lev){
    int i, j, r, x, y, idx = 0, max=0;
    // 미리 보이는 블럭의 추천 배치를 고려했을 때 얻을 수 있는 최대 점수
    int touch = 0;
    int cs;
    char new_node[HEIGHT][WIDTH];
    
    // 회전 4가지
    for (r = 0; r < 4; r++) {

        if(r >= 2 && (nextBlock[lev] == 0 || nextBlock[lev] >= 4))
            continue;
        
        // x축 위에서 움직이며 위치에 대한 가지수
        for (i = -1; i < WIDTH; i++) {

            for(y = 0; y < HEIGHT; y++)
                for(x = 0; x < WIDTH; x++)
                    new_node[y][x] = rec[y][x];
            touch = 0;

            for(j = 0; j < HEIGHT; j++)
                if(!CheckToMove(rec, nextBlock[lev], r, j, i))
                    break;
            j--;
            
            if(j == -1) continue;

            for (y = 0; y < 4; y++) {
                for (x = 0; x < 4; x++) {
                    if(block[nextBlock[lev]][r][y][x] == 1) {
                        if(j + y + 1 <= HEIGHT && (new_node[j + y + 1][i + x] || j + y + 1 == HEIGHT))
                            touch++;
                        new_node[j + y][i + x] = 1;
                    }
                }
            }

            cs = DeleteLine(new_node);

            if(lev != 2)
                cs += recommend(new_node, lev + 1);

            if(max <= (cs + touch * 10)) {
                max = cs + touch * 10;
                if(lev == 0) {
                    recommendX = i;
                    recommendY = j;
                    recommendR = r;
                }
            }
        }
    }

    return max;
}

void recommendedPlay(){
    //user code
}
