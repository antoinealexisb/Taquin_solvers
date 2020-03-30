//Importations
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//Constantes et define
#define G 1
#define D 2
#define H 3
#define B 4

//Structure
typedef struct taq1
{
	int cout;
	int strategie_simple;
	int cout_total;
	int **taq;
	int pos0l;
	int pos0c;
	char lettre;
}taquin;

typedef struct n
{
	taquin *t;
	struct n *suivant;
}noeud;

int nbl; //nombre de lignes
int nbc; //nombre de colonnes

/****
 Fonction permettant d'ajouter les lettres des mouvements dans un tableau (pour chaque noeud)
 @s (str[]) tableau rattaché au mouvement dans les noeuds
 @c (char) lettre à ajouter H,D,G,B
 */
//void append(char *s, char c)
//{
//	int len = strlen(s);
//	s[len] = c;
//	s[len+1] = '\0';
//}

/****
 Fonction qui à chaque noeud abandonné vide le tableau (Cette fonction ne sert à rien)
 @ (str[]) tableau
 */
//void suppr(char *s)
//{
//	int len = strlen(s);
//	s[len-1] = '\0';
//}

/****
 Fonction distance_manhattan qui compte pour chaque case, le nombre de cases qui la sépare 
 de sa case dans le taquin but, et qui en retourne la somme.
 https://fr.wikipedia.org/wiki/Distance_de_Manhattan
 @t (taquin *) le taquin
 #somme (int) retourne la somme 
 */
int distance_manhattan(taquin *t)
{
	int i,j;
	int cpt = 0;
	int somme = 0;
	int lig, col;
	for (i=0; i<nbl; i++)
	{
		for (j=0; j<nbc; j++)
		{
			cpt = t->taq[i][j];
			if (cpt != 0)
			{
				if (cpt != (i*nbc)+j)
				{
					lig = cpt/nbc;
					col = cpt%nbc;
					somme += ((lig>i)?(lig-i):(i-lig)) + ((col>j)?(col-j):(j-col));
				}
			}
		}
	}
	return somme;
}


/****
 Fonction charger qui permet de charger un fichier taquin mélanger et le fichier taquin but.
 @s (const char *) le fichier texte où se trouve le taquin
 #t (taquin *) retourne le taquin chargé
 */
taquin* charger(const char *s) {
	int n,col=0,lig=0,i;
	FILE *f;
	taquin *le_taquin = (taquin *)malloc(sizeof(taquin));
	if(!(f=fopen(s,"r")))
		perror("Erreur, le fichier n'est pas lisible : "),exit(1);

	if(fscanf(f,"taquin %d %d\n",&nbl,&nbc) != 2) //on prend le nombre de lignes et colonnes, après le mot taquin sinon erreur
		fprintf(stderr,"Format incorecte, mettre sous la forme exemple : \ntaquin 3 3\n0 1 2 -1\n3 4 5 -1\n6 7 8 -1\n\n"),exit(2);
	
	le_taquin->taq=(int **)malloc(sizeof(int *)*nbl); //On initialise le taquin
	for (i=0; i<nbl; i++)
	{
		le_taquin->taq[i] = (int *)malloc(sizeof(int)*nbc);
	}
	
	// On parcours toutes les cases du taquin dans le fichier texte
	while(fscanf(f,"%d",&n) != EOF) {
		if(n == -1) //Si on arrive à la fin de la ligne du taquin, on passe à la ligne suivante et on recommence
		{
			lig++;
			col=0;
		}
		else //on stocke la valeur des différentes cases dans la structure appropriées
		{
			if (!n)
			{
				le_taquin->pos0c = col;
				le_taquin->pos0l = lig;
			}
			le_taquin->taq[lig][col] = n;
			col++;
		}
	}

	le_taquin->cout = 0;
	
	le_taquin->strategie_simple = distance_manhattan(le_taquin); //ICIIICIIC
				
	le_taquin->cout_total = le_taquin->strategie_simple;
	
	fclose(f);
	return le_taquin;
}



/****
 Fonction qui permet d'afficher un taquin et le dernier mouvement de celui-ci
 @t (taquin *) le taquin à afficher
 */
