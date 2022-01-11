#include <stdio.h>
#include <string.h>


//Structs para carregar os arquivos insere.bin e remove.bin
typedef struct{
	int CodCli;
	int CodF;
	char Genero[50];
	char NomeCli[50];
	char NomeF[52];
}Clif;

typedef struct{
	int CodCli_rem;
	int CodF_rem;
}Remove;


//Verificação de arquivo já existente ou não, caso não exista cria o arquivo
int verifica(){
	FILE *dp;
	
	if((dp = fopen("data.bin", "r+"))==NULL){
		if((dp = fopen("data.bin", "w+"))==NULL){
			printf("Erro ao abrir o arquivo!\n\n");
			fclose(dp);
			return 0;
		}
	}
	fclose(dp);
	return 1;
}


//função que insere os registro no arquivo data.bin
int insere_arquivo(Clif p[10]){
	int flag=1, k, tamanho=0, caractere=35, posicao, verifica=0, aux1, aux2, aux3, achou=0, j=0;
	FILE *dp, *prox, *arq;
	if((dp = fopen("data.bin", "r+"))==NULL){						//Abre o arquivo data.bin
		printf("\nErro ao abrir o arquivo data.bin!"); 
	}
	
	
	if((arq = fopen("lista.bin", "r+"))==NULL){						//Abre ou cria o arquivo lista.bin, lista.bin é uma lista que contém posição, flag de remoção e tamanho de todos os registros inseridos
		if((arq = fopen("lista.bin", "w+"))==NULL){
			printf("Erro ao abrir o arquivo!\n\n");
		}
	}
	
	if((prox = fopen("prox_cod.bin", "r+"))==NULL){					//Abre prox_cod.bin para controle de quantos já foram incluidos e removidos para quando o programam for fechado
		printf("\nErro ao abrir o arquivo de próximos códigos!");
	}
	
	fread(&k,sizeof(int),1,prox);
	rewind(prox);
	
	tamanho = sizeof(int)*4; 
	tamanho = tamanho + ((strlen(p[k].Genero)) + (strlen(p[k].NomeCli)) + (strlen(p[k].NomeF)) + 5); //Calcula tamanho do registro completo
	
	fread(&aux1,sizeof(int),1,arq);
	
	while((!feof(arq)) && achou==0){			//Faz a verificação para ver se tem algum espaço vazio no arquivo data.bin
		if(j!=0){
			fread(&aux1,sizeof(int),1,arq);
		}
		j=1;
		fread(&aux2,sizeof(int),1,arq);
		fread(&aux3,sizeof(int),1,arq);
		
		if(aux2==0){
			if(tamanho<=aux3){				//Condição se achou espaço vazio e espaço<tamanho do arquivo novo marca
				achou=1;
			}
		}else{
			achou=0;
		}
	}
	
	if(achou==1){						//Caso seja válida a verificação acima insere no espaço disponível
		fseek(dp,aux1,SEEK_SET);
		fseek(arq,-12,SEEK_CUR);
		posicao = ftell(dp);
	
			fwrite(&posicao,sizeof(int),1,arq);
			fwrite(&flag,sizeof(int),1,arq);
			fwrite(&tamanho,sizeof(int),1,arq);

	
			fwrite(&tamanho,sizeof(int),1,dp);
			fwrite(&flag, sizeof(int),1,dp);
			fwrite(&p[k].CodCli,sizeof(int),1,dp);
			fputc(caractere,dp);
			fwrite(&p[k].CodF,sizeof(int),1,dp);
			fputc(caractere,dp);
			fwrite(p[k].Genero,strlen(p[k].Genero),1,dp);
			fputc(caractere,dp);
			fwrite(p[k].NomeCli,strlen(p[k].NomeCli),1,dp);
			fputc(caractere,dp);
			fwrite(p[k].NomeF,strlen(p[k].NomeF),1,dp);
			fputc(caractere,dp);
	
			k++;
	
			fwrite(&k,sizeof(int),1,prox);
	
			fclose(prox);
			fclose(dp);
			fclose(arq);
	
			return 1;
	}else{												//Caso não ache insere no final do arquivo
		fseek(dp,0,SEEK_END);
		fseek(arq,0,SEEK_END);
		
		posicao = ftell(dp);
	
			fwrite(&posicao,sizeof(int),1,arq);
			fwrite(&flag,sizeof(int),1,arq);
			fwrite(&tamanho,sizeof(int),1,arq);

	
			fwrite(&tamanho,sizeof(int),1,dp);
			fwrite(&flag, sizeof(int),1,dp);
			fwrite(&p[k].CodCli,sizeof(int),1,dp);
			fputc(caractere,dp);
			fwrite(&p[k].CodF,sizeof(int),1,dp);
			fputc(caractere,dp);
			fwrite(p[k].Genero,strlen(p[k].Genero),1,dp);
			fputc(caractere,dp);
			fwrite(p[k].NomeCli,strlen(p[k].NomeCli),1,dp);
			fputc(caractere,dp);
			fwrite(p[k].NomeF,strlen(p[k].NomeF),1,dp);
			fputc(caractere,dp);
	
			k++;
	
			fwrite(&k,sizeof(int),1,prox);
	
			fclose(prox);
			fclose(dp);
			fclose(arq);
	
			return 1;
	}
}

