//Eren Öztürk METU NCC 140 PA5
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NameLong 20

struct structOfScores{//Structure of the Score Table
    char TeamOne[NameLong];
    char TeamTwo[NameLong];
    int TeamOnesScore;
    int TeamTwosScore;
};
struct structOfTable{//Structure of the League Table
    char TeamName[NameLong];
    int WinMatches;
    int DrawMatches;
    int LostMatches;
    int Points;
    int Average;
    int ScoredGoals;
};

int LoadScoreTable(char fileName[NameLong], struct structOfScores *);
void CreateLeagueTable(struct structOfScores *, int NumberOfTeam, struct structOfTable *);
void OrderLeagueTable(int NumberOfTeam, struct structOfTable *);
void DisplayLeagueTable(int NumberOfTeam, struct structOfTable *);
int SearchTeam(int NumberOfTeam, struct structOfTable *);

int main(){
    int NumberOfMatchs;
    struct structOfScores *scoreTable;
    scoreTable=(struct structOfScores*)malloc(NumberOfMatchs*sizeof(struct structOfScores));

    char fileName[]="scores.txt";

    NumberOfMatchs=LoadScoreTable(fileName, scoreTable);//return the number of matches

    struct structOfTable leagueTable[NumberOfMatchs*2];//Therefore, 2 times of number of matches gives me how many team in the league, so I can create league table now

    CreateLeagueTable(scoreTable, NumberOfMatchs*2, leagueTable);

    OrderLeagueTable(NumberOfMatchs*2, leagueTable);

    DisplayLeagueTable(NumberOfMatchs*2, leagueTable);

    printf("\n");
    int checker;
    char choice;
    do{
        printf("Press E for exit or S for search: ");//Taking users choice for search or exit
        fflush(stdin);
        scanf("%c",&choice);
        if(choice=='E' || choice=='e')//If choice is 'E' or 'e' then user want to exit so breaking the loop is a good solution for it
            break;

        checker=SearchTeam(NumberOfMatchs*2, leagueTable);//Goes for a search

        if(checker==-1)
            printf("That team is unknown! Please try again!\n");//If could not find team checker would be -1 so it didn't find the team
        else{//It will be one if team name is in the table
            printf("%s's rank is %d!\n",leagueTable[checker].TeamName, checker+1);//It will enter and says its rank
        }

    }while(choice!='E');//If something entered different from 'E' or 'e' it continue to take Team Names and search it



    printf("Wishing you a successful week!\n");

    return 0;
}

int LoadScoreTable(char fileName[NameLong], struct structOfScores *scoreTable){
    int counterOfHowManyMatchPlayed=0, counterForTable;

    FILE *inFile;
    inFile=fopen(fileName, "r");//Open stream for read the file
    if(inFile == NULL){
        printf("%s was not opened.\n",fileName);
        exit(1);
    }

    while(fscanf(inFile, "%s %s %d %d\n", scoreTable[counterOfHowManyMatchPlayed].TeamOne, scoreTable[counterOfHowManyMatchPlayed].TeamTwo, &scoreTable[counterOfHowManyMatchPlayed].TeamOnesScore, &scoreTable[counterOfHowManyMatchPlayed].TeamTwosScore) != EOF){//Loading file
        counterOfHowManyMatchPlayed++;//Reads first teams name, second teams name, first teams score, and second teams score
    }

    fclose(inFile);//Close stream

    printf("Score records file has been successully loaded!\nFollowing records have been loaded:\n\n");//Let's create score table
    printf("\tTeam1\t\tTeam2\t\tTeam1Score\tTeam2Score\n\t----------------------------------------------------------\n");
    for(counterForTable=0;counterForTable<counterOfHowManyMatchPlayed;counterForTable++){
        if(strlen(scoreTable[counterForTable].TeamOne)<8)//For better look, looks to length of the team names and add tab or not
            printf("\t%s\t\t",scoreTable[counterForTable].TeamOne);
        else if(strlen(scoreTable[counterForTable].TeamOne)<16)
            printf("\t%s\t",scoreTable[counterForTable].TeamOne);

        if(strlen(scoreTable[counterForTable].TeamTwo)<8)//For better look, looks to length of the team names and add tab or not
            printf("%s\t\t",scoreTable[counterForTable].TeamTwo);
        else if(strlen(scoreTable[counterForTable].TeamTwo)<16)
            printf("%s\t",scoreTable[counterForTable].TeamTwo);

        printf("%d\t\t%d\n",scoreTable[counterForTable].TeamOnesScore, scoreTable[counterForTable].TeamTwosScore);//Print the scores
    }

    return counterOfHowManyMatchPlayed;//Return the how many matches played
}

