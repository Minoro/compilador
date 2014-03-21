#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<iostream>
using namespace std;

//Declaração das funções
int recortarPalavra(char *palavra);

bool isInicioComposto(char palavra);
bool isFimComposto(char palavra);
bool isComposto(char *palavra);
bool isSimbolo(char palavra);
bool isNumero(char palavra);
bool isLetra(char palavra);
bool isSinal(char palavra);
bool isPalavraReservada(char *palavra);
void avaliarEstado(int estadoFinal, char *token);

//Declaração dos estados do automato
int estado1(char *palavra);
int estado2(char *palavra);
int estado3(char *palavra);
int estado4(char *palavra);
int estado5(char *palavra);
int estado6(char *palavra);
int estado7(char *palavra);
int estado8(char *palavra);
int estado10(char *palavra);
//char *cortaPalavra(string palavra);


//const char *simbolos[] = {",", ":", ";",".","-","+","/","(",")","=","*",">","<","{","}"};
const char simbolos[] = ",:;,.-+/()=*><{}";
const char nums[] = "1234567890";
const char letras[] = "abcdefghijklmnopqrstuvxz0123456789";

bool isComentario = false;

int main(int argc, char* argv[]){
	int linha =1;//linha do codigo
	//FILE *entrada = fopen("entrada.txt", "r+");//abertura do arquivo de entrada
	ifstream entrada;
	entrada.open("entrada.txt");//abertuda do arquivo de entrada

	char *palavraLida;//ponteiro para palavra lida
	char *ptch, *pt_token;//ponteiro para analizar o token
	char type;//tipo do token (variavel, inteiro,etc)
	int estadoFinal;
	bool isComentario = false;//booleano para verificar se é comentário
	string palavra;
	if(entrada == NULL){
		cout<<"Erro ao abrir o arquivo"<<endl;
		return -1;
	}
	
	while(getline(entrada, palavra)){
		palavraLida =(char *)malloc((sizeof(char)*palavra.length())+1);
		palavraLida = (char *) palavra.c_str();

		pt_token = strtok(palavraLida, " ");//corta a linha nos espaços em branco
		while(pt_token){
			estadoFinal = recortarPalavra(pt_token);
			if(estadoFinal == -1){
				printf("Erro na linha %d\n", linha);
				printf("Token inválido: %s\n", pt_token);
				entrada.close();
				return -1;
			}
			pt_token = strtok(NULL, " ");
		}
		linha++;
		//cout<<"Palavra "<<palavra<<endl; 
	}//fim-while(getline(entrada, palavra))
	entrada.close();
}
/*
	Recorta a palavra lida, recortando os simbolos especiais
	Em caso de erro retorna -1, em caso de sucesso retorna 1
*/
int recortarPalavra(char *palavra){
	int estadoFinal;
	char *buffer;
	int i;
	//calcula tamanho do buffer
	if(isSimbolo(palavra[0])){
		if(strlen(palavra) >1 && isComposto(&palavra[0]))
			i = 2;//tamnho do buffer de simbolo composto
		else
			i = 1;//tamanho do buffer de simbolo simples
	}else if(isNumero(palavra[0])){//se for numero
		i = strspn(palavra, nums);//tamanho do buffer é o tamanho de numeros seguidos
	}else if(isLetra(palavra[0])){//se for letra
		i = strspn(palavra, letras);//tamnho do buffer é o numero de letras seguidas
	}else{
		cout<<"AAA"<<endl;
		i = strlen(palavra);//simbolo não reconhecido
	}
	buffer =(char *) malloc((sizeof(char)*i)+1);//cria buffer para analisar o token

	strncpy(buffer, palavra, i);//passa o token para o buffer

	cout<<buffer << " = ";

	if(!isComentario){
		estadoFinal = estado1(buffer);//analisa o token
		avaliarEstado(estadoFinal, buffer);

		if(!strcmp(buffer, "(*") || !strcmp(buffer, "{"))
			isComentario = true;
		if(!strcmp(buffer, "*)") || !strcmp(buffer, "}"))
			isComentario = false;//fim de comentario
		if(estadoFinal == -1)
			return -1;
	}

	if(!strcmp(buffer, "(*") || !strcmp(buffer, "{"))
		isComentario = true;
	if(!strcmp(buffer, "*)") || !strcmp(buffer, "}")){
		isComentario = false;//fim de comentario
		int estadoFinal = estado1(buffer);//analisa o token
		avaliarEstado(estadoFinal, buffer);
	}

	free(buffer);//desaloca buffer
	

	if(i < strlen(palavra) && !isComentario) //se a palavra foi recortada manda a outra parte da palavra para avaliação
		recortarPalavra(&palavra[i]);

	return 0;
}

/*
	Verifica se o char enviado é um simbolo
*/
bool isSimbolo(char palavra){
	if((palavra >= 40 && palavra <= 47) ||(palavra >= 58 && palavra <= 62) || palavra == 123 || palavra == 125 )
		return true;
	return false;
}

/*
	Verifica se o char enviado é o inicio de um simbolo composto
*/
bool isInicioComposto(char palavra){
	if(palavra == ':' || palavra == '<' || palavra == '(' || palavra == '*')
		return true;
	return false;

}

/*
	Verifica se o simbolo enviado é o fim de um simbolo composto
*/
bool isFimComposto(char palavra){
	if(palavra == '=' || palavra == '>' || palavra == ')' || palavra == '*')
		return true;
	return false;

}
/*
	Retorna verdadeiro se for um simbolo composto
*/

bool isComposto(char *palavra){
	const char *simbolos_comp[] ={"(*", "*)", "<>", ">=", "<=", ":="};
	int lenVetor = sizeof(simbolos_comp)/sizeof(*simbolos_comp);
	for(int i = 0; i < lenVetor; i++){
		if(!strcmp(palavra, simbolos_comp[i]))
			return true;
	}
	return false;
}

