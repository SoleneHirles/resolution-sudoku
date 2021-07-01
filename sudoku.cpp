#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef vector<vector<int>> Grille;

typedef struct {
  size_t lig;
  size_t col;
} Indices;

void imprime(const Grille &g);	// deja ecrite, voir en fin de fichier.


/* Renvoie true ssi la grille a la bonne forme et ne contient que des
 * valeurs comprises entre 0 et 9. On ne cherche pas a verifier les autres
 * contraintes.
 */
bool bienFormee(const Grille &g) {
    if (not(g.size()==9)){
        return false;
    }else{
        size_t i=0;
        for (i; i<g.size();i=i+1){
            if (not(g.at(i).size()==9)){
                return false;
            }
            for(size_t j=0; j<g.at(i).size();j=j+1){
                if((g.at(i).at(j)>9) or (g.at(i).at(j)<0)){
                    return false;
                }
            }
        }
    }
  return true;
}


/* Toutes les fonctions ci-apres supposent que la grille est bien formee */

/* Renvoie le nombre de cases vides dans g */
size_t nbVides(const Grille &g) {
  size_t cpt = 0;
  for(size_t i=0;i<g.size(); i=i+1){
    for(size_t j=0; j<g.at(i).size(); j=j+1){
        if(g.at(i).at(j)==0){
            cpt=cpt+1;
        }
    }
  }
  return cpt;
}


/* renvoie les indices du coin superieur gauche du sous-carre qui contient
 * la case dont on passe les indices en argument
 */
Indices supGauche(Indices ij) {
    while (not((ij.lig==0) or (ij.lig==3) or (ij.lig==6))){
               ij.lig=ij.lig-1;
        }
        while (not((ij.col==0) or (ij.col==3) or (ij.col==6))){
               ij.col=ij.col-1;
        }
  return ij;
}


/* remplit v avec le contenu de la ligne d'indice il.
 * Comme v n'est pas forcement vide, ne pas oublier de faire v.clear()
 * avant de mettre des choses dedans.
*/
void LtoV(const Grille &g, size_t il, vector<int> &v) {
  v.clear();
  for(size_t j=0; j<g.at(il).size();j=j+1){
    v.push_back(g.at(il).at(j));
  }
}


/* remplit v avec le contenu de la colonne d'indice ic */
void CtoV(const Grille &g, size_t ic, vector<int> &v) {
    v.clear();
    for(size_t j=0; j<g.size();j=j+1){
        v.push_back(g.at(j).at(ic));
    }
}


/* remplit v avec le contenu du sous-tableau 3*3 dont le coin superieur
 * gauche est la case dont on passe les indices
 */
void BtoV(const Grille g, Indices ind, vector<int> &v) {
    v.clear();
    for(size_t i=0; i<3; i=i+1){
        for(size_t j=0; j<3; j=j+1){
            v.push_back(g.at(ind.lig+i).at(ind.col+j));
        }
    }
}


/* renvoie true si dans v chaque valeur de 1 à 9 apparait au plus 1 fois */
bool VOk(const vector<int> &v) {
  for(size_t i=1; i<10; i=i+1){
        size_t compte=0;
    for(size_t j=0; j<v.size(); j=j+1){
        if (v.at(j)==i){
            compte=compte+1;
        }
    }
    if(compte>1){
        return false;
    }
  }
  return true;
}

/* A l'aide des fonctions précédentes, Verifie si la grille est valide:
 * chaque valeur de 1 à 9 apparait au plus une fois dans chaque ligne,
 * colonne et sous-tableau. La grille est supposee bien formee.
 */
