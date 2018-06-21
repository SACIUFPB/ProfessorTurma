#include "ProfessorTurma.h"

int ProfessorTurma::procuraIDdadiciplinaPeloNome(string jt){
    for(int i = 0; i<diciplina.size();i++){
        if(diciplina[i].compare(jt))
            return i;
    }
}

void ProfessorTurma::carregarDados( string transformarDoJson){

    auto ObjetoEmJson = json::parse(transformarDoJson);
    //FOR que vai iterar no json, ver a quantidade de cadeiras e a quantidade de professores
    //itera no json e cria um array de professores, toda vez que voce achar um nome, você vai dar um append no array de professores
    //professores [numero de professores]
    //criar um array de professores por turmas
    vector<int> quantidadeDeCreditosdiciplina;
    vector<int> quantidadeDeCreditosMaximo;
    vector<int> quantidadeDeCreditosMinimos;



    for (json::iterator it = ObjetoEmJson["docente"].begin(); it != ObjetoEmJson["docente"].end(); ++it){
        professores.push_back((*it)["name"].get<string>());
        quantidadeDeCreditosdiciplina.push_back((*it)["maxCreditos"].get<int>());
        quantidadeDeCreditosdiciplina.push_back((*it)["minCreditos"].get<int>());
    }

    for (json::iterator it = ObjetoEmJson["diciplina"].begin(); it != ObjetoEmJson["diciplina"].end(); ++it){
        diciplina.push_back((*it)["nome"].get<string>());
        quantidadeDeCreditosdiciplina.push_back((*it)["creditosDia"].get<int>());
    }

    vector<vector<int>> professorXdiciplina (professores.size(), vector<int>(diciplina.size(), 0));
    int i = 0;
    
    for(json::iterator it = ObjetoEmJson["docente"].begin(); it != ObjetoEmJson["docente"].end(); ++it){
        int j;
        for(json::iterator jt = (*it)["select1"].begin(); jt != (*it)["select1"].end(); ++jt){
            j = procuraIDdadiciplinaPeloNome((*jt).get<string>());
            professorXdiciplina[i][j] = 3;
        }
        
        for(json::iterator jt = (*it)["select2"].begin(); jt != (*it)["select2"].end(); ++jt){
            j = procuraIDdadiciplinaPeloNome((*jt).get<string>());
            professorXdiciplina[i][j] = 2;
        }
        
        for(json::iterator jt = (*it)["select3"].begin(); jt != (*it)["select3"].end(); ++jt){
            j = procuraIDdadiciplinaPeloNome((*jt).get<string>());
            professorXdiciplina[i][j] = 1;
        }
        i++;
    }
    
    // for(int i = 0; i<professores.size(); i++){
    //     for(int j = 0; j<diciplina.size(); j++){
    //         cout << professorXdiciplina[i][j]<< endl;
    //      }
        
    // }

    this->numProf = professores.size(); 
    this->numDis = diciplina.size();
    this->crd = quantidadeDeCreditosdiciplina;
    this->maxCrd = quantidadeDeCreditosMaximo;
    this->minCrd = quantidadeDeCreditosMinimos;
    this->pref = professorXdiciplina;              
    this->choque = choque;
}