void afficheResult(taquin *t)
{
	int i,k;
	for(i=0; i<nbl; i++) {
		for (k=0; k<nbc; k++)
		{
			printf("%d ", t->taq[i][k]);
		}
		printf("\n");
	}
	printf("\n dernier mouvement pour arriver au taquin but : %c\n\n",t->lettre);	
	printf("cout total : %d avec un cout de stratégie simple à : %d et avec un cout d'incrémentation à : %d\n",t->cout_total, t->strategie_simple, t->cout);		
}


/****
 Fonction qui permet d'afficher la combinaison des mouvements
 @t (taquin *) le taquin
 */
/*
void afficheResult2(taquin *t)
{
        int i = 0;
        while ((t->lettre[i]>='B') && (t->lettre[i]<='H')){
		printf("%c",t->lettre[i]);
                i++;}
        //for (int i=0; i<24; i++)
        //{
        //        printf(" %c ",t->lettre[i]);
        //}
}


*/

/****
 Fonction fils qui permet de deplace pere pour faire un fils (soit D,H,B,G) grâce au taquin donné en parametre
 @t (taquin *) le taquin à qui on calcule ses fils
 @pos (int)  G si on deplace la case vide à gauche
			 D si on deplace la case vide à droite
			 H si on deplace la case vide en haut
 			 B si on deplace la case vide en bas
 #tfils (taquin *) retourne le taquin fils que l'on souhaite, NULL si pas possible
 */
taquin *fils(taquin *t, int pos)
{
	int i,j;
	//creation du taquin fils, où fils aura les même case que père
	taquin *tfils = (taquin *)malloc(sizeof(taquin));
	tfils->taq=(int **)malloc(sizeof(int *)*nbl);
	for(i=0; i<nbl; i++) {		
		tfils->taq[i] = (int *)malloc(sizeof(int)*nbc);
		for (j=0; j<nbc; j++)
		{
			tfils->taq[i][j] = t->taq[i][j]; 
		}		
	}
	switch(pos) //modification du fils pour avoir celui-ci que l'on veut
	{
		case G :
		{
			if (t->pos0c-1 < 0) //Si on ne peut pas deplacer la case vide vers la gauche, on retourne NULL
				return NULL;
			tfils->taq[t->pos0l][t->pos0c] = tfils->taq[t->pos0l][t->pos0c-1];
			tfils->taq[t->pos0l][t->pos0c-1] = 0;
			tfils->pos0l = t->pos0l;
			tfils->pos0c = t->pos0c-1;
			//append(tfils, 'G');
			tfils->lettre = 'G';
			break;
		}

		case D :
		{
			if (t->pos0c+1 > (nbc-1))//pareille sur la droite
				return NULL;
			tfils->taq[t->pos0l][t->pos0c] = tfils->taq[t->pos0l][t->pos0c+1];
			tfils->taq[t->pos0l][t->pos0c+1] = 0;
			tfils->pos0l = t->pos0l;
			tfils->pos0c = t->pos0c+1;
			//append(tfils, 'D');
			tfils->lettre = 'D';
			break;
		}

		case H :
		{
			if (t->pos0l-1 < 0) //pareille sur le haut
				return NULL;
			tfils->taq[t->pos0l][t->pos0c] = tfils->taq[t->pos0l-1][t->pos0c];
			tfils->taq[t->pos0l-1][t->pos0c] = 0;
			tfils->pos0l = t->pos0l-1;
			tfils->pos0c = t->pos0c;
			//append(tfils, 'H');
			tfils->lettre = 'H';
			break;
		}

		case B :
		{
			if (t->pos0l+1 > (nbl-1)) //pareille sur le bas
				return NULL;
			tfils->taq[t->pos0l][t->pos0c] = tfils->taq[t->pos0l+1][t->pos0c];
			tfils->taq[t->pos0l+1][t->pos0c] = 0;
			tfils->pos0l = t->pos0l+1;
			tfils->pos0c = t->pos0c;
			//append(tfils, 'B');
			tfils->lettre = 'B';
			break;
		}
	}
	//On incrémente le nombre de coût, on calcule la distance et son coût total
	tfils->cout = t->cout+1;
	tfils->strategie_simple = distance_manhattan(tfils); // AJOUT stratégie simple
	tfils->cout_total = tfils->cout + tfils->strategie_simple;
	return tfils;
}