bool valide(const Grille &g) {
    vector<int> v;
    bool res=true;
    size_t i=0;
    while((i<9)and(res)){ //vérification ligne
        LtoV(g,i,v);
        res=VOk(v);
        i=i+1;
    }
    if (not(res)){
        return res;
    }
    i=0;
    while((i<9)and(res)){ //vérification colonne
        CtoV(g,i,v);
        res=VOk(v);
        i=i+1;
    }
    if (not(res)){
        return res;
    }
    Indices car;
    for(size_t n=0;n<7;n=n+3){ //Vérification des carrées
            car.lig=n;
        for(size_t j=0;j<7;j=j+3){
            car.col=j;
            BtoV(g,car,v);
            if(not(VOk(v))){
                return false;
            }
        }
    }
    return true;
}


/* renvoie le vecteur des valeurs disponibles pour la case d'indices (il, ic)
 * compte-tenu des valeurs placées sur sa ligne, colonne et son sous-tableau
 */
vector<int> possibles(const Grille &g, Indices ind) {
  vector<int> res; // vecteur vide par defaut
  res.clear();
  vector<int> v;
  int i=1;
  while(i<10){
    LtoV(g,ind.lig,v);
    size_t j=0;
    while((j<v.size())and (v.at(j)!=i)){j=j+1;}; //ligne
    if(j>=v.size()){ //i n'a pas été trouvé
        j=0;
        CtoV(g,ind.col,v);
        while((j<v.size())and (v.at(j)!=i)){j=j+1;}; //colonne
        if(j>=v.size()){ //i n'a pas été trouvé
            j=0;
            BtoV(g,supGauche(ind),v);
            while((j<v.size())and (v.at(j)!=i)){j=j+1;};//carrée
                if(j>=v.size()){ //i n'a pas été trouvé
                    res.push_back(i); // i est donc une valeur possible
                }
        }
    }
    i=i+1;
   }
  return res;
}


/* Renvoie un vecteur avec les indices de toutes les cases vides du sous-tableau
 * contenant la case dont on passe les indices en argument
 */
vector<Indices> casesVides(const Grille &g, Indices ind) {
  vector<Indices> v; // vide par defaut.
  Indices car=supGauche(ind);
  for(size_t i=0; i<3; i=i+1){
    for(size_t j=0; j<3; j=j+1){
        if(g.at(car.lig+i).at(car.col+j)==0){
            Indices sol;
            sol.lig=car.lig+i;
            sol.col=car.col+j;
            v.push_back(sol);
        }
    }
  }
  return v;
}

/* Fournir des entiers (positifs ou nuls) ci-dessous, sinon la fonction
 * boucle. On verra comment faire plus robuste dans un autre TP.
 *
 * Rappel: les indices des vecteurs sont decales de 1 par rapport aux indices
 * vus par l'utilisateur
 * Retourne true ssi l'utilisateuir a fourni une suggestion (validee)
 *
 * Fonction partiellement ecrite, pour eviter que vous coinciez eventuellement
 * sur des pbs de lecture au clavier
 */
bool userSuggest(Grille &g) {
  size_t val, ilu, jlu;
  while (true) { // sortie via return dans le corps de boucle
    cout << "Suggerez une valeur (0 si pas de suggestion): ";
    cin >> val;
    if (val == 0) { return false; }
    else if (val < 0 or val > 9) {
      cout << "Valeur incorrecte: " << val << endl;
    } else {
      cout << "Donnez les indices de la case: i j = "; // ex: 5 5
      cin >> ilu >> jlu;
      bool res = false;
      if((ilu>0) and (ilu<10) and (jlu>0) and (jlu<10)){ //vérification de l'existence des indices
        if(g.at(ilu-1).at(jlu-1)==0){ //la case est vide
            g.at(ilu-1).at(jlu-1)=val;
            if (not(valide(g))){
                g.at(ilu-1).at(jlu-1)=0;
            }else{
                res=true;
            }

        }
      }
      return res;
    }
  }
}


/* Parcourt une fois la grille en remplissant automatiquement toutes les cases
 * pour lesquelles il y a une seule valeur possible.
 * Renvoie le nombre de cases qu'on vient de remplir.
 */
