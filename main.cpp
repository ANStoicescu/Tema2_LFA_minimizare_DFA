#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
ifstream f("date.in");
int n;
int init;//punctul din care plecam
bool finalstate[100];//punctele finale le bifam cu 1 in vectorul fin
bool unreachablestate[100];// punctele care la care nu putem ajunge din starea initiala o sa le bifam cu 1 aici
int tab[100][2];//tabelul de tranzitii dintre fiecare punct
bool mat[100][100];//tabelul pe care il contruiesc sa aplic teorema Myhill-Nerode
int grfin[100],nrgrfin=0;
int tabfin[100][2];
bool grfinalestate[100];
bool grunreachablestate[100];
char grup[100][100];

void unreachablestate_finder(int i)
{
    unreachablestate[i]=0;
    if(unreachablestate[tab[i][0]]!=0)
        unreachablestate_finder(tab[i][0]);
    if(unreachablestate[tab[i][1]]!=0)
        unreachablestate_finder(tab[i][1]);
}

int main()
{
    f>>n;
    for(int i=0; i<n; i++)
    {
        int x,y;
        char c;
        f>>x>>y>>c;
        tab[x][c-97]=y;
    }
    f>>init;
    for(int i=0; i<=n/2; i++)
        unreachablestate[i]=1;
    unreachablestate[init]=0;
    unreachablestate_finder(init);
    for(int i=0;i<=n/2;i++)
        if(tab[i][0]==i && tab [i][1]==i)unreachablestate[i]=1;
    int x;
    while(f>>x)
        finalstate[x]=1;
    n/=2;
    for( int i=1; i<=n; i++)
        for(int j=1; j<=i-1; j++)
            if(finalstate[i]!=finalstate[j])
                mat[i][j]=mat[j][i]=1;// initializam tabelul pentru acest AFD
    bool gata=0;
    while(gata==0)
    {
        gata=1;
        for(int i=1; i<=n; i++)
        {
            for( int j=1; j<=i-1; j++)
                if(mat[i][j]==0)
                {
                    /*
                        Daca sunt perechi mat[i][j]==0 astfel incat mat[δ(i,0)][δ(j,0)] sau mat[δ(x,1)][δ(y,1)] nu sunt marcate
                    atunci marcam mat[i][j]=1 si gata=0 deoarece continuam ciclul pana nu mai facem modificari
                    */
                    if(mat[tab[i][0]][tab[j][0]] || mat[tab[i][1]][tab[j][1]])
                    {
                        mat[i][j]=mat[j][i]=1;
                        gata=0;
                    }
                }
        }
    }
    for(int i=1; i<=n; i++) //in grfin grupam punctele astfel incat sa obtinem grupele din AFD minimal
    {
        for( int j=1; j<=i-1; j++)
            if(mat[i][j]==0)
            {
                if(grfin[i]==0 && grfin[j]==0)
                {
                    nrgrfin++;
                    grfin[i]=grfin[j]=nrgrfin;
                }
                else if(grfin[j]==0)
                    grfin[j]=grfin[i];
                else
                    grfin[i]=grfin[j];
            }
    }
    for(int i=1; i<=n; i++)
        if(grfin[i]==0)
            grfin[i]=++nrgrfin;
    init=grfin[init];//gasim grupa care contine punctul initial si o declaram ca fiind initiala
    int nr=0;
    for(int i=1; i<=nrgrfin; i++)
    {
        stringstream ss;
        ss<<'(';
        for(int j=1; j<=n; j++)
        {
            if(grfin[j]==i)
            {
                tabfin[i][0]=grfin[tab[j][0]];
                tabfin[i][1]=grfin[tab[j][1]];
                ss<<j;
                if(finalstate[j]==1 && !grfinalestate[i])
                {
                    grfinalestate[i]=1;
                    nr++;
                }
                if(unreachablestate[j]==1)
                    grunreachablestate[i]=1;
            }
        }
        ss<<')';
        ss>>grup[i];
    }
    cout<<"AFD-ul minimal este: \n";
    for(int i=1; i<=nrgrfin; i++)
    {
        if(grunreachablestate[i]!=1)
        {
            if(grunreachablestate[tabfin[i][0]]!=1)
            cout<<grup[i]<<' '<<grup[tabfin[i][0]]<<" a"<<'\n';
            if(grunreachablestate[tabfin[i][1]]!=1)
            cout<<grup[i]<<' '<<grup[tabfin[i][1]]<<" b"<<'\n';
        }
    }
    cout<<"\nPunctul initial este grupa: "<<grup[init]<<'\n';
    if(nr>1)
        cout<<"Punctele finale sunt grupele: ";
    else
        cout<<"Punctul final este grupa: ";
    for(int i=0; i<=n; i++)
        if(grfinalestate[i]==1)
            cout<<grup[i]<<' ';
    cout<<'\n';
    return 0;
}
/*

date.in pentru exemplul din PDF:

12
1 4 a
1 3 b
2 1 a
2 4 b
3 5 a
3 6 b
4 5 a
4 6 b
5 5 a
5 6 b
6 6 a
6 6 b
1
3 4 5

date.in pentru un exemplu, cu un punct la care nu putem ajunge, plecand din starea initiala:

14
1 2 a
1 3 b
2 4 1
2 5 b
3 4 b
3 5 a
4 7 a
4 7 b
5 7 a
5 7 b
6 4 a
6 5 b
7 7 a
7 7 b
1
2 3 7
*/
