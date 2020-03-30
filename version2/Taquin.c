#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 4
#define MAX2 16
#define MAX_SEUIL 2000

#define H 1
#define G 2
#define D 3
#define B 4
#define REV 5
#define FIN -1

#define ABS(X) (((X)>0)?((X)):(-(X)))
/*Le Brute Force c'est mal :D 
Fonctionne bien pour un taquin 3*3
idées bases:
grille 4*4 transformer en 3*3
exemple:
15 11 8 12         0  1  2  3
14 10 9 13         4  5  6  7 
 2  6  1 4   ===>8  9  10 11 (normalement c'est que je dois faire)
 3  7  4 0          12 13 14 15

Mais faisons une étape intermédiaire 
15 11 8 12	        X  X  X  3
14 10 9 13         X  X  X  7 
 2  6  1 4   ===>X  X  X  11 (Permet de savoir la profondeur minimal) problème les X ne sont peut-etre pas optimal pour la suite
 3  7  4 0          12 13 14 15

Exploiter des informations externes pour limiter les recherches - la position du blanc, -les coups DBGHDB sert à rien comme BDHGBD, BH,HB...; développer les noeuds k avant de vouloir faire k+1;

Trouver le nombre de cases mal placées : - des cases en conflits comme avec si la Casea et Caseb même ligne/colonne dans le taquin but (augmente le seuil de recherche)

Trouver le chemin le plus optimale: - donne une idée de la profondeur; - coup de recherche moins important; formule ABS(xb-xa)+ABS(yb-ya)

Travailler avec les fils savoir si un taquin se trouve dans une liste de noeuds.

############## LIBERER LA MEMOIRE !!!!!!!!!!! #######################


*/

int iVide;
int jVide;
int tabdist[MAX2];

/*Cases mal placées ------- oui en théorie c'était la mais je sais plus ce que j'ai fait, enfin si le chemin optimal modifié XD (distance)...*/
void truc_facile_ou_pas(char *t){
  int i,j,ii;
  int iNormal,jNormal;
  
  for(i=0, ii = 0 ; i<MAX2 ; i += MAX, ii++)
    {
      for(j=0 ; j<MAX ; j++)
        {
          if(t[i+j]!=0)
            {
              iNormal = (t[i+j]) >> 2;
              jNormal = (t[i+j]) & 3;
              tabdist[t[i+j]]=ABS(iNormal - ii) + ABS(jNormal-j);
              tabdist[0] += tabdist[t[i+j]];
            }
        }
    }
}

int dist(char *t, int pos){
  int ii,jj,i,j;
  ii = (t[pos]) >> 2;
  jj = (t[pos]) & 3;
  i=(pos) >> 2;
  j=(pos) & 3;
  return ABS(i - ii) + ABS(j-jj);
}

/*Affiche le Taquin non résolu XD */
void afficherTaquin(char *t){
  int i,j;
  printf("\n");
  for(i = 0 ; i<MAX2 ; i+=MAX){
    for(j=0; j<MAX ; j++){
      if((t[i+j]))
	printf(" %2d |",t[i+j]);
      else
	printf(" XX |");
    }
    printf("\n");
    for(j=0;j<MAX;j++){
      printf("-----");
    }
    printf("\n");
  }
  return;
}

/*Cest Jolie ... ou pas c'est moche , c'est très très brouillon ... je me demande vraiment si je suis pas là pour ralentir le processeur :D 
sinon cela modifie le "taquin" et vérifie si celui ci peut-être resolu (ce sert de la distance pour vérifier les noeuds)*/
void modifierTaquin(char *t, int dir){
  if(dir==H)
    {
      tabdist[0]-=tabdist[t[((iVide-1)<<2)+jVide]];
      t[(iVide<<2)+jVide] = t[((iVide-1)<<2)+jVide];
      t[((iVide-1)<<2)+jVide] = 0;
      tabdist[t[(iVide<<2)+jVide]]=dist(t,(iVide<<2)+jVide);
      tabdist[0]+=tabdist[t[(iVide<<2)+jVide]];
      iVide--;
    }
  else if (dir==G)
    {
      tabdist[0]-=tabdist[t[((iVide)<<2)+jVide-1]];
      t[(iVide<<2)+jVide] = t[(iVide<<2)+(jVide-1)];
      t[(iVide<<2)+(jVide-1)] = 0;
      tabdist[t[(iVide<<2)+jVide]]=dist(t,(iVide<<2)+jVide);
      tabdist[0]+=tabdist[t[(iVide<<2)+jVide]];
      jVide--;
    }
  else if (dir==D)
    {
      tabdist[0]-=tabdist[t[((iVide)<<2)+jVide+1]];
      t[(iVide<<2)+jVide] = t[(iVide<<2)+(jVide+1)];
      t[(iVide<<2)+(jVide+1)] = 0;
      tabdist[t[(iVide<<2)+jVide]]=dist(t,(iVide<<2)+jVide);
      tabdist[0]+=tabdist[t[(iVide<<2)+jVide]];
      jVide++;
    }
  else if(dir == B)
    {
      tabdist[0]-=tabdist[t[((iVide+1)<<2)+jVide]];
      t[(iVide<<2)+jVide] = t[((iVide+1)<<2)+jVide];
      t[((iVide+1)<<2)+jVide] = 0;
      tabdist[t[(iVide<<2)+jVide]]=dist(t,(iVide<<2)+jVide);
      tabdist[0]+=tabdist[t[(iVide<<2)+jVide]];
      iVide++;
    }
  else
    printf("*** ERRRRREEEEEUUUUUUURRRRR c'est pas possible frère ***%d\n",dir);
}