int remove_arquivo(Remove q[10]){									//função para remover
	int flag=0, k, m, tamanho, codn, codf, desc, status=0, j=0;
	FILE *dp, *prox, *arq;
	
	if((dp = fopen("data.bin", "r+"))==NULL){
		printf("\nErro ao abrir o arquivo data.bin!");
	}
	
	if((arq = fopen("lista.bin", "r+"))==NULL){
		printf("\nErro ao abrir o arquivo lista.bin!");
	}
	
	if((prox = fopen("prox_cod.bin", "r+"))==NULL){
		printf("\nErro ao abrir o arquivo de próximos códigos!");
	}
	
	fread(&k,sizeof(int),1,prox);									//Alimentar prox_cod.bin para não perdermos referência quando o programa é fechado
	fread(&m,sizeof(int),1,prox);
	rewind(prox);
	
	while((fread(&tamanho,sizeof(int),1,dp)) && status==0){			//Alimentar a lista.bin para sabermos se vai existir espaço removido do data.bin
		fread(&desc,sizeof(int),1,dp);
		fread(&codn,sizeof(int),1,dp);
		fseek(dp,1,SEEK_CUR);
		fread(&codf,sizeof(int),1,dp);
		
		
		
		if(codn==q[m].CodCli_rem){
			if(codf==q[m].CodF_rem){
				status = 1;
			}else{
				tamanho = tamanho - 17;
				fseek(dp,tamanho,SEEK_CUR);
			}
		}else{
				tamanho = tamanho - 17;
				fseek(dp,tamanho,SEEK_CUR);
		}
		j++;
	}
	j--;
	
	fseek(dp,-17,SEEK_CUR);
	
	fwrite(&flag,sizeof(int),1,dp);							//Caso ache ele seta a flag de lista.bin que é reconhecida como os arquivos removidos ela seta em 0
	
	if(j==0){
		j=4;
	}else{
		j = ((j*12) + sizeof(int));
	}

	fseek(arq,j,SEEK_SET);
	fwrite(&flag,sizeof(int),1,arq);
	
	m++;
	
	fwrite(&k,sizeof(int),1,prox);
	fwrite(&m,sizeof(int),1,prox);
	
	fclose(prox);
	fclose(dp);
	fclose(arq);
	
	return 1;
}

int carrega_arquivo(Clif p[10], Remove q[10]){				//Função que carrega arquivos em uma struct tanto insere.bin quanto remove.bin
	FILE *arq;
	int j=0;
	
	if((arq=fopen("insere.bin", "r+"))==NULL){
		printf("Erro ao abrir o arquivo insere.bin\n\n");
	}
	
	while(fread(&p[j].CodCli,sizeof(p[j].CodCli),1,arq)){
		fread(&p[j].CodF,sizeof(p[j].CodF),1,arq);
		fread(p[j].Genero,sizeof(p[j].Genero),1,arq);
		fread(p[j].NomeCli,sizeof(p[j].NomeCli),1,arq);
		fread(p[j].NomeF,sizeof(p[j].NomeF),1,arq);
		
		j++;
	}
	
	fclose(arq);
	j=0;
	
	if((arq=fopen("remove.bin", "r+"))==NULL){
		printf("Erro ao abrir o arquivo insere.bin\n\n");
	}
	
	j=0;
	
	while(fread(&q[j].CodCli_rem,sizeof(q[0].CodCli_rem),1,arq)){
		fread(&q[j].CodF_rem,sizeof(q[0].CodF_rem),1,arq);

		j++;
	}
	
	return 1;
}

int pega_campo(FILE *arq, char *pont_campo){			//Fução para pegar os campos de tamanho variáveis já inseridos em data.bin (função necessária para a compactação do arquivo
	char a;
	int i=0, x;
	
	pont_campo[i] = '\0';
	
	while(fread(&a,sizeof(char),1,arq)){
	if(a == '#'){
		break;	
	}else{
		pont_campo[i] = a;
		i++;
	}
}
	pont_campo[i] = '\0';
	
	return strlen(pont_campo);
}