size_t uneValeur(Grille &g) {
  size_t cpt = 0;
  for(size_t i=0; i<g.size(); i=i+1){
    for(size_t j=0; j<g.at(i).size(); j=j+1){
        if (g.at(i).at(j)==0){
            Indices ind;
            ind.lig=i;
            ind.col=j;
            vector <int> v=possibles(g,ind);
            if(v.size()==1){
                cpt=cpt+1;
                g.at(i).at(j)=v.at(0);
            }
        }
    }
  }
  return cpt;
}


/* Parcourt une fois la grille en remplissant automatiquement toutes les cases
 * pour lesquelles parmi les valeurs potentielles pour cette case il y en a
 * une dont c'est la seule destination possible (la mettre ailleurs donnerait
 * une grille invalide).
 * Renvoie le nombre de cases qu'on vient de remplir.
 */
size_t unePosition(Grille &g) {
  size_t cpt = 0;
  for(size_t i=0; i<g.size(); i=i+1){
    for(size_t j=0; j<g.at(i).size(); j=j+1){
        if (g.at(i).at(j)==0){
            Indices ind;
            ind.lig=i;
            ind.col=j;
            vector <int> v=possibles(g,ind);
            vector <Indices> k=casesVides(g,ind);
            size_t obj=0;
            while (obj<v.size() and g.at(i).at(j)==0){
                bool trouve=false;
                for(size_t n=0; n<k.size(); n=n+1){
                    if((k.at(n).lig==ind.lig)and((k.at(n).col==ind.col))){ //on enlève les indices de la case dans le tableau de case vides (évite de comparer le vecteur possibilité de ind à lui-même)
                        continue;
                    }
                    vector<int> pos=possibles(g,k.at(n));
                    for(size_t m=0; m<pos.size();m=m+1){
                        if(pos.at(m)==v.at(obj)){
                            trouve=true;
                        }
                    }
                }
                if(not(trouve)){
                    g.at(i).at(j)=v.at(obj);
                    cpt=cpt+1;
                }
                obj=obj+1;
            }
        }
    }
  }
  return cpt;
}


/* Enchaine les boucles de resolution. A chaque tour, une seule strategie
 * est mise en oeuvre et on essaye de l'appliquer sur toutes les cases vides
 * avant de retourner dans la boucle de resolution.
 * aFaire est le nombre de cases vides au demarrage de la simulation.
 */
void automatique(Grille &g, string name, size_t aFaire) {
  size_t tour = 0;
  bool echec = false;
  cout << "Debut du traitement de la grille " << name << endl;

  /* Enlever le commentaire ci-dessous quand la fonction nbVides, sinon
   * on sort tout de suite de la boucle.
   * ne rendra plus sytematiquement 0 !
   */
  while (not echec and aFaire!= 0) {
    size_t cpt = 0; // cpt de cases remplies par la strategie courante
    tour += 1;
    cout << "Tour numero " << tour << endl;
    imprime(g);
    size_t c=uneValeur(g);
    if (c==0){
        c=unePosition(g);
        if(c==0){
            if(userSuggest(g)){
                c=1;
            }
        }

    }
    cpt=c+cpt;
    aFaire=aFaire-cpt;
    if (cpt == 0) {
      cout << "Aucune strategie n'a marche, on abandonne\n";
      echec = true;
    }
  }
  if (echec) {
    cout << "Grille " << name << ": Echec de la resolution\n\n";
  } else {
    imprime(g);
    if (valide(g)) {
      cout << "Grille " << name << " resolue\n\n";
    } else {
      // N'est pas censé arriver. On quitte definitivement le programme
      cerr << "Erreur interne" << endl;
      exit(1);
    }
  }
}


void joue(Grille &g, string name) {
  imprime(g);
  if (not bienFormee(g)) {
    cerr << "Grille " << name << " mal formee" << endl;
  }
  else if (not valide(g)) {
   cerr << "Grille " << name << " incoherente" << endl;
  }
  else {
    size_t aFaire = nbVides(g);
    automatique(g, name, aFaire);
  }
}


