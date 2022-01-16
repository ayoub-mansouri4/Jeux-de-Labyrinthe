#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include<conio.h>
#define INFINITY 9999

int MAX,noeudDepart; // MAX est le nombre des noeuds qu'on va l'utiliser dans la matrice d'adjacence
int m=21,p=21; //m,p sont les dimensions de la grille et m,p doit etre  impaire
               //on modelise cette grille par une matrice de type(m,p)
#define M 21   //  meme chose
#define P 21   // meme chose
int  Path[103*103];// tableau qui va prendre les noeud du plus court chemin
int temp_mat[M][P]; //  matrice temporaire qui modelise le labyrinthe et qui va contenir des cellule(des nbrs !=0 et !=-1) et des murs(des -1) et des murs cassés(des 0)
int ligneEntree,colonneEntree,positionEntree; // les caractéristiques de noeud de départ
// on considere une cellule comme un sommet

int printRandoms(int lower, int upper){  // cette fonction retourne un nombre aléatoire entre lower et upper
    return (rand()%(upper-lower+1))+lower;
}
void init_laby(int mat[m][p]){     // cette fonction initialise le labyrinthe (initialement le labyrinthe est une grille)
// la matrice doit contenir des -1 dans la 1ere ligne et dans la 1ere colonne et dans la derniere ligne et aussi dans la derniere colonne(comme un griage)
// mat[m][p] modelise le labyrinthe
    int i,j,k=1;
    for(i=0;i<m;i++){
        for(j=0;j<p;j++)
        {
            if(i%2==0||j%2==0) mat[i][j]=-1;   //construie des murs
            if(i%2==1 && j%2==1)                //construire des cellules
            {
                mat[i][j]=k;
                k++;
            }
        }
    }
}

void creer_laby(int mat[m][p]){
      srand(time(0));
      int xe,ye;  // dim du noued de départ
      // le noeud de départ doit etre dans 1 ere ligne ou dans le haut de la 1ere colonne
      do{
        xe=printRandoms(0,m/2);
        ye=printRandoms(0,p/2);
    }while((xe==0 && ye==0) || (xe==0 && ye==p-1) || xe%2==0 ||ye%2==0 ); // le noeud de départ ne doit pas etre les positions (0,0)... c'est à dire dans les bordues
     if(xe<=ye){      //si xe<=ye le noeud de départ(entrée) va etre dans la 1ere colonne
        ye=0;
        mat[xe][ye]=mat[xe][ye+1]; // le noeud d'entrée prend la valeur de l'élément d'indice (xe,1)
        ligneEntree=xe;
        colonneEntree=ye;
        positionEntree=1;  // donc on est dans la position 1
    }
    else if (xe>ye){ //si xe>ye le noeud de départ(entrée) va etre dans la 1ere ligne
        xe=0;
        mat[xe][ye]=mat[xe+1][ye];  // le noeud d'entrée prend la valeur de l'élément d'indice (1,ye)
        ligneEntree=xe;
        colonneEntree=ye;
        positionEntree=2; // // donc on est dans la position 1
    }

    mat[m-1][1]=mat[xe][ye]; // la 1ere sortie va prendre la valeur de l'entrée
    mat[m-2][p-1]=mat[xe][ye]; //la 2émé sortie va prendre la valeur de l'entrée

                    /*-----------------------------------------------------------------------------------------------------------------------------------*/

 // cette fonction verifie si tous les cellules ont la méme valeur de mat[1][1], si oui elle retourne 0,sinon elle retourne 1
      int is_finished(int mat[m][p]){
          int i,j;
	    for ( i = 1; i < m - 1; i += 2){
		    for ( j = 1; j < p - 1; j += 2){
			   if (mat[i][j] !=mat[1][1])
				  return 0;
		}
	}
     return 1;
   }



    int i,j;
    mat[m-2][p-1]=mat[m-1][1]=30;// les deux sortie doit egale à 30

    //cette boucle verifie si deux cellules adjacents sont différents entre eux
    //si oui ,elle va rendre ces deux cellules égaux elle va casser le murs qui existe entre eux(remplacer -1 par un 0)
   while (is_finished(mat)==0)
   {

            int x,y;
            do{
                x=printRandoms(1,m-2);
                y=printRandoms(1,p-2);
           }while(mat[x][y]!=-1);// prendre un mur aléatoirement(mat[x][y]==-1 signifie des éléments qui  sont  des murs)


          int cell1;
		  int cell2;
		  if(mat[x-1][y]==-1){// on verifie si on a dans une colonne des murs
		      cell1=mat[x][y-1]; //cellule1=l'élément qui exsite à gauche du mur
              cell2=mat[x][y+1];  //cellule2=l'élément qui exsite à droit du mur
		  }else{       // donc on est dans une ligne des murs
		      cell1=mat[x-1][y]; //cellule1=l'élément qui exsite en bas  du mur
              cell2=mat[x+1][y];  //cellule1=l'élément qui exsite en haut du mur
		  }
		  if(cell1!=cell2 && cell1!=0 && cell2!=0){ // si les 2 cellules sont différents entre eux et si les 2 cellules sont différents des murs cassés
               mat[x][y]=0;   // casser le mur (remplacer -1 par un 0)
		  }
		  for (i = 1; i < m-1; i += 2)
			{
				for (j = 1; j < p-1; j += 2)
				{
					if (mat[i][j] == cell2)
						mat[i][j] = cell1; // rendre ces deux cellules égaux
				}
			}
   }

}