string ProfessorTurma::solveCoin()
{

   // Cria problema

   UFFProblem* prob = UFFLP_CreateProblem();


   // Cria variaveis X

	string varName;
  	stringstream s;
	for (int i = 0; i < numProf; i++) {
		for (int j = 0; j < numDis; j++) {
			if (pref[i][j] > 0) {
				s.clear();
				s << "X(" << i << "," << j << ")";
				s >> varName;
		        UFFLP_AddVariable(prob, (char*)varName.c_str(), 0.0, 1.0, pref[i][j], UFFLP_Binary);
			}
		}
	}

   // Cria variaveis Y

	// for (int i = 0; i < numPrepostos; i++) {
	// 	for (int j = 0; j < numAudiencias; j++) {
	// 		if (prepostoPode[i][j]) {
	// 			s.clear();
	// 			s << "Y(" << i << "," << j << ")";
	// 			s >> varName;
	// 	      UFFLP_AddVariable(prob, (char*)varName.c_str(), 0.0, 1.0, tempoDeslocamento[i][j], UFFLP_Binary);
	// 		}
	// 	}
	// }
    

    string consName;

	// Todas as audiencias que precisam de advogados devem ser atendidas por um deles

	for (int j = 0; j < numDis; j++) {

        s.clear();
        s << "c1_" << j;
        s >> consName;

		for (int i = 0; i < numProf; i++) {
			if (pref[i][j] > 0) {
                s.clear();
                s << "X(" << i << "," << j << ")";
                s >> varName;
   	            UFFLP_SetCoefficient( prob, (char*)consName.c_str(),(char*)varName.c_str(), 1.0);
			}
		}
	    UFFLP_AddConstraint( prob, (char*)consName.c_str(), 1.0, UFFLP_Equal);		

	}



    for (int i = 0; i< numProf; i++){
        for(int j = 0; j< numDis; j++){
            bool flag = false;

            s.clear();
            s << "c2_" << i << "," << j;
            s >> consName;
            
            cout << "entrou1.1" << endl;
            if (pref[i][j] > 0) {
                s.clear();
                s << "X(" << i << "," << j << ")";
                s >> varName;
                UFFLP_SetCoefficient( prob, (char*)consName.c_str(),(char*)varName.c_str(), 1.0);

                for (int k = j+1; k < numDis; k++) {

                    if (pref[i][k] > 0 && choque[j][k]) {

                        flag = true;
                        s.clear();
                        s << "X(" << i << "," << k << ")";
                        s >> varName;
                        UFFLP_SetCoefficient( prob, (char*)consName.c_str(),(char*)varName.c_str(), 1.0);
                    }
    		    }

                if (flag) {
                    int counter = 0;
                    UFFLP_AddConstraint( prob, (char*)consName.c_str(), 1.0, UFFLP_Less);	
                }

            }          	
        }
    }

        
    cout << "entrou3" << endl;

   for(int i = 0; i < numProf; i++){
        s.clear();
        s << "c3_" << i; 
        s >> consName;
        bool flag = false;
        for(int j = 0; j < numDis; j++){
            
            if (pref[i][j] > 0) {
                s.clear();
                flag = true;
                s << "X(" << i << "," << j << ")";
                s >> varName;

                
                UFFLP_SetCoefficient( prob, (char*)consName.c_str(),(char*)varName.c_str(), crd[j]);
            }
        }
        if(flag)
            UFFLP_AddConstraint( prob, (char*)consName.c_str(), maxCrd[i], UFFLP_Less);	
   }

    cout << "entrou4" << endl;

   for(int i = 0; i < numProf; i++){

        s.clear();
        s << "c4_" << i; 
        s >> consName;
        bool flag = false;

        for(int j = 0; j < numDis; j++){
            if (pref[i][j] > 0) {
                flag = true;
                s.clear();
                s << "X(" << i << "," << j << ")";
                s >> varName;
                UFFLP_SetCoefficient( prob, (char*)consName.c_str(),(char*)varName.c_str(), crd[j]);
            }
        }
        if(flag)
            UFFLP_AddConstraint( prob, (char*)consName.c_str(), minCrd[i], UFFLP_Greater);	

    }

	// Escreve modelo no arquivo .lp

    UFFLP_WriteLP( prob, "SACI.lp" );

	// Resolve modelo

    UFFLP_StatusType status = UFFLP_Solve( prob, UFFLP_Maximize );

    json jSaida;
    if (status == UFFLP_Optimal) {

      double value;
      cout << "Solucao otima encontrada!" << endl << endl;
      cout << "Solucao:" << endl;

      UFFLP_GetObjValue( prob, &value );
      cout << "Valor da funcao objetivo = " << value << endl;

    // Imprime valor das variaveis nao-nulas
    
    for (int i = 0; i < numProf; i++) {
        for (int j = 0; j < numDis; j++) {
            if(pref[i][j] > 0){
                s.clear();      
                s << "X(" << i << "," << j << ")";
                s >> varName;
                UFFLP_GetSolution( prob, (char*)varName.c_str(), &value );

                if (value > 0.1) {
                    //cout << varName << " = " << value << endl;
                    jSaida["Docentes"][i]["Professor"].push_back(professores[i]);
                    jSaida["Docentes"][j]["Disciplina"].push_back(diciplina[j]);
                }
            }
        }
    }
/*
		// Imprime solucao
	
		printResults(data, prob, advogadoPode, prepostoPode, true);

		// Imprime solucao no arquivo
	
		printResultsToFile(data, prob, advogadoPode, prepostoPode, true);
   */
   }

    else if (status == UFFLP_Infeasible)
   {
       cout << "Solucao inviavel" << endl;		
		//printResults(data, prob, advogadoPode, prepostoPode, false);
   }

   else
   {
      cout << "It seems that the solver did not finish its job...";
      cout << endl;
   }


   // Destroy the problem instance

   UFFLP_DestroyProblem( prob );

	return jSaida.dump(4);
}


/*extern "C" {
    string RecebeDados(string recebeDoJson){
        
        ProfessorTurma x;

        x.carregarDados(recebeDoJson);

        return x.solveCoin();
    }
}*/