/*
	Verifica se o char enviado é uma letra (maiuscula ou minuscula)
*/
bool isLetra(char palavra){
	if((palavra >= 65 && palavra <= 90) || (palavra >= 97 && palavra <= 122))
		return true;
	return false;
}

/*
	Verifica se o char enviado como parametro é um numero inteiro
*/
bool isNumero(char palavra){
	if(palavra >= 48 && palavra <= 57)//se for numero
		return true;
	return false;
}
/*
	Verifica se o char enviado esta com sinal na primeira posição
*/
bool isSinal(char palavra){
	if(palavra == '+' || palavra == '-')
		return true;//esta com um sinal na frente
	return false;
}

/*
	Verifica se a palavra (token) é uma palavra reservada
*/
bool isPalavraReservada(char *palavra){
	char *palavrasReservadas[] = {"until", "boolean", "char", "string", "case", "downto", "and", "while", "write", "do", "end", "begin", "integer", "read", "or", "for", "to", "program", "repeat", "if", "else", "then", "go to", "var", "array", "div", "not", "of", "procedure", "type", "label", "function", "end."};//palavras reservadas
	int lenVetor = sizeof(palavrasReservadas)/sizeof(*palavrasReservadas);//conta para tamanho do vetor

	for(int i = 0; i < lenVetor; i++){//verifica se é palavra reservada
		if(!strcmp(palavra, palavrasReservadas[i]))
			return true;
	}
	return false;
}



void avaliarEstado(int estadoFinal, char *token){
	switch(estadoFinal){

		case -1:
			cout<<"erro"<<endl;
			break;
		case 2:
			if(isPalavraReservada(token))
				cout<<"É reservada"<<endl;
			else
				cout<<"Identificador"<<endl;
			break;
		case 3:
			cout<<"NUMERO"<<endl;
			break;
		case 4:
			cout<<"Simbolo Especial"<<endl;
			break;
		case 5:
			cout<<"Simbolo Composto"<<endl;
			break;
		case 6:
			cout<<"Simbolo Especial"<<endl;
			break;
		case 7:
			cout<<"Simbolo Especial"<<endl;
			break;
		case 8:
			cout<<"Simbolo Especial"<<endl;
			break;
		case 9:
			cout<<"Comentário"<<endl;
			break;
		case 10:
			cout<< "Simbolo Especial"<<endl;
			break;

	}//fim-switch

}

/*
	Estados do automato
*/

int estado1(char *palavra){
	//int estadoFinal = 1;

	if(palavra[0] == '\0')
		return 1;//
	if((palavra[0] >=65 && palavra[0]<= 90) ||(palavra[0] >=97 && palavra[0] <= 122) )
		return estado2(&palavra[1]);
	
	if((palavra[0]>= 48 && palavra[0] <= 57))//se ler numero + ou - vai para estado 3
		return estado3(&palavra[1]);

	if(palavra[0] == 58 || palavra[0] == 62)//tratar : e >
		return estado4(&palavra[1]);
	
	if(palavra[0] == '<')
		return estado6(&palavra[1]);

	if(palavra[0] == '(')
		return estado7(&palavra[1]);
	
	if(palavra[0] == '*')
		return estado8(&palavra[1]);

	if(isSimbolo(palavra[0]))
		return estado10(&palavra[1]);

	return -1;

}	

int estado2(char *palavra){
	if(palavra[0] == '\0')
		return 2;//retorna o numero do estado
	if((palavra[0] >=65 && palavra[0]<= 90) ||(palavra[0] >=97 && palavra[0] <= 122) )
		return estado2(&palavra[1]);
	if((palavra[0]>= 48 && palavra[0] <= 57))//se ler numero
		return estado2(&palavra[1]);
	if(palavra[0] == ':')
		return estado4(&palavra[1]);
	if(palavra[0] == '<' || palavra[0] == '>')
		return estado6(&palavra[1]);
	if(palavra[0] == '(')
		return estado7(&palavra[1]);
	if(palavra[0] == '*')
		return estado8(&palavra[1]);

	return -1;//não reconhecida
}

int estado3(char *palavra){
	if(palavra[0] == '\0')
		return 3;
	if(palavra[0]>= 48 && palavra[0] <= 57)
		return estado3(&palavra[1]);
	else if( palavra[0] == 42 || palavra[0] == 45 || palavra[0] == 43 || palavra[0] == 47)//se ler operando
		return estado1(&palavra[1]);
	return -1;
}

int estado4(char *palavra){	
	if(palavra[0] == '\0')
		return 4;
	if(palavra[0] == '=')
		return estado5(&palavra[1]);
	
	return -1;
}

int estado5(char *palavra){
	if(palavra[0] == '\0')
		return 5;
	return -1;
}

int estado6(char *palavra){
	if(palavra[0] == '\0')
		return 6;
	if(palavra[0] == '=' || palavra[0] == '>')
		return estado5(&palavra[1]);
	return -1;
}

int estado7(char *palavra){
	if(palavra[0] == '\0')
		return 7;
	if(palavra[0] == '*')
		return 9;//marcador para abrir comentario
	return -1;
}

int estado8(char *palavra){
	if(palavra[0] == '\0')
		return 8;
	if(palavra[0] == ')'){
		if(palavra[1] != '\0')
			return estado1(&palavra[1]);//fim de comentario retorna ao primeiro estado para analise
		return 9;//marcador de fim de comentario
	}
	return -1;
}

int estado10(char *palavra){
	if(palavra[0] == '\0')
		return 10;
	return -1;
}


