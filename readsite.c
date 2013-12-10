#include <stdio.h>
#include <stdlib.h>
/*******************************************************************************************

********************************************************************************************/
char line[4200];
char c;
int i = 0;

//*******************************************************************************************
//Structure of each team
struct team
{
int place;
char teamName[15];
int gp;			//gamesplayed
int w;			//wins
int l;			//losses
int ot; 		//ot losses
int p; 			//points
float row;	
float hrow;	
float rrow;	
float pp;	
float gpg;
float gapg;	
float fvf_fa;	
float pwrplay;	
float penkill;	
float spg;	
float sapg;	
float wp_sf;	
float wp_tf;	
float wp_lap1;	
float wp_lap2;	
float wp_outshoot;	
float wp_outshot;	
float faceoff;	
} team[25];
//*******************************************************************************************

int main()
{
FILE *fp;
int i = 0;
int j = 0;
int cnt = 0;
int outOfBrack = 0;
//*******************************************************************************************
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
//*******************************************************************************************

system("grep TORONTO file1.txt | tr '/>' ' ' > file2.txt");
system("rm file1.txt");

//*******************************************************************************************
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
//*******************************************************************************************
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

//*******************************************************************************************

//*******************************************************************************************
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
//*******************************************************************************************
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
//*******************************************************************************************
//print statement
for (i = 0; i < 25; i++)
{
	if (team[i].gpg > 3.0)
	{
	printf("%s\n", team[i].teamName);
	}
}
//*******************************************************************************************
return 0;
}

