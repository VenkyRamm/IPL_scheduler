#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "playoffs.h"

typedef struct {
    char n[20];
    char v[30];
} Tm;

typedef struct {
    char t1[20], t2[20];
    char v[30];
    char d[15];
    char ti[10];
} Mt;

typedef struct {
    char n[20];
    int mp, w, l, pts;
    int rs, rc;
    float nrr;
} Pt;

void init(Tm t[], Pt p[]) {
    char *tn[8] = {"MI", "CSK", "RCB", "KKR", "SRH", "DC", "PBKS", "RR"};
    char *vn[8] = {"Mumbai", "Chennai", "Bangalore", "Kolkata", "Hyderabad", "Delhi", "Punjab", "Jaipur"};
    for (int i = 0; i < 8; i++) {
        strcpy(t[i].n, tn[i]);
        strcpy(t[i].v, vn[i]);
        strcpy(p[i].n, tn[i]);
        p[i].mp = p[i].w = p[i].l = p[i].pts = 0;
        p[i].rs = p[i].rc = 0;
        p[i].nrr = 0.0;
    }
}

int bRand() {
    return (rand() % 100) < 2;
}

void genSch(Tm t[], Mt m[], int *mc) {
    int d = 1, mo = 4, pr[8][8] = {0}, cnt[8] = {0}, hv[8][8] = {0}, c = 0;
    while (*mc < 56) {
        if (c > 1000) { printf("Too many retries.\n"); break; }
        int i = rand() % 8, j = rand() % 8;
        if (i == j || pr[i][j] >= 2 || cnt[i] >= 14 || cnt[j] >= 14) { c++; continue; }
        if (*mc > 0) {
            if (!strcmp(t[i].n, m[*mc-1].t1) || !strcmp(t[i].n, m[*mc-1].t2) || 
                !strcmp(t[j].n, m[*mc-1].t1) || !strcmp(t[j].n, m[*mc-1].t2)) { c++; continue; }
        }
        if (hv[i][j] == 0) {
            strcpy(m[*mc].t1, t[i].n);
            strcpy(m[*mc].t2, t[j].n);
            strcpy(m[*mc].v, t[i].v);
            hv[i][j] = 1;
        } else {
            strcpy(m[*mc].t1, t[j].n);
            strcpy(m[*mc].t2, t[i].n);
            strcpy(m[*mc].v, t[j].v);
        }
        sprintf(m[*mc].d, "2025-%02d-%02d", mo, d);
        sprintf(m[*mc].ti, (*mc % 8 == 0) ? "3:30 PM" : "7:30 PM");
        if (*mc % 7 == 0) { sprintf(m[*mc].ti, "3:30 PM"); d--; }
        else { sprintf(m[*mc].ti, "7:30 PM"); }
        cnt[i]++; cnt[j]++; pr[i][j]++; pr[j][i]++; (*mc)++;
        if (++d > 30) { d = 1; mo++; }
        c++;
    }
}

int closeV(const char *v1, const char *v2) {
    const char *p[][3] = {
        {"Mumbai", "Jaipur", "Punjab"},
        {"Chennai", "Bangalore", "Hyderabad"},
        {"Bangalore", "Chennai", "Hyderabad"},
        {"Kolkata", "Delhi", "Mumbai"},
        {"Hyderabad", "Bangalore", "Chennai"},
        {"Delhi", "Kolkata", "Punjab"},
        {"Punjab", "Delhi", "Mumbai"},
        {"Jaipur", "Mumbai", "Delhi"}
    };
    for (int i = 0; i < 8; i++)
        if (!strcmp(v1, p[i][0]))
            return (!strcmp(v2, p[i][1]) || !strcmp(v2, p[i][2]) || !strcmp(v1, v2));
    return 0;
}

void optTravel(Mt m[], int mc, Tm t[]) {
    char lv[8][30] = {""};
    for (int i = 0; i < mc - 1; i++) {
        for (int x = 0; x < 8; x++) {
            if (!strcmp(m[i].t1, t[x].n) || !strcmp(m[i].t2, t[x].n))
                strcpy(lv[x], m[i].v);
        }
        for (int j = i + 1; j < mc; j++) {
            int sw = 0;
            for (int x = 0; x < 8; x++) {
                if ((!strcmp(m[j].t1, t[x].n) || !strcmp(m[j].t2, t[x].n)) &&
                    strlen(lv[x]) > 0 && closeV(lv[x], m[j].v)) {
                    sw = 1;
                    break;
                }
            }
            if (sw) {
                Mt tmp = m[i+1];
                m[i+1] = m[j];
                m[j] = tmp;
                break;
            }
        }
    }
}

void updPts(Pt p[], char win[], char lose[], int rw, int rl) {
    for (int i = 0; i < 8; i++) {
        if (!strcmp(p[i].n, win)) {
            p[i].mp++; p[i].w++; p[i].pts += 2;
            p[i].rs += rw; p[i].rc += rl;
        } else if (!strcmp(p[i].n, lose)) {
            p[i].mp++; p[i].l++;
            p[i].rs += rl; p[i].rc += rw;
        }
    }
}