int main() {
  Grille g2 = { { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
                { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
                { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
                { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
                { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
                { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
                { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
                { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
                { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
  };
  Grille g3 = { { 0, 2, 7, 0, 6, 5, 0, 4, 9 },
		{ 3, 5, 0, 0, 0, 9, 0, 0, 8 },
		{ 8, 9, 0, 0, 0, 0, 0, 0, 0 },
		{ 2, 0, 0, 0, 9, 0, 0, 0, 0 },
		{ 0, 0, 0, 6, 8, 7, 0, 0, 0 },
		{ 0, 0, 0, 0, 3, 0, 0, 0, 5 },
		{ 0, 0, 0, 0, 0, 0, 0, 6, 3 },
		{ 7, 0, 0, 2, 0, 0, 0, 8, 4 },
		{ 4, 8, 0, 9, 7, 0, 1, 5, 0}
  };
  Grille g4 = { { 8, 0, 0, 0, 0, 0, 1, 0, 0 },
		{ 0, 3, 4, 0, 2, 0, 6, 0, 0 },
		{ 0, 9, 0, 4, 0, 0, 0, 0, 2 },
		{ 5, 1, 0, 0, 4, 2, 9, 6, 0 },
		{ 0, 0, 0, 0, 6, 0, 0, 0, 0 },
		{ 0, 6, 7, 1, 5, 0, 0, 2, 3 },
		{ 9, 0, 0, 0, 0, 4, 0, 3, 0 },
		{ 0, 0, 2, 0, 1, 0, 7, 4, 0 },
		{ 0, 0, 8, 0, 0, 0, 0, 0, 1 }
  };
  // accepte au moins deux solutions: une avec 6 dans la case du centre
  // l'autre avec 5 dans la case du centre
  // La premiere solution demande moins de cycles.
  // Si on donne la valeur 6 pour la case (3, 3) on a de nouveau besoin
  // de donner une suggestion plus tard.
  Grille g5 = { { 0, 0, 8, 0, 0, 0, 2, 0, 0 },
		{ 0, 0, 4, 3, 0, 0, 0, 0, 1 },
		{ 0, 5, 0, 0, 0, 7, 0, 8, 9 },
		{ 0, 0, 5, 7, 4, 0, 0, 0, 2 },
		{ 0, 0, 0, 1, 0, 3, 0, 0, 0 },
		{ 4, 0, 0, 0, 2, 0, 6, 0, 0 },
		{ 2, 6, 0, 5, 0, 0, 0, 7, 0 },
		{ 5, 0, 0, 0, 0, 2, 9, 0, 0 },
		{ 0, 0, 1, 0, 0, 0, 5, 0, 0 }
  };
  // Les 3 premieres grilles ci-dessous se resolvent en n'utilisant
  // que la strategie uneValeur
  joue(g2, "g2");
  joue(g3, "g3");
  joue(g4, "g4");
  // Celle-la demande la strategie unePosition et une fois (au moins)
  // l'aide de l'utiiisateur. Si on ne donne pas l'aide au bon endroit
  // on peut avoir debsoin d'une autre suggestion ulterieurement
  joue(g5, "g5");
  return 0;
}

		// Fonctions fournies, rien à modifier a priori.

/* Imprime une ligne de séparation formée du caractère passé en argument */
void imprimeL(char c) {
  cout << ' ';
  for(size_t i = 0; i < 9; i += 1) { cout << c << c << c << c; }
  cout << endl;
}

/* suppose la grille bien formee: 9 lignes de 9 colonnes de chiffres de 0 à 9 */
void imprime(const Grille &g) {
  for(size_t i = 0; i < 9; i += 1) {
    imprimeL(i % 3 == 0 ? '+' : '-'); // ligne d'en-tete
    for(size_t j = 0; j < 9; j += 1) {
      char c = '0' + g.at(i).at(j);
      cout << (j % 3 == 0 ? '+' : '|') << ' ' << c << ' ';
    }
    cout << "+" << endl;
  }
  imprimeL('+'); // ligne finale du bas
  cout << endl;
}