void CreateLeagueTable(struct structOfScores *scoreTable, int NumberOfTeam, struct structOfTable leagueTable[NumberOfTeam]){
    int counter, counterForLeagueTable;
    for(counter=0;counter<NumberOfTeam;counter++){//Initially all of them needs to be zero
        leagueTable[counter].WinMatches=0;
        leagueTable[counter].DrawMatches=0;
        leagueTable[counter].LostMatches=0;
        leagueTable[counter].Points=0;
        leagueTable[counter].Average=0;
        leagueTable[counter].ScoredGoals=0;
    }
    //However, after matches played it will change
    for(counter=0, counterForLeagueTable=0;counterForLeagueTable<NumberOfTeam;counter++, counterForLeagueTable+=2){
        strcpy(leagueTable[counterForLeagueTable].TeamName, scoreTable[counter].TeamOne);
        strcpy(leagueTable[counterForLeagueTable+1].TeamName, scoreTable[counter].TeamTwo);//Copying names to the league table

        leagueTable[counterForLeagueTable].ScoredGoals+=scoreTable[counter].TeamOnesScore;
        leagueTable[counterForLeagueTable+1].ScoredGoals+=scoreTable[counter].TeamTwosScore;//Loading goals to the league table

        leagueTable[counterForLeagueTable].Average+=scoreTable[counter].TeamOnesScore-scoreTable[counter].TeamTwosScore;
        leagueTable[counterForLeagueTable+1].Average+=scoreTable[counter].TeamTwosScore-scoreTable[counter].TeamOnesScore;//Loading averages to the league table

        if(scoreTable[counter].TeamOnesScore>scoreTable[counter].TeamTwosScore){//TeamOne wins
            leagueTable[counterForLeagueTable].WinMatches+=1;
            leagueTable[counterForLeagueTable].Points+=3;
            leagueTable[counterForLeagueTable+1].LostMatches+=1;
        }
        else if(scoreTable[counter].TeamOnesScore<scoreTable[counter].TeamTwosScore){//TeamTwo wins
            leagueTable[counterForLeagueTable+1].WinMatches+=1;
            leagueTable[counterForLeagueTable+1].Points+=3;
            leagueTable[counterForLeagueTable].LostMatches+=1;
        }
        else{//Draw
            leagueTable[counterForLeagueTable].DrawMatches+=1;
            leagueTable[counterForLeagueTable].Points+=1;
            leagueTable[counterForLeagueTable+1].DrawMatches+=1;
            leagueTable[counterForLeagueTable+1].Points+=1;
        }
    }
}