void calcNRR(Pt p[]) {
    for (int i = 0; i < 8; i++)
        if (p[i].mp > 0)
            p[i].nrr = ((float)p[i].rs / (p[i].mp * 20.0)) - ((float)p[i].rc / (p[i].mp * 20.0));
}

void simMts(Mt m[], Pt p[], int mc) {
    for (int i = 0; i < mc; i++) {
        int r1 = 120 + rand() % 80;
        int r2 = 120 + rand() % 80;
        char *w = r1 >= r2 ? m[i].t1 : m[i].t2;
        char *l = r1 >= r2 ? m[i].t2 : m[i].t1;
        updPts(p, w, l, r1 > r2 ? r1 : r2, r1 > r2 ? r2 : r1);
    }
    calcNRR(p);
}

void dispPts(Pt p[]) {
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 8 - i - 1; j++)
            if (p[j].pts < p[j+1].pts || (p[j].pts == p[j+1].pts && p[j].nrr < p[j+1].nrr)) {
                Pt tmp = p[j]; p[j] = p[j+1]; p[j+1] = tmp;
            }

    printf("\n******** Final Points Table ********\n");
    printf("Team\tMP\tW\tL\tPts\tNRR\n");

    PTeam top[4];
    for (int i = 0; i < 8; i++) {
        if (i < 4) {
            strcpy(top[i].t, p[i].n);
            top[i].nrr = p[i].nrr;
        }
        printf("%s\t%d\t%d\t%d\t%d\t%.2f\n", p[i].n, p[i].mp, p[i].w, p[i].l, p[i].pts, p[i].nrr);
    }

    char ch;
    printf("Simulate playoffs (y/n): ");
    scanf(" %c", &ch);
    if (ch == 'y') real(top);
}

void dispSch(Mt m[], int mc) {
    FILE *fp = fopen("schedule.txt", "w");
    printf("\n******** IPL Match Schedule ********\n");
    for (int i = 0; i < mc; i++) {
        if (i % 8 == 0) { printf("\n"); fprintf(fp, "\n"); }
        printf("Match %2d : %4s vs %4s | Date: %s | Time: %s | Venue: %s\n",
               i+1, m[i].t1, m[i].t2, m[i].d, m[i].ti, m[i].v);
        fprintf(fp, "Match %2d : %4s vs %4s | Date: %s | Time: %s | Venue: %s\n",
                i+1, m[i].t1, m[i].t2, m[i].d, m[i].ti, m[i].v);
    }
    fclose(fp);
}

void dispTmSch(Mt m[], int mc, char t[]) {
    printf("\n******** %s Schedule ********\n", t);
    for (int i = 0; i < mc; i++)
        if (!strcmp(m[i].t1, t) || !strcmp(m[i].t2, t))
            printf("Match %2d : %4s vs %4s | Date: %s | Time: %s | Venue: %s\n",
                   i+1, m[i].t1, m[i].t2, m[i].d, m[i].ti, m[i].v);
}

void dispVenSch(Mt m[], int mc, char v[]) {
    printf("\n******** %s Venue Schedule ********\n", v);
    for (int i = 0; i < mc; i++)
        if (!strcmp(m[i].v, v))
            printf("Match %2d : %4s vs %4s | Date: %s | Time: %s | Venue: %s\n",
                   i+1, m[i].t1, m[i].t2, m[i].d, m[i].ti, m[i].v);
}

void chkWeather(Mt m[], int mc) {
    printf("\nChecking weather...\n");
    int c=0;
    for (int i = 0; i < mc; i++) {
        if ((rand()%100) < 2 && c<1) {
            printf("Weather issue at %s, Match %d (%s vs %s) on %s. Rescheduling...\n",
                   m[i].v, i+1, m[i].t1, m[i].t2, m[i].d);
            sprintf(m[i].d, "2025-05-19");
            printf("New date: %s\n", m[i].d);
            c++;
        }
    }
    if (c==0) printf("No reschedules.\n");
}

int main() {
    Tm t[8];
    Mt m[56];
    Pt p[8];
    int mc = 0;
    srand(time(NULL));
    init(t, p);
    genSch(t, m, &mc);
    chkWeather(m, mc);
    printf("Total Matches Scheduled: %d\n", mc);
    dispSch(m, mc);
    int ch;
    while (1) {
        printf("Options:\n1.Team Schedule\n2.Venue Schedule\n3.Exit\nEnter: ");
        scanf("%d", &ch);
        if (ch == 1) {
            char tn[5];
            printf("Enter Team Name (caps): ");
            scanf("%s", tn);
            dispTmSch(m, mc, tn);
        } else if (ch == 2) {
            char v[25];
            printf("Enter Venue Name: ");
            scanf("%s", v);
            dispVenSch(m, mc, v);
        } else break;
    }
    simMts(m, p, mc);
    dispPts(p);
    return 0;
}
