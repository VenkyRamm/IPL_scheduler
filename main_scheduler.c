#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "playoffs.h"

#define MAX_TEAMS 8
#define MAX_MATCHES 56


typedef struct {
    char name[20];
    char homeVenue[30];
} Team;

typedef struct {
    char team1[20], team2[20];
    char venue[30];
    char date[15];
    char time[10];
} Match;

typedef struct {
    char team[20];
    int matchesPlayed, wins, losses, points;
    int runsScored, runsConceded;
    float netRunRate;
} PointsTable;

Team teams[MAX_TEAMS];
Match matches[MAX_MATCHES];
PointsTable points[MAX_TEAMS];
int matchCount = 0;

void initializeTeams() {
    char *teamNames[MAX_TEAMS] = {"MI", "CSK", "RCB", "KKR", "SRH", "DC", "PBKS", "RR"};
    char *venues[MAX_TEAMS] = {"Mumbai", "Chennai", "Bangalore", "Kolkata", "Hyderabad", "Delhi", "Punjab", "Jaipur"};
    
    for (int i = 0; i < MAX_TEAMS; i++) {
        strcpy(teams[i].name, teamNames[i]);
        strcpy(teams[i].homeVenue, venues[i]);
        strcpy(points[i].team, teamNames[i]);
        points[i].matchesPlayed = points[i].wins = points[i].losses = points[i].points = 0;
        points[i].runsScored = points[i].runsConceded = 0;
        points[i].netRunRate = 0.0;
    }
}

void generateSchedule() {
    int day = 1, month = 4;
    int pairs[MAX_TEAMS][MAX_TEAMS] = {0};
    int homeVenueUsed[MAX_TEAMS][MAX_TEAMS] = {0}; 
    int attempt = 0;

    while (matchCount < MAX_TEAMS * (MAX_TEAMS - 1)) {
        int team1 = rand() % MAX_TEAMS;
        int team2 = rand() % MAX_TEAMS;

        
        if (team1 == team2 || pairs[team1][team2] >= 2) {
            attempt++;
            continue;
        }

        if (matchCount > 0) {
            if (strcmp(teams[team1].name, matches[matchCount-1].team1) == 0 || 
                strcmp(teams[team1].name, matches[matchCount-1].team2) == 0 ||
                strcmp(teams[team2].name, matches[matchCount-1].team1) == 0 || 
                strcmp(teams[team2].name, matches[matchCount-1].team2) == 0) 
            {
                attempt++;
                continue;
            }
        }


        if (homeVenueUsed[team1][team2] == 0) {
            strcpy(matches[matchCount].team1, teams[team1].name);
            strcpy(matches[matchCount].team2, teams[team2].name);
            strcpy(matches[matchCount].venue, teams[team1].homeVenue);
            homeVenueUsed[team1][team2] = 1;  
        } else {
            strcpy(matches[matchCount].team1, teams[team2].name);
            strcpy(matches[matchCount].team2, teams[team1].name);
            strcpy(matches[matchCount].venue, teams[team2].homeVenue);
            homeVenueUsed[team1][team2] = 0;  
        }

        sprintf(matches[matchCount].date, "2025-%02d-%02d", month, day);
        if(matchCount%7==0){
            sprintf(matches[matchCount].time,"3:30 PM");
            day--;
        }
        else{
            sprintf(matches[matchCount].time,"7:30 PM");
        }

        pairs[team1][team2]++;
        pairs[team2][team1]++;
        matchCount++;

        if (++day > 30) {
            day = 1;
            month++;
        }
    }
}

void updatePointsTable(char winner[], char loser[], int runsWinner, int runsLoser) {
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (strcmp(points[i].team, winner) == 0) {
            points[i].matchesPlayed++;
            points[i].wins++;
            points[i].points += 2;
            points[i].runsScored += runsWinner;
            points[i].runsConceded += runsLoser;
        } 
        if (strcmp(points[i].team, loser) == 0) {
            points[i].matchesPlayed++;
            points[i].losses++;
            points[i].runsScored += runsLoser;
            points[i].runsConceded += runsWinner;
        }
    }
}

void calculateNRR() {
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (points[i].matchesPlayed > 0) {
            float overs = points[i].matchesPlayed * 20.0;
            points[i].netRunRate = ((float)points[i].runsScored/overs) - ((float)points[i].runsConceded/overs);
        }
    }
}

void simulateMatches() {
    for (int i = 0; i < matchCount; i++) {
        char winner[20], loser[20];
        int runs1 = 120 + rand() % 80;
        int runs2 = 120 + rand() % 80;

        if (runs1 >= runs2) {
            strcpy(winner, matches[i].team1);
            strcpy(loser, matches[i].team2);
        } else {
            strcpy(winner, matches[i].team2);
            strcpy(loser, matches[i].team1);
            int temp = runs1;
            runs1 = runs2;
            runs2 = temp;
        }
        updatePointsTable(winner, loser, runs1, runs2);
    }
    calculateNRR();
}

void displayPointsTable() {
    for (int i = 0; i < MAX_TEAMS - 1; i++) {
        for (int j = 0; j < MAX_TEAMS - i - 1; j++) {
            if (points[j].points < points[j + 1].points ||
                (points[j].points == points[j + 1].points && points[j].netRunRate < points[j + 1].netRunRate)) {
                PointsTable temp = points[j];
                points[j] = points[j + 1];
                points[j + 1] = temp;
            }
        }
    }

    printf("\n******** Final Points Table ********\n");
    printf("Team\tMP\tW\tL\tPts\tNRR\n");

    PlayoffTeam toppers[MAX_TOPTEAMS]; 
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (i < MAX_TOPTEAMS) {
            strcpy(toppers[i].team, points[i].team);
            toppers[i].netRunRate = points[i].netRunRate;
        }
        printf("%s\t%d\t%d\t%d\t%d\t%.2f\n", points[i].team, points[i].wins + points[i].losses,
               points[i].wins, points[i].losses, points[i].points, points[i].netRunRate);
    }

    real(toppers); 
}

void displaySchedule() {
    printf("\n******** IPL Match Schedule ********\n");
    for (int i = 0; i < matchCount; i++) {
        printf("%d Match: %s vs %s | Date: %s | Time: %s | Venue: %s\n", i+1,
               matches[i].team1, matches[i].team2, matches[i].date, matches[i].time, matches[i].venue);
    }
}

int main() {
    srand(time(NULL));
    initializeTeams();
    generateSchedule();
    printf("Total Matches Scheduled: %d\n", matchCount);
    displaySchedule();
    simulateMatches(); 
    displayPointsTable(); 
    return 0;
}