// cette fonction affiche le labyrinthe
void afficher_laby(int mat[m][p],SDL_Window* window,SDL_Renderer *renderer){
    SDL_Rect Cell; // creer un rectangle
    Cell.h = 30; //  rectangle de longueur 30px
    Cell.w = 30; //  rectangle de largeur 30px

    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<m;j++){
            // dessiner un rectangle dans la position (i,j)
            Cell.x =j*30;
            Cell.y = i*30;
            if(mat[i][j]==-1){// afficher les murs  de couleur orange
                SDL_SetRenderDrawColor(renderer, 255,116,0,SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &Cell);
                SDL_RenderPresent(renderer);
            }
            else {// afficher les chemins  de couleur blanc
                 SDL_SetRenderDrawColor(renderer,255,255,255,SDL_ALPHA_OPAQUE);
                 SDL_RenderFillRect(renderer, &Cell);
                 SDL_RenderPresent(renderer);
        }
    }
    }
}
// cette fonctions compte le nombre des noeuds qui existent dans notre labyrinthe
void nbrNoeuds(int mat[m][p]){
    int i,j,noeuds=0;
     for(i=0;i<m;i++){
         for(j=0;j<p;j++){
            if(mat[i][j]!=-1 && mat[i][j]!=0) noeuds++; // si l'élément !=-1 et !=0 ==> on a un noeud
         }
     }
    MAX=noeuds;

}
#define max 103  // dans le cas d'un labyrinthe de taille m*p avec m=p=21 , le nombre des noeuds =103

int Matrice_Adja[max][max];// initialiser la matrice d'adjacence