/****
 Fonction nextTaquin qui permet de choisir le prochain taquin dans la liste passée en paramètre.
 On prend le taquin qui se trouve dans le premier noeud de la liste (noeud **) et on l'enleve de la liste
 @liste (noeud **) la liste contenant des noeuds
 #res (taquin *) retourne le prochain taquin de la liste
 */
taquin *nextTaquin(noeud **liste)
{
	taquin *res = (*liste)->t;
	noeud *noe = *liste;
	*liste=(*liste)->suivant;
	free(noe);
	return res;
}

/****
 Fonction comparaisonTaquins qui compare deux taquins passés en paramètre.
 @taquin1 (taquin *) un taquin
 @taquin2 (taquin *) une autre taquin
 #return 1 si les taquin sont identiques
 */
int comparaisonTaquins(taquin *taquin1, taquin *taquin2)
{
	int i,j;
	for(i=0; i<nbl; i++) {
		for (j=0; j<nbc; j++)
		{
			if (!(taquin1->taq[i][j] == taquin2->taq[i][j]))
				return 0;
		}		
	}
	return 1;
}

/****
 Fonction qui permet de savoir si un taquin se trouve dans une liste de noeuds.
 @liste (noeud *) 
 @t (taquin *)
 #return 1 si le taquin se trouve dans la liste, 0 sinon.
 */
int compare(noeud *liste, taquin *t)
{
	while (liste)
	{
		if (comparaisonTaquins(liste->t,t))
		{
			return 1;
		}
		liste = liste->suivant;
	}
	return 0;
}

/****
 Fonction freeTaquin qui permet de libérer un taquin 
 @t (taquin *) le taquin que l'on doit libéré
 */
void freeTaquin(taquin *t)
{
	int i;
	for (i=0; i<nbl; i++)
		free(t->taq[i]);
	free(t->taq);
	free(t);   
}

/****
 Fonction libereMaListe qui permet de libérer tous les noeuds d'une liste sauf le taquin de
 départ et le taquin but.
 @liste (noeud *)
 @dep (taquin *) le taquin de départ
 @but (taquin $) le taquin but
 */
void libereMaListe(noeud *liste, taquin *dep, taquin *but)
{
	noeud *l;
	while(liste)
	{
		l = liste;
		if (!(comparaisonTaquins(l->t,dep)||comparaisonTaquins(l->t,but)))
			freeTaquin(l->t);
		liste = liste->suivant;
		free(l);
	}
	free(liste);
}


/****
 Fonction qui permet d'ajouter un taquin à ma liste de noeuds. On ajoutera le taquin à la liste
 en fonction de son cout total.
 @t (taquin *) le taquin à ajouter
 @liste (noeud **)
 */
void ajouteAMaListe(taquin *t, noeud **liste)
{
	noeud *save = NULL;
	noeud *temp = *liste;
	noeud *tete = (noeud *)malloc(sizeof(noeud)); //creation du nouveau noeud
	tete->t = t;
	tete->suivant = NULL;
	while (temp) //Tant que la place du nouveau taquin n'est pas trouvé on passe au suivant et on garde le noeud précédent en mémoire
	{
		if (t->cout_total <= (temp->t)->cout_total) //Si trouvé on sort
		{
			tete->suivant = temp;
			break;
		}
		save = temp; //sauvegarde du noeud
		temp = temp->suivant;		
	}
	if (save) //faire pointer le noeud précèdent sur le nouveau
		save->suivant = tete;
	else
		*liste = tete;
}


/****
 Fonction qui utilise le seuil donné en paramètre.
 @t (taquin *)
 @but (taquin *) le taquin but
 @seuil (int) le seuil qui ne doit pas être dépassé
 #return le nombre de coût qu'il faut pour résoudre le taquin si on tombe sur un taquin but, sinon
 	on retourne le nouveau seuil
 */