/* C'est lui le cerveau, il donne les ordres :D */
void solve(char *t,int seuil){
  char pile1[seuil<<2];
  int sp1=-1;
  char pile2[seuil];
  int sp2=-1;
  int nouveau_seuil=MAX_SEUIL;
  int posp=0;
  int cpt,dirsuiv=0,solved=0,h=0;
  if(seuil >= 81){ //78 limite si pas de chance (car très long... et surtout segmentation fault) si chance 80 arrive :D :D :D :D 
     return;
  sp1++;
  if (iVide > 0)
    pile1[(sp1<<2) + (posp++)] = H;
  if (jVide > 0)
    pile1[(sp1<<2) + (posp++)] = G;
  if (jVide < MAX-1)
    pile1[(sp1<<2) + (posp++)] = D;
  if (iVide < MAX-1)
    pile1[(sp1<<2) + (posp++)] = B;
  if(posp < MAX)
    pile1[(sp1<<2) + posp] = FIN;
  for(;(sp1 != FIN);){ //nouvelle postition
    for(cpt=0,dirsuiv=0;cpt<MAX;cpt++){
      if(pile1[(sp1<<2)+cpt]==FIN)
        break;
      if(pile1[(sp1<<2)+cpt]){
        dirsuiv=pile1[(sp1<<2)+cpt];//retire posibilité de la pile
        pile1[(sp1<<2)+cpt]=0;
        break;
      }
    }
    if(dirsuiv){
      sp2++;
      pile2[sp2]=dirsuiv;
      modifierTaquin(t,pile2[sp2]);
      if(!tabdist[0]){
        solved=1;
        break;
      }
      if((tabdist[0]+sp1+1)>seuil){
        if((nouveau_seuil==MAX_SEUIL)||((tabdist[0]+sp1+1)>nouveau_seuil)){
          nouveau_seuil=tabdist[0]+sp1+1;
        }
        modifierTaquin(t,REV - pile2[sp2]);
        sp2--;
      }
      else{
        posp=0;
        sp1++;
        if (iVide > 0)
          if(pile2[sp2]!=B)
            pile1[(sp1<<2)+(posp++)] = H;
        if (jVide > 0)
          if(pile2[sp2]!=D)
            pile1[(sp1<<2)+(posp++)] = G;
        if (jVide < MAX-1)
          if(pile2[sp2]!=G)
            pile1[(sp1<<2)+(posp++)] = D;
        if (iVide < MAX-1)
          if(pile2[sp2]!=H)
            pile1[(sp1<<2)+(posp++)] = B;
        if(posp < MAX)
          pile1[(sp1<<2)+posp] = FIN;
      }
    }
    else{
      if(sp2!=FIN){
        modifierTaquin(t,REV - pile2[sp2]);
        sp2--;
      }
      sp1--;
    }
  }
  if(solved){
    afficherTaquin(t);
    printf("Resolu en %d coups\n",sp2+1);
  }
  else{
    printf("profondeur ??!!?? := %d\n",nouveau_seuil);
    tabdist[0]=0;
    truc_facile_ou_pas(t);
    solve(t,nouveau_seuil);
  }
}

int main(void){
  char t[MAX2];
  int i=0;
  
  t[i++] = 15;//00
  t[i++] = 11;
  t[i++] = 8;
  t[i++] = 12;//03
  t[i++] = 14;//10
  t[i++] = 10;
  t[i++] = 9;
  t[i++] = 13;
  t[i++] = 2;
  t[i++] = 6;
  t[i++] = 1;
  t[i++] = 4;
  t[i++] = 3;
  t[i++] = 7;
  t[i++] = 5;
  t[i++] = 0;
  iVide = 3;
  jVide = 3;

 
  tabdist[0]=0;
  truc_facile_ou_pas(t);
  solve(t,tabdist[0]);

  return 0;
}