int  *matrice_Adjacence(int mat[m][p]){
     int i,j,k=0,noeuds=0;

     int adj[MAX][MAX];// initialiser la matrice d'adjacence ,cest la méme matrice Matrice_Adja[max][max]
     for(i=0;i<MAX;i++){ // initialiser la matrice par des 0
          for(j=0;j<MAX;j++){
             adj[i][j]=0;
          }
      }
     //la matrice temporaire est identique à la matrice mat[m][p] sauf que chaque cellule prend un valeur unique entre[0,nombre des cellule]
     for(i=0;i<m;i++){
         for(j=0;j<p;j++){
             if(mat[i][j]!=0 && mat[i][j]!=-1){
                temp_mat[i][j]=k; // la cellule 1 va prendre 0 ,  la cellule 2 va prendre 1,la cellule 3 va prendre 2... juqu'à la fin(lorsqu'on parcourt tous les cellules)
                k++;
             }else temp_mat[i][j]=mat[i][j];
         }
     }

      int index1,index2;
    // verifier si deux sommets(cellules) sont adjacents .
      for(i=0;i<m;i++){
          for(j=0;j<p;j++){
              if(mat[i][j]==0 && mat[i-1][j]!=-1 && mat[i+1][j]!=0 && mat[i-1][j]==mat[i+1][j]) {// verifier si deux sommets(cellules) sont adjacents (1er cas).
                  index1 = temp_mat[i-1][j];// le 1er sommet est un inidce de la matrice d'adjacence
                  index2 = temp_mat[i+1][j] ;// le 2éme sommet est un inidce de la matrice d'adjacence tel que ces 2 sommets sonts adjacents
                  adj[index1][index2]=1;
                  adj[index2][index1]=1;
              }else if(mat[i][j]==0 && mat[i][j-1]!=-1 && mat[i][j+1]!=0 && mat[i][j-1]==mat[i][j+1]){// verifier si deux sommets(cellules) sont adjacents(2éme cas) .
                  index1 = temp_mat[i][j-1];// le 1er sommet est un inidce de la matrice d'adjacence
                  index2 = temp_mat[i][j+1] ;// le 2éme sommet est un inidce de la matrice d'adjacence tel que ces 2 sommets sonts adjacents
                  adj[index1][index2]=1;
                  adj[index2][index1]=1;
              }
          }
      }
      // traitement des cas particuliers
      index1=temp_mat[m-2][p-1]; // la 2eme sortie est un indice de la matrice d'adjacence
      index2=temp_mat[m-2][p-2]; // le sommet qui existe à gauche de la 2eme sortie ,il est adjacent à celle ci.
      adj[index1][index2]=1;
      adj[index2][index1]=1;

      index1=temp_mat[m-1][1]; // la 1ere sortie est un indice de la matrice d'adjacence
      index2=temp_mat[m-2][1]; // le sommet qui existe au-dessus de la 1ere sortie ,il est adjacent à celle ci.
      adj[index1][index2]=1;
      adj[index2][index1]=1;
      //determiner la position d'entree
      if(positionEntree==1){
          //ye=0 c'est à dire que l'entree existe dans la 1ere colonne
          //le sommet qui existe au-dessous de l'entee ,il est adjacent de celle ci
         index1=temp_mat[ligneEntree][colonneEntree];
         index2=temp_mat[ligneEntree][colonneEntree+1];
      }else if(positionEntree==2){
          //xe=0  c'est à dire que l'entree existe dans la 1ere ligne
          //le sommet qui existe à droite de l'entee ,il est adjacent de celle ci
          index1=temp_mat[ligneEntree][colonneEntree];
          index2=temp_mat[ligneEntree+1][colonneEntree];
      }
       noeudDepart=temp_mat[ligneEntree][colonneEntree];// noeudDepart on va l'utiliser la fonction dijkstra
       adj[index1][index2]=1;
       adj[index2][index1]=1;

        int *ptrAdj;
        ptrAdj=adj;
        for(i=0;i<MAX;i++){
            for(j=0;j<MAX;j++){
                 Matrice_Adja[i][j]=adj[i][j];// stocker les éléments de la matrice adj dans une autre matrice d'adjacence qui est une variable globale.
            }
        }
      return ptrAdj;
}
void dijkstra(int G[MAX][MAX],int n,int startnode,SDL_Window* window,SDL_Renderer *renderer)
{
    SDL_Rect rect;
    rect.h=30;
    rect.w=30;
	int cost[MAX][MAX],distance[MAX],pred[MAX];
	int visited[MAX],count,mindistance,nextnode,i,j;

	//pred[] stocke le prédécesseur de chaque nœud
	//count donne le nombre de nœuds vus jusqu'à présent
	//créer la matrice des coûts
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(G[i][j]==0)
				cost[i][j]=INFINITY;
			else
				cost[i][j]=G[i][j];

	//initialisation pred[],distance[] and visited[]
	for(i=0;i<n;i++){
		distance[i]=cost[startnode][i];
		pred[i]=startnode;
		visited[i]=0;
	}
	distance[startnode]=0;
	visited[startnode]=1;
	count=1;
	while(count<n-1)
	{
		mindistance=INFINITY;

		//nextnode donne le nœud à la distance minimale
		for(i=0;i<n;i++)
			if(distance[i]<mindistance&&!visited[i])
			{
				mindistance=distance[i];
				nextnode=i;
			}

			//vérifier si un meilleur chemin existe via nextnode
			visited[nextnode]=1;
			for(i=0;i<n;i++)
				if(!visited[i])
					if(mindistance+cost[nextnode][i]<distance[i])
					{
						distance[i]=mindistance+cost[nextnode][i];
						pred[i]=nextnode;
					}
		count++;
	}

	//afficher le chemin et la distance de chaque nœud
    int index1=0;
	for(i=0;i<n;i++){
		if(i!=startnode && (i==MAX-2 || i==MAX-1) ){ // afficher le chemin entre l'entree et chacune deux sorties
            Path[index1]=i; //le tableau Path va prendre les sommets du 1ere chemin et aussi du 2eme chemin ,mais ils sont inversés(less orties vers l'entree)
			j=i;
			do{
				j=pred[j];
                index1++;
                Path[index1]=j; //le tableau Path va prendre les sommets du 1ere chemin et aussi du 2eme chemin ,mais ils sont inversés(less orties vers l'entree)
			}while(j!=startnode);
	   }
    }
    index1++; // index1 est le nombre des sommets qui existent dans les deux chemins
    int TablPath[index1]; // TablPath va prendre les sommets des 2 chemins tel qu'ils sont rangés du sommet départ vers les deux sommets d'arrivés
    for(i=0;i<index1;i++){
        TablPath[i]=Path[index1-(i+1)];
    }
    // on considere une cellule comme un sommet
    //cette derniere partie afficheles 2 chemins qui sont trouvés
    for(int v=0;v<index1;v++){
        for(i=0;i<m;i++){
            for(j=0;j<p;j++){
               if(TablPath[v]==temp_mat[i][j] && TablPath[v]!=0) {// afficher en jaune les éléments de ces chemins(on determine la position de chaque élément)
                                rect.x =j*30;
                                rect.y =i*30;
                                SDL_SetRenderDrawColor(renderer, 255, 255, 0,SDL_ALPHA_OPAQUE);
                                SDL_RenderFillRect(renderer, &rect);
                                SDL_Delay(300);
                                SDL_RenderPresent(renderer);
                                if(temp_mat[i+2][j]==TablPath[v+1] && temp_mat[i+1][j]==0  ){
                                    // On a l'élément d'indices i et j est un sommet de chemin ,
                                    // et si l'élément d'indices i+2 et j est un sommet
                                    //alors on déduit que l'élément d'indices i+1 et j et doit être coloré(mur casse c'est à dire passage entre deux cellule)
                                        rect.x =j*30;
                                        rect.y =(i+1)*30;
                                        SDL_SetRenderDrawColor(renderer, 255, 255, 0,SDL_ALPHA_OPAQUE);
                                        SDL_RenderFillRect(renderer, &rect);
                                        SDL_Delay(100);
                                        SDL_RenderPresent(renderer);
                                    }
                                else if(temp_mat[i][j+2]==TablPath[v+1] && temp_mat[i][j+1]==0  ){
                                     // On a l'élément d'indices i et j est un sommet de chemin ,
                                    // et si l'élément d'indices i et j+2 est un sommet
                                    //alors on déduit que l'élément d'indices i et j+1 et doit être coloré(mur casse c'est à dire passage entre deux cellule)
                                        rect.x =(j+1)*30;
                                        rect.y =i*30;
                                        SDL_SetRenderDrawColor(renderer, 255, 255, 0,SDL_ALPHA_OPAQUE);
                                        SDL_RenderFillRect(renderer, &rect);
                                        SDL_Delay(100);
                                        SDL_RenderPresent(renderer);
                                    }
                                else if(temp_mat[i][j-2]==TablPath[v+1] && temp_mat[i][j-1]==0  ){
                                     // On a l'élément d'indices i et j est un sommet de chemin ,
                                    // et si l'élément d'indices i et j-2 est un sommet
                                    //alors on déduit que l'élément d'indices i et j-1 et doit être coloré(mur casse c'est à dire passage entre deux cellule)
                                        rect.x =(j-1)*30;
                                        rect.y =i*30;
                                        SDL_SetRenderDrawColor(renderer, 255, 255, 0,SDL_ALPHA_OPAQUE);
                                        SDL_RenderFillRect(renderer, &rect);
                                        SDL_Delay(100);
                                        SDL_RenderPresent(renderer);
                                    }
                                else if(temp_mat[i-2][j]==TablPath[v+1] && temp_mat[i-1][j]==0  ){
                                     // On a l'élément d'indices i et j est un sommet de chemin ,
                                    // et si l'élément d'indices i-2 et j est un sommet
                                    //alors on déduit que l'élément d'indices i-1 et j et doit être coloré(mur casse c'est à dire passage entre deux cellule)
                                        rect.x =j*30;
                                        rect.y =(i-1)*30;
                                        SDL_SetRenderDrawColor(renderer, 255, 255, 0,SDL_ALPHA_OPAQUE);
                                        SDL_RenderFillRect(renderer, &rect);
                                        SDL_Delay(100);
                                        SDL_RenderPresent(renderer);
                                    }
                         }
                    if(TablPath[v]==temp_mat[i][j] && TablPath[v]==0 && (i==0 || j==0)){
                        //afficher le noeud de depart(entree) en jaune s'il est dans la 1ere colonne ou bien la 1ere ligne
                                        rect.x =j*30;
                                        rect.y =i*30;
                                        SDL_SetRenderDrawColor(renderer, 255, 255, 0,SDL_ALPHA_OPAQUE);
                                        SDL_RenderFillRect(renderer, &rect);
                                        SDL_Delay(100);
                                        SDL_RenderPresent(renderer);
                 }
            }
        }
    }
}
int  main(int argc,char **argv){
   SDL_Window *window = NULL;   //creer une fenetre
   SDL_Renderer *renderer = NULL; //creer renderer
   SDL_Init(SDL_INIT_VIDEO);
   window = SDL_CreateWindow("Labyrinthe",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,m*30,p*30,0);//ceer une fenetre centree de taille m*30*p*30
   renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);//creer renderer d'indice -1

// on fait l'appel les fonctions qui existent au-dessusS
   srand(time(0));
   int mat[m][p]; //mat[m][p] signifie notre labyrinthe
   init_laby(mat);
   creer_laby(mat);
   afficher_laby(mat,window,renderer);
   nbrNoeuds(mat);
   matrice_Adjacence(mat);
   dijkstra(Matrice_Adja,MAX,noeudDepart,window,renderer);

   SDL_Delay(100000);
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();
   return 0;

}