void OrderLeagueTable(int NumberOfTeam, struct structOfTable leagueTable[NumberOfTeam]){
    int intervalCounter, findingBestOneCounter;//Counters
    int betterOnesPoints, betterOnesAverage, betterOnesGoals, betterOnesLine;//Help to find the best
    int changerWin, changerDraw, changerLoss, changerPoints, changerAverage, changerGoals;//Help to change
    char helpToChangeName[NameLong];

    for(intervalCounter=0;intervalCounter<NumberOfTeam;intervalCounter++){//Finds best one in the interval then put it on the top then not include the ex best one find new best one in new interval
        for(findingBestOneCounter=intervalCounter, betterOnesPoints=-1, betterOnesAverage=-100, betterOnesGoals=-1;findingBestOneCounter<NumberOfTeam;findingBestOneCounter++){
            if(leagueTable[findingBestOneCounter].Points>betterOnesPoints){//If another ones point is more than the ex better ones
                betterOnesPoints=leagueTable[findingBestOneCounter].Points;//Changing better one
                betterOnesAverage=leagueTable[findingBestOneCounter].Average;
                betterOnesGoals=leagueTable[findingBestOneCounter].ScoredGoals;
                betterOnesLine=findingBestOneCounter;
            }
            if(leagueTable[findingBestOneCounter].Points==betterOnesPoints && leagueTable[findingBestOneCounter].Average>betterOnesAverage){//If another ones point is equal but average is more than the ex better ones
                betterOnesPoints=leagueTable[findingBestOneCounter].Points;//Changing better one
                betterOnesAverage=leagueTable[findingBestOneCounter].Average;
                betterOnesGoals=leagueTable[findingBestOneCounter].ScoredGoals;
                betterOnesLine=findingBestOneCounter;
            }
            if(leagueTable[findingBestOneCounter].Points==betterOnesPoints && leagueTable[findingBestOneCounter].Average==betterOnesAverage && leagueTable[findingBestOneCounter].ScoredGoals>betterOnesGoals){//If another ones point and average is equal but scored goal is more than the ex better ones
                betterOnesPoints=leagueTable[findingBestOneCounter].Points;//Changing better one
                betterOnesAverage=leagueTable[findingBestOneCounter].Average;
                betterOnesGoals=leagueTable[findingBestOneCounter].ScoredGoals;
                betterOnesLine=findingBestOneCounter;
            }
        }
        if(betterOnesLine!=intervalCounter){//If the top one is better no need for change
            strcpy(helpToChangeName, leagueTable[intervalCounter].TeamName);//Placing better one to the top
            changerWin=leagueTable[intervalCounter].WinMatches;
            changerDraw=leagueTable[intervalCounter].DrawMatches;
            changerLoss=leagueTable[intervalCounter].LostMatches;
            changerPoints=leagueTable[intervalCounter].Points;
            changerAverage=leagueTable[intervalCounter].Average;
            changerGoals=leagueTable[intervalCounter].ScoredGoals;


            strcpy(leagueTable[intervalCounter].TeamName, leagueTable[betterOnesLine].TeamName);
            leagueTable[intervalCounter].WinMatches=leagueTable[betterOnesLine].WinMatches;
            leagueTable[intervalCounter].DrawMatches=leagueTable[betterOnesLine].DrawMatches;
            leagueTable[intervalCounter].LostMatches=leagueTable[betterOnesLine].LostMatches;
            leagueTable[intervalCounter].Points=leagueTable[betterOnesLine].Points;
            leagueTable[intervalCounter].Average=leagueTable[betterOnesLine].Average;
            leagueTable[intervalCounter].ScoredGoals=leagueTable[betterOnesLine].ScoredGoals;


            strcpy(leagueTable[betterOnesLine].TeamName, helpToChangeName);
            leagueTable[betterOnesLine].WinMatches=changerWin;
            leagueTable[betterOnesLine].DrawMatches=changerDraw;
            leagueTable[betterOnesLine].LostMatches=changerLoss;
            leagueTable[betterOnesLine].Points=changerPoints;
            leagueTable[betterOnesLine].Average=changerAverage;
            leagueTable[betterOnesLine].ScoredGoals=changerGoals;
        }
    }
}

void DisplayLeagueTable(int NumberOfTeam, struct structOfTable leagueTable[NumberOfTeam]){
    if(leagueTable==NULL)
        printf("Table has not been loaded!\n");// If the table has not been loaded then appropriate error message should be displayed to the user.

    else{
        printf("\n\nFormed Leauge Table is as follows:\n\n");//Preparing for the table
        printf("\tTeams\t\tWin\tDraw\tLoss\tPoints\tAverage\tGoals\n\t-------------------------------------------------------------\n");

        int counter;
        for(counter=0;counter<NumberOfTeam;counter++){//Display teams one by one
            if(strlen(leagueTable[counter].TeamName)<8)//For better look, looks to length of the team names and add tab or not
                printf("\t%s\t\t",leagueTable[counter].TeamName);
            else if(strlen(leagueTable[counter].TeamName)<16)
                printf("\t%s\t",leagueTable[counter].TeamName);
            //After displaying name of the team, it comes to display how many win, draw, loss, point, average, and goal made by this team
            printf("%d\t%d\t%d\t%d\t%d\t%d\n",leagueTable[counter].WinMatches, leagueTable[counter].DrawMatches, leagueTable[counter].LostMatches, leagueTable[counter].Points, leagueTable[counter].Average, leagueTable[counter].ScoredGoals);
        }
    }
}

int SearchTeam(int NumberOfTeam, struct structOfTable leagueTable[NumberOfTeam]){
        char TeamName[NameLong];
        printf("Enter the name of the team: ");//If user wanted to continue to search user will give a team name for search
        fflush(stdin);
        gets(TeamName);

        int findingTeamo;
        for(findingTeamo=0;findingTeamo<NumberOfTeam;findingTeamo++){//Try every team name in table
            if(strcmp(TeamName, leagueTable[findingTeamo].TeamName)==0){//If it is in table
                return findingTeamo;//Then it will return where it is
            }
        }
        return -1;//If could not find team return -1
}
