#include "ulibc.h"

#define SIZE 3

int b[SIZE][SIZE];
int turn;

/*
    Fill Board with 0
*/
void initBoard(){
    int i,j;
     for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            b[i][j]=0;
        }
    }
}

/*
    Display Board & infos to Ascii art
*/
void print(){
    int x;
    for(x=0;x<SIZE;x++)
            printf("----");
    printf("\n");
    int i,j;
    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            if(j != SIZE-1)
                printf(" %d |",b[i][j]);
            else
                printf(" %d ",b[i][j]);
        }
        printf("\n");
        for(x=0;x<SIZE;x++)
            printf("----");
        printf("\n");
    }
    printf("Player : (%d)\n",turn);
     
}

/*
    Check if current Pos give Victory
*/
int checkV(int i,int j){
    int token = 0;
    int item = b[i][j];
    int count = 0;
    
    int it;
    //check line
    for(it=0;it<SIZE;it++){
        if(b[it][j] == item)
            count++;
        else
            count=0;
    }
    
    if(count == SIZE){
       printf("%d won line\n",item);
       token = 1;
    }
        
    count = 0;
    
    //check column
    int jt;
    for(jt=0;jt<SIZE;jt++){
        if(b[i][jt] == item)
            count++;
        else
            count=0;
    }
    
    if(count == SIZE){
        printf("%d won colum\n",item);
        token = 1;
    }
        
    count = 0;
    
    if(i==j){
        for(jt=0;jt<SIZE;jt++){
            if(b[jt][jt] == item)
                 count++;
            else
                count=0;
        }
        
        if(count == SIZE){
            printf("%d won diag left\n",item);
            token = 1;
        }
            
        count = 0;
    }
    
    if(i+j == SIZE-1){
        int loop = 0;
        it = SIZE-1;
        jt = 0;
        while(loop!=SIZE){
            loop++;
            if(b[it][jt] == item)
                 count++;
            else
                count=0;
            
            it-=1;
            jt+=1;
        }
        if(count == SIZE){
            printf("%d won diag right\n",item);
            token = 1;
        }
            
        count = 0;
    }
    return token;
}

/*
 Switch Turn 
*/
int turnGame(){
    int x,y;
    while(true){
        printf("Movement\n");
        printf("X Pos :\n");
        char xChar = getc();
        printf("Y Pos :\n");
        char yChar = getc();
    
        x= xChar - '0';
        y= yChar - '0';
        
        if(x<SIZE && x>=0 && y<SIZE && y>=0 && b[x][y]==0)
            break;
    }
    
    
    if(turn == 1)
        b[x][y] = 1;
    else
        b[x][y] = 2;

    
    turn = (turn%2)+1;
    print();
    return checkV(x,y);
}

int main()
{
    initBoard();
    turn = 1;
    print();
    
    int max = SIZE* SIZE;
    while(turnGame()==0 && max != 0){
        max-=1;
    }
    
    return 0;
}