int compacta(){						//Funcção para compactar o arquivo data.bin
	FILE *arq, *arq1, *arq2, *arq3;
	int aux1, aux2, aux3, flag, codcli, codf, tamanho, caractere=35, c, int_campo1, int_campo2, int_campo3, posicao_nova;
	char nomecli[50], nomef[50], genero[50];
	
	if((arq = fopen("lista.bin", "r+"))==NULL){
		printf("Não foi possível abrir o arquivo!\n");
	}
	
	if((arq1 = fopen("data.bin", "r+"))==NULL){
		printf("Não foi possível abrir o arquivo!\n");
	}
	
	if((arq2 = fopen("data_sub.bin", "w+"))==NULL){
		printf("Não foi possível abrir o arquivo!\n");
	}
	
	if((arq3 = fopen("lista_sub.bin", "w+"))==NULL){
		printf("Não foi possível abrir o arquivo!\n");
	}
	
	while(fread(&aux1,sizeof(int),1,arq)){			//Ele vai percorrer todos os elementos de lista.bin
		fread(&aux2,sizeof(int),1,arq);
		fread(&aux3,sizeof(int),1,arq);
		
		rewind(arq1);
		
		if(aux2==1){								//Caso a flag remoção esteja setado em 1 é arquivo existente					
			fseek(arq1,aux1,SEEK_SET);				//No lista.bin o primeiro registro de cada trio é a posição inicial de cada registro em data.bin
			fread(&tamanho,sizeof(int),1,arq1);		//Faz a verificação e pega todos os registros de tamanho fico (ints) e tamanho variavel (char) em data.bin
			fread(&flag,sizeof(int),1,arq1);
			fread(&codcli,sizeof(int),1,arq1);
			fread(&c,1,1,arq1);
			fread(&codf,sizeof(int),1,arq1);
			fread(&c,1,1,arq1);
			int_campo1 = pega_campo(arq1,nomecli);
			int_campo2 = pega_campo(arq1,nomef);
			int_campo3 = pega_campo(arq1,genero);

			posicao_nova = ftell(arq2);
			fwrite(&posicao_nova,sizeof(int),1,arq3); //Atualiza o arquivo lista.bin para excluir os registros inativos
			fwrite(&aux2,sizeof(int),1,arq3);
			fwrite(&aux3,sizeof(int),1,arq3);
			
			
			fwrite(&tamanho,sizeof(int),1,arq2);	//Escreve todos os registros ativos em um novo arquivo chamado data_sub.bin
			fwrite(&flag, sizeof(int),1,arq2);
			fwrite(&codcli,sizeof(int),1,arq2);
			fputc(caractere,arq2);
			fwrite(&codf,sizeof(int),1,arq2);
			fputc(caractere,arq2);
			fwrite(nomecli,int_campo1,1,arq2);
			fputc(caractere,arq2);
			fwrite(nomef,int_campo2,1,arq2);
			fputc(caractere,arq2);
			fwrite(genero,int_campo3,1,arq2);
			fputc(caractere,arq2);
		}
	}
	
	fclose(arq);
	fclose(arq1);
	fclose(arq2);
	fclose(arq3);
	
	remove("lista.bin");
	rename("lista_sub.bin", "lista.bin");
	remove("data.bin");					//Após terminar remove o arquivo com as fragmentações
	rename("data_sub.bin", "data.bin"); //Renomeia o arquivo novo limpo
	
	return 1;
}

int menu(){						//Imprime o menu inicial
	int opcao;
	
	printf("-------- Menu ---------\n");
	printf("1 - Inserir novo registro\n");
	printf("2 - Excluir Registro\n");
	printf("3 - Compactacao\n");
	printf("4 - Carregar Arquivos\n");
	printf("5 - Sair\n");
	printf("\nEsolha a opcao desejada: ");
	scanf("%d", &opcao);
	
	if(opcao<=0 || opcao>5){
		printf("Opcao Invalida!\n\n");
		return 0;
	}
	
	return opcao;
}


int main(){
	
	Clif clif[10];
    Remove remove[10];
    
	int opcao = 1, aux, prov=0;
	FILE *arquivo, *arquivo2, *aux_arq;
	
	if((aux_arq = fopen("prox_cod.bin", "r+"))==NULL){		//Cria o arquivo prox_cod.bin para quando fecharmos o programa ele saber o próximo elemento a ser inserido e removido
		if((aux_arq = fopen("prox_cod.bin", "w+"))==NULL){
			printf("Erro ao abrir o arquivo!\n\n");
		}
		fwrite(&prov,sizeof(int),1,aux_arq);
		fwrite(&prov,sizeof(int),1,aux_arq);
	}
	
	fclose(aux_arq);
	
	printf("------------ Seja Bem-Vindo ao Sistema de Streaming de Videos ------------\n\n");
	
	while(opcao>0 && opcao<=4){
		opcao = menu();
		
		switch(opcao){
			case 1:
				verifica();
					aux = insere_arquivo(clif);
					if(aux==1){
						printf("Inserido com sucesso!\n\n");
					}else if(aux==0){
						printf("Nao foi possivel inserir o registro!\n\n");
					}else if(aux==2){
						printf("Todos os registros ja foram inseridos!\n\n");
					}
				break;
				
			case 2:
				verifica();
					aux = remove_arquivo(remove);
					if(aux==1){
						printf("Removido com sucesso!\n\n");
					}else{
						printf("Nao foi possivel remover o registro!\n\n");
					}
				break;
				
			case 3:
				aux = compacta();
				if(aux ==1){
					printf("Arquivo compactado com sucesso\n\n");
				}else{
					printf("Não foi possível compactar o arquivo!\n\n");
				}
				break;
				
			case 4:
				aux = carrega_arquivo(clif,remove);
				if(aux==1){
					printf("Carregado com sucesso!\n\n");
				}else{
					printf("Nao foi possivel carregar os arquivos!\n\n");
				}
				break;
			case 5:
				break;
		}
	}
}
