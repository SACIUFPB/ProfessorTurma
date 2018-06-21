#ifndef PROFESSORTURMA_H
#define PROFESSORTURMA_H

#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <sstream>
#include "/home/rodrigoramalho/Desktop/AgoraVai/SACI/Prof->Turma/SACI/UFFLP/UFFLP.h"
#include "/home/rodrigoramalho/Desktop/AgoraVai/SACI/Prof->Turma/SACI/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class ProfessorTurma{
    public:
        int numProf; 
        int numDis;
        
        vector<string> professores;
        vector<string> diciplina;
        vector<int> crd;
        vector<int> maxCrd;
        vector<int> minCrd;
        vector<vector<int>> pref;

        bool **choque;
        int procuraIDdadiciplinaPeloNome(string jt);
        void carregarDados(string transformarNoJson);
        string solveCoin();
};


/*extern "C" { 
    string RecebeDados(string recebeDoJson);
}*/


#endif