int fonctionSeuil(taquin *t, taquin *but,int seuil)
{
	int min = -2;
	int final;
	int ct;
	
	noeud *dejaParcourue = (noeud *)malloc(sizeof(noeud)); //liste des taquin parcourus
	noeud *liste = (noeud *)malloc(sizeof(noeud)); //liste en fonction du cout des taquins
	
	liste = NULL;
	dejaParcourue = NULL;
	taquin *tmp;
	taquin *e;

	ajouteAMaListe(t,&liste);
	while(liste != NULL)//si liste vide, on passe au prochain seuil
	{
		e = nextTaquin(&liste);//fils
		ajouteAMaListe(e,&dejaParcourue);
		if (comparaisonTaquins(e,but)) //si taquin fils correspond but alors c'est bon :D
		{
			final = (e->cout)*(-1); //multiplication par (-1) pour sortir du do{} while() parcours()
			afficheResult(e); //on affiche le resultat 
			//afficheResult2(e);
			//ON LIBERE LA MEMOIRE AA !!!!!!!!! 
			libereMaListe(dejaParcourue,t,but);
			libereMaListe(liste,t,but);
			freeTaquin(e);
			return final;
		}
		else 
		{
			//On prend le fils de e qui à déplacé la case vide 

			if((tmp = fils(e,G)))
			{
				if(!compare(dejaParcourue, tmp)) //deja parcourue on passe
				{
					ct = tmp->cout_total;
					if (ct <= seuil)//ajout dans la liste si le cout <= seuil que nous sommes
						ajouteAMaListe(tmp,&liste);
					else 
					{
						if (ct < min || min == -2)
							min = ct;
						freeTaquin(tmp);
					}
				}
			}
			if((tmp = fils(e,D)))
			{
				if(!compare(dejaParcourue, tmp))
				{
					ct = tmp->cout_total;
					if (ct <= seuil)
						ajouteAMaListe(tmp,&liste);
					else 
					{
						if (ct < min || min == -2)
							min = ct;
						freeTaquin(tmp);
					}
				}
			}
			if((tmp = fils(e,H)))
			{
				if(!compare(dejaParcourue, tmp))
				{
					ct = tmp->cout_total;
					if (ct <= seuil)
						ajouteAMaListe(tmp,&liste);
					else 
					{
						if (ct < min || min == -2)
							min = ct;
						freeTaquin(tmp);
					}
				}
			}
			if((tmp = fils(e,B)))
			{
				if(!compare(dejaParcourue, tmp))
				{
					ct = tmp->cout_total;
					if (ct <= seuil)
						ajouteAMaListe(tmp,&liste);
					else 
					{
						if (ct < min || min == -2)
							min = ct;
						freeTaquin(tmp);
					}
				}
			}
		}
	}
	libereMaListe(dejaParcourue,t,but);
	libereMaListe(liste,t,but);
	return min;
}

/****
 Fonction qui permet d'effectuer la résolution sur un taquin quelconque vers un taquin but.
 @depart (taquin *) le taquin de départ
 @but (taquin *) le taquin qui est resolu
 */
void parcours(taquin *depart,taquin *but)
{
	int seuil = depart->strategie_simple;
	do 
	{
		printf("seuil: %d\n",seuil);
		seuil = fonctionSeuil(depart,but,seuil); //Miaouuuuu 
		
	} while (seuil > 0);
	printf("\nrésultat : %d\n",seuil*(-1)); //affiche le nombre de coup comme il faut
}

int main(int argc, char **argv)
{
	taquin *t = NULL;//creation taquin à résoudre
	taquin *but = NULL;//creation taquin but
	
	if (argc != 3)
		perror("\n#######################################\nErreur dans les arguments !!!!!!!!!!!!\n"),exit(1);
	
	/* On charge les taquin de départ et but */
	t = charger(argv[1]);
	but = charger(argv[2]);
	parcours(t,but);
		
	freeTaquin(t);
	freeTaquin(but);
	return 0;
}

