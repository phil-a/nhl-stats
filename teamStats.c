#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/***********************************************************************************************

//***********************************************************************************************/
char line[4200];
char c;
int i = 0;

//***********************************************************************************************
//Structure of each team
struct team
{
int place;
char teamName[15];
int gp;	//gamesplayed
int w;	//wins
int l;	//losses
int ot; //ot losses
int p; 	//points
float row; //regulation + ot wins
float hrow; //home reg + ot wins
float rrow; //road reg + ot wins
float pp; //point percentage
float gpg; // goals per game
float gapg; //goals against per game
float fvf_fa; // 5 on 5 for/against ratio
float pwrplay; // power play %
float penkill; // penalty kill %
float spg; //shots per game
float sapg; // shots against per game
float wp_sf; //winning % - scoring first
float wp_tf; //winning % - trailing first
float wp_lap1; //winning % - leading after period 1
float wp_lap2; //winning % - leading after period 1
float wp_outshoot; //winning % - outshooting
float wp_outshot; //winning % - outshot
float faceoff; //face off win %
float prWin; //pythagenPuck
float E;

float prWin2; //pythagenPuck
float E2;

} team[25];
//***********************************************************************************************

int main()
{
FILE *fp;
int i = 0;
int j = 0;
int cnt = 0;
int outOfBrack = 0;
//**********************************************************************************************
//	Read in team stats from site
fp=(popen("wget --quiet -O - http://www.nhl.com/ice/teamstats.htm?navid=nav-sts-teams#","r"));
FILE *file1 = fopen("file1.txt","w");
while ((c=(getc(fp))) != EOF)
{
	if (c == '<')
	outOfBrack = 1;
	if (c == '>')
	outOfBrack = 0;

	if (outOfBrack == 0)
	fprintf(file1,"%c",c);
}
fclose(file1);
pclose(fp);
//***********************************************************************************************

system("grep TORONTO file1.txt | tr '/>' ' ' > file2.txt");
system("rm file1.txt");

//***********************************************************************************************
//	Save into array
FILE *file2 = fopen("file2.txt","r");
while ((c=(getc(file2))) != EOF)
{
	line[i] = c;
	i++;	
}
//printf("cnt: %d", i);
fclose(file2);
system("rm file2.txt");
//***********************************************************************************************
//get rid of garbage
for (i = 0; i < 42; i++)
{
line[i] = ' ';
}
//get rid of garbage
for (i = 4094; i < 4199; i++)
{
line[i] = ' ';
}

//fix city names
for (i = 0; i < 4197;i++)
{
	if ((line[i] >= 65) && (line[i] <= 90))
	{
	//printf("Z>d>A");
		if ((line[i+2] >= 65) && (line[i+2] <= 90))
		{
			//printf("Z>f>A");
			if(line[i+1] == ' ')
			{
			//	printf("e=blank");
				line[i+1] = '_';
			}
		}
	}
}

//***********************************************************************************************

//***********************************************************************************************
//	Create file with wanted format
FILE *file3 = fopen("file3.txt","w");
int teamCnt = 0;
int isSpace = 1;
int elemCnt = 0;
for (i = 0; i < 4200; i++)
{
	
	if ((isSpace == 1) && (line[i] != ' '))
	{
	elemCnt++;
	isSpace = 0;
	fputc(line[i],file3);
	}
	else if ((isSpace == 0) && (line[i] == ' '))
	{
	isSpace = 1;
	fputc(line[i],file3);
	}

	else if ((elemCnt == 25) && (line[i] == ' '))
	{
	line[i] = '\n';
	fputc('\n',file3);
	elemCnt = 0;
	teamCnt++;
	isSpace = 1;
	}
	else
	{
	fputc(line[i],file3);
	}
}

fclose(file3);
//***********************************************************************************************
//	Read into Team Structure

line[0] = '\0';
file3 = fopen("file3.txt","r");
i = 0;

while ((fgets(line, sizeof(line), file3)) != NULL )
{
sscanf(line,"%d %s %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", &team[i].place, team[i].teamName, &team[i].gp, &team[i].w, &team[i].l, &team[i].ot, &team[i].p, &team[i].row, &team[i].hrow, &team[i].rrow, &team[i].pp, &team[i].gpg, &team[i].gapg, &team[i].fvf_fa, &team[i].pwrplay, &team[i].penkill, &team[i].spg, &team[i].sapg, &team[i].wp_sf, &team[i].wp_tf, &team[i].wp_lap1, &team[i].wp_lap2, &team[i].wp_outshoot, &team[i].wp_outshot, &team[i].faceoff);
i++;
}

fclose(file3);
system("rm file3.txt");

//***********************************************************************************************
//Prediction Calculation

for (i = 0; i < 25; i++)
{
team[i].E = powf((team[i].gpg + team[i].gapg),0.458);
team[i].prWin = powf(team[i].gpg,team[i].E) / ( (powf(team[i].gpg,team[i].E)) + (powf(team[i].gapg,team[i].E)) );
}

//***********************************************************************************************
//print statement
for (i = 0; i < 25; i++)
{
printf("%s   \tPrWin:%f\n", team[i].teamName,team[i].prWin);
	
}
//***********************************************************************************************
return 0;
}


