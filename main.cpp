#include <iostream>
#include <string>
#include "ProfessorTurma.h"
#include <vector>
#include <fstream>

using namespace std;

int main(){
    string json;
    ifstream arquivo;
    
    arquivo.open("json.txt");

    while(arquivo.good()){
        string aux;
        getline(arquivo,aux);
        json+=aux+"\n";
    }
    
    ProfessorTurma otimizador;
    
    otimizador.carregarDados(json);

    string resultado = otimizador.solveCoin();

    cout << resultado << endl;
    
    return 0;
}