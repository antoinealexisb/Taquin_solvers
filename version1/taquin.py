import random
import copy

class Taquin:
    '''Classe qui modélise le jeu du taquin.
    Par défaut, le taquin est construit en dimension 4 x 4.
    '''

    def __init__(self,dim=4,gdNbre=80):
        '''Taquin, int, int -> Taquin
        construit un taquin de dim lignes x dim colonnes.
        gdNbre est le nombre de mouvements aléatoires pour
        initialiser une partie.
        '''
        assert dim > 1
        self.__plateau = []
        self.__dim = dim
        self.__dic=[]
        for i in range(dim):
            ligne = []
            for j in range(dim):
                ligne.append((i*dim)+j+1)
            self.__plateau.append(ligne)
        self.__plateau[-1][-1] = -1
        self.__coords_vide = (self.__dim-1, self.__dim-1)
        self.__gdNbre = gdNbre
        

    def est_valide(self,lig,col):
        '''Taquin, int, int -> boolean
        teste si lig et col sont des coordonnées valides.
        '''
        return 0 <= lig < self.__dim and 0 <= col < self.__dim
        
    def est_vide(self,lig,col):
        '''Taquin, int, int -> boolean
        teste si lig et col sont des coordonnées du vide.
        '''
        return self.__coords_vide == (lig, col)
        

    def __str__(self):
        '''Taquin -> str
        retourne le plateau sous forme texte.
        '''
        mess=''
        for i in range(self.__dim):
            mess += self.ligne_traits()
            mess += self.ligne_plateau(i)
        mess += self.ligne_traits()
        return mess


    def ligne_traits(self) :
        '''Taquin -> str
        retourne une ligne de traits.
        '''
        res = ''
        for i in range(self.__dim) :
            res += '+--'
        res += '+\n'
        return res

    def ligne_plateau(self,lig) :
        '''Taquin, int -> str
        retourne la lig-ieme ligne du plateau 
        sous forme textuelle.
        '''
        res = ''
        for i in range(self.__dim) :
            res += '|'
            if self.__plateau[lig][i] == -1 :
                res += "  "
            elif self.__plateau[lig][i] < 10 :
                res += " "+str(self.__plateau[lig][i])
            else :
                res += str(self.__plateau[lig][i])
        res +="|\n"
        return res

    
    
    def bouge_case(self,val):
        '''Taquin, int -> boolean
        bouge le palet qui contient la valeur val 
        si c'est possible et retourne Vrai.
        si le mouvement n'a pas pu avoir lieu, 
        retourne Faux. 
        '''
        lig,col = self.coords(val)
        if (lig,col) == (-1,-1) :
            return False
        self.permute_case_vide(lig,col)
        return True
        

    def permute_case_vide(self,lig,col):
        '''Taquin, int -> boolean
        permute le palet (lig,col) avec le vide
        '''
        l_vide, c_vide = self.__coords_vide
        self.__plateau[l_vide][c_vide] = self.__plateau[lig][col]
        self.__plateau[lig][col] = -1
        self.__coords_vide = lig,col

    def voisins_vide(self):
        '''Taquin -> list((int,int))
        retourne la liste des coordonnees des voisins 
        du vide.
        '''
        res = []
        l_vide, c_vide = self.__coords_vide
        if l_vide > 0 :
            res.append((l_vide-1,c_vide))
        if l_vide < self.__dim - 1 :
            res.append((l_vide+1,c_vide))
        if c_vide > 0 :
            res.append((l_vide,c_vide-1))
        if c_vide < self.__dim - 1 :
            res.append((l_vide,c_vide+1))
        return res


    def coords(self,val):
        '''Taquin, int -> (int, int)
        retourne les coordonnées de val si val est un voisin du vide,
        (-1,-1) sinon.
        '''
        voisins_valides = self.voisins_vide()
        for (lig,col) in voisins_valides :
            if self.__plateau[lig][col] == val :
                return lig,col
        return -1,-1


    def partie_finie(self) :
        '''Taquin -> boolean
        detecte si la partie est finie.
        '''
        for i in range(self.__dim):
            for j in range(self.__dim):
                if self.__plateau[i][j] != (i*self.__dim)+j+1 and not self.est_vide(i,j):
                    return False
        return True


    def reinit(self):
        '''Taquin (inout) -> None
        initialise une partie pour jouer.
        '''
        dic=[]
        #testeur=str([['X', 'X', 'X', 3], ['X', 'X', 'X', 7], ['X', 'X', 'X', 11], [12, 13, 14, 15]])
        testeur=str([[15, 11, 'X', 12], [14, 'X', 'X', 13], ['X', 'X', 'X', 'X'], [3, 7, 'X', 'X']])
        #testeur=[['X', 'X', 'X', 3], ['X', 'X', 'X', 7], ['X', 'X', 'X', 11], [12, 13, 14, 15]]
        tab2=[]
        k=0
        truc=0
        while tab2!=testeur and self.__gdNbre<81: #self.__plateau!= testeur:
            while k<=(2**self.__gdNbre)*self.__dim and tab2!=testeur:
                self.__plateau=[['X', 'X', 'X', 3], ['X', 'X', 'X', 7], ['X', 'X', 'X', 11], [12, 13, 14, 15]]
                self.__coords_vide = (0,0)
                ind_anc = (0,0)
                for i in range (self.__gdNbre):
                    candidats = self.voisins_vide()
                    ind = random.randint(0,len(candidats)-1)
                    while candidats[ind] == ind_anc:
                        ind = random.randint(0,len(candidats)-1)
                    ind_anc = self.__coords_vide
                    self.permute_case_vide(candidats[ind][0],candidats[ind][1])
                #print(tab2, ' => ', testeur)
                tab2=str(copy.deepcopy(self.__plateau)).replace('-1',"'X'")
                #tab2[0][0],tab2[0][1],tab2[0][2],tab2[1][0],tab2[1][1],tab2[1][2],tab2[2][0],tab2[2][1],tab2[2][2]='X','X','X','X','X','X','X','X','X'
                #print(tab2)
                #print(testeur)
                #print(self.__plateau)
                if tab2=="[[15, 11, 'X', 12], [14, 'X', 'X', 13], ['X', 'X', 'X', 'X'], [3, 7, 'X', 'X']]":#self.__plateau not in dic:#tab2==testeur:
                    #dic.append(self.__plateau)
                    k+=1
                    #print((dic))
                    print(self.__gdNbre, 'whouaaaaa', self.__plateau)
                #elif truc>500:
                    #k=100000
                    #k+=(self.__gdNbre*self.__dim)**2
                    #truc=0
                #else:
                    #truc+=1
            k=0
            truc=0
            self.__gdNbre+=1
            dic=[]
            print(self.__gdNbre)
        print(self.__plateau, self.__gdNbre-1)
			
			

    def dim(self):
        ''' self -> None.
		retourne la dimension du taquin '''
        return self.__dim
    
    def get(self,lig,col):
        '''self -> None.
		retourne la valeur du plateau en position(lig,col)'''
        return self.__plateau[lig][col]
        
        
### fin de la classe Taquin
def main() :
    '''c est le main '''
    taq = Taquin()
    print(taq)
    taq.reinit()
    while not taq.partie_finie():
        print(taq)
        choix = int(input("Quelle valeur voulez-vous bouger ? "))
        res = taq.bouge_case(choix)
        if not res :
            print("Non, ce n'est pas possible avec cette valeur.")
    print(taq)
    print("Partie finie. Bravo")


### script principal
if __name__ == '__main__' :
    main()

