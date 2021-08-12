#include <api.h>
#include <stdlib.h>

/* Definindo as constantes */
#define w 	1  	/* Constante da componente inercial */
#define phi	2	/* Constante da componente social */


Message position, veloc, pbest, gbest, funcao0, funcao;
/* Função objetivo */

int fobj (int x1, int x2){
	return (10*x1 + x2 - 12);
}

/* PSO */

int main(){

	/* Inicialização dos parâmetros */

	int n_var = 2;
    	int n_part = 50;
    	    
    	int Menor = 0;
   	int MenorFun0 = 0, MenorFun = 0;  	    
    	int i, j;
    
    	Echo("Início do PSO: ");
    	Echo(itoa(GetTick()));
    	
    	/* Inicializando os valores das particulas */
    	
    	for(i=0; i < MSG_SIZE; i++){ /* MSG_SIZE é definido no api.h como sendo 128 */
            	if (i < n_var*n_part){
            		position.msg[i] = rand(GetTick(),1,100);
            	}
            	else{
            		position.msg[i] = 0;
            	}
        }
        
        /* Inicializando o vetor funcao0 com os valores das particulas iniciais */
        
        for (i=0; i < MSG_SIZE; i++){
        	if (i < n_part){
        		funcao0.msg[i] = fobj(position.msg[2*i],position.msg[2*i+1]);
        	}
        	else{
        		funcao0.msg[i] = 0;
        	}
        }
        
        /* Guardando o menor valor da função e a partícula de menor valor */
        
        for (i=0; i < MSG_SIZE; i++){
        	if (i < n_part){
        		if (funcao0.msg[i] < funcao0.msg[Menor]){
        			Menor = i;
        			MenorFun0 = funcao0.msg[i];
        		}
        	}
        }
        
                
        /* Inicializando os valores de veloc, pbest e gbest */
        
        for(i=0; i < MSG_SIZE; i++){ 
            	if (i < n_var*n_part){
        		veloc.msg[i] = 1;
        		pbest.msg[i] = position.msg[i];
        	}
        	else{
        		veloc.msg[i] = 0;
        		pbest.msg[i] = 0;
        	}
        }
        
        for(i=0; i < MSG_SIZE; i++){
        	if (i < n_var){
        		gbest.msg[i] = position.msg[2*Menor+i];
        	}
        	else{
        		gbest.msg[i] = 0;
        	}
        }
        
        /* Algoritmo do PSO */
        
        int ite = 0;
        int target_error = 100000;
        int inercial = 0, social = 0, cognitivo = 0;
        
        while ((ite < 200) && (target_error > 0)){
        	
        	/* Atualizando o vetor veloc */
        	for(i=0; i < MSG_SIZE; i++){
            		if (i < n_var*n_part){
            			
        			inercial = w * veloc.msg[i];
        			social = phi * (rand(GetTick(),2,100)%10) * (pbest.msg[i] - position.msg[i]);
        			cognitivo = phi * (rand(GetTick(),2,100)%10) * (gbest.msg[i%n_var] - position.msg[i]);
        			        			        			
        			veloc.msg[i] = inercial + social + cognitivo;
        		}
        		else{
        			veloc.msg[i] = 0;
        		}
        	}
        	
        	/* Atualizando as particulas */
        	for(i=0; i < MSG_SIZE; i++){
            		if (i < n_var*n_part){
        			position.msg[i] = position.msg[i] + veloc.msg[i];
        		}
        		else{
        			position.msg[i] = 0;
        		}
        	}
        	
        	  	
        	/* Avaliação da função objetivo para os novos valores das particulas */
        	for(i=0; i < MSG_SIZE; i++){
            		if (i < n_part){
        			funcao.msg[i] = fobj(position.msg[2*i],position.msg[2*i+1]);
        		}
        		else{
        			funcao.msg[i] = 0;
        		}
        	}
        	        	
        	for (i=0; i < n_part; i++){
        		if (funcao.msg[i] < funcao0.msg[i]){
        			for (j=0; j < n_var; j++){
        				pbest.msg[2*i+j] = position.msg[2*i+j];
        			}
        			funcao0.msg[i] = funcao.msg[i];
        		}
        	}
        	
        	
        	
        	/* Atualizando o menor valor da função e a melhor particula */
        	for(i=0; i < MSG_SIZE; i++){
            		if (i < n_part){
        			if (funcao0.msg[i] < funcao.msg[Menor]){
        				Menor = i;
        		 		MenorFun = funcao0.msg[i];
        			}
        		}
        	}
        	
        	/* Cálculo da tolerância */
        	target_error = MenorFun - MenorFun0;
        	if (target_error < 0){
        		target_error = - target_error;
        	}
        	
        	/* Atualizando gbest e o melhor valor da função */
        	for (i=0; i < n_var; i++){
        		if (MenorFun < MenorFun0){
        			gbest.msg[i] = pbest.msg[2*Menor+i];
        			
        		}
        	}
        	
        	MenorFun0 = fobj(gbest.msg[0], gbest.msg[1]);
        	
        	/* Imprimindo os resultados */
        	Echo("Iteracao: ");
        	Echo(ite);
        	Echo("Melhor Particula: ");
        	Echo(Menor);
        	Echo("Valor da funcao objetivo: ");
        	Echo(MenorFun0);
        	Echo("==================================");
        	ite = ite + 1;
        }
        
        Echo("Melhor resultado da funcao: ");
        Echo(itoa(MenorFun0));
        Echo("Melhor solucao: ");
        Echo("X1 = ");
        Echo(itoa(gbest.msg[0]));
        Echo("X2 = ");
        Echo(itoa(gbest.msg[1])); 
        
        Echo("Fim do PSO: ");
        Echo(itoa(GetTick()));    
}
