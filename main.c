#include <stdio.h>
#include <stdlib.h>
#define MAX 10 // máximo do vetor de células (ver na função main)
int* ini;


/*
	Estrutura célula contendo os seguintes campos:
		- valor
		- linha e coluna (posição da célula)
		- ponteiro para o próximo elemento
		- ponteiro para o elemento anterior
*/
typedef struct celula
{
	int valor;
	int lin, col;
	struct celula *prox;
	struct celula *ant;
} t_celula;


// variáveis globais
t_celula *primeiro; // primeiro elemento da lista
t_celula *ultimo; // último elemento da lista


// protótipos de funções
t_celula* criar_celula(int lin, int col, int valor);
void inserir(t_celula *novo);
void deletar(int lin, int col);
int vazia();
t_celula* existe_celula(int lin, int col);
void imprimir_elementos();
void imprimir_matriz();


/* Função que cria uma nova celula passando a linha e coluna */
t_celula* criar_celula(int lin, int col, int valor){
	t_celula * novo;

	// aloca espaço em memória
	novo = (t_celula*)malloc(sizeof(t_celula));

	// testa se houve falha na alocação
	if(!novo){
		printf("Erro ao alocar memoria!!\n");
		exit(1);
	}

	// atribuição de variáveis
	novo->valor = valor;
	novo->lin = lin;
	novo->col = col;

	return novo;
}


/*
	Função que insere um novo elemento (célula) na matriz esparsa
	A lista é ordenada de acordo com o nome da célula
*/
void inserir(t_celula *novo){
	/*
		obtém uma célula passando a linha e coluna do novo elemento
		para verificar se a posição já está ocupada
	*/
	t_celula* cel = existe_celula(novo->lin, novo->col);

	if(!cel){ // verifica se não existe
		if(!ultimo){
			/*
				Se o "ultimo" é NULL, então a lista está vazia, o "novo"
				será o primeiro elemento a ser inserido
			*/

			// o próximo e o anterior apontam para NULL
			novo->prox = NULL;
			novo->ant = NULL;

			// faz o "ultimo" e "primeiro" apontarem para "novo"
			ultimo = novo;
			primeiro = novo;
		}
		else
		{
			// "p" serve para percorrer a lista
			// "ant" aponta para o anterior de "p"
			t_celula *p, *ant;


			p = primeiro; // começa do topo da lista
			ant = p->ant; // "ant" é o anterior ao "p", ou seja, NULL

			// poderia fazer também: ant = NULL;

			// percorre a lista
			while(p) // enquanto "p" diferente de NULL
			{
				/*
					Se a célula corrente "p" precede a
					célula "novo", então continua percorrendo
				*/
				if((p->lin < novo->lin) || ((p->lin == novo->lin) && (p->lin + p->col) < (novo->lin + novo->col)))
				{
					ant = p; // guarda o anterior
					p = p->prox; // aponta para o próximo
				}
				else
				{
					/*
						Se cair aqui é porque encontramos
						onde inserir a nova célula
					*/

					/*
						Se p->ant é diferente de NULL, então trata-se
						de um elemento intermediário
					*/
					if(p->ant)
					{
						/*
							p->ant é a célula que contém um nome imediatamente
							anterior à nova célula que será inserida.
							Então dizemos que o próximo de p->ant irá apontar
							para essa nova célula
						*/
						p->ant->prox = novo;

						/*
							Se p->ant é a célula imediatamente anterior à
							célula "novo", então "p" é a célula imediatamente
							posterior à célula "novo", por isso dizendos que o
							próximo de "novo" aponta para "p'.
						*/
						novo->prox = p;

						// novo->ant tem que apontar para o p->ant
						novo->ant = p->ant;

						// faz p->ant apontar para o novo
						p->ant = novo; // correção de: novo->avetnt = i;

						return; // sai da função
					}

					/*
						Se chegar aqui é porque p->ant é NULL, então
						iremos inserir o novo elemento no início, ou seja,
						o "novo" será o novo primeiro da lista
					*/

					novo->prox = p; // o próximo do novo aponta para "p"
					novo->ant = NULL; // o anterior aponta para NULL (primeiro da lista)
					p->ant = novo; // faz o anterior de "p" apontar para o novo
					primeiro = novo; // "primeiro" aponta para "novo"

					return; // sai da função
				}
			}

			/*
				Se chegou aqui, então insere no final, agora preciso
				da variável "ant".
			*/

			// faz o próx. do antigo último apontar para "novo"
			ant->prox = novo;

			// como o "novo" agora é o último, então o próximo dele
			// aponta para NULL
			novo->prox = NULL;

			// o anterior ao "novo" aponta para o "ant"
			novo->ant = ant;

			// atualiza o último elemento da lista
			ultimo = novo;
		}
	}
	else
	{
		// se já existe então sobrescreve o valor
		cel->valor = novo->valor;
	}
}

// Função que deleta um elemento pela posição (linha e coluna)
void deletar(int lin, int col){
	// verifica se existe a célula (elemento)
	if(existe_celula(lin, col)){
		// remove do começo
		if(primeiro->lin == lin && primeiro->col == col){
			// pego o apontador para o próximo
			t_celula *prox = primeiro->prox;

			free(primeiro); // libera memória

			// verifico se o próximo existe
			if(prox)
				prox->ant = NULL; // atualizo o anterior

			// faz o "primeiro" ser o "prox"
			primeiro = prox;

			return; // sai da função
		}

		/*
			Se chegou aqui é porque NÃO irá remover do início e
			existe mais de 1 elemento
		*/
		t_celula *p = primeiro->prox;

		// percorre a lista
		while(p){
			if(p->lin == lin && p->col == col)	{
				if(p->prox){ // testa se NÃO é o último elemento
					/*
						Faço o próximo do anterior de "p" apontar
						para o próximo de "p"
					*/
					p->ant->prox = p->prox;

					/*
						Faço o anterior do proximo de "p" apontar
						para o anterior de "p"
					*/
					p->prox->ant = p->ant;
				}
				else{
					// se chegou aqui, é porque é o último elemento
					
					// faz o proximo do anterior de "p" apontar para NULL
					p->ant->prox = NULL;
					
					// faz o anterior de "p" ser o "ultimo"
					ultimo = p->ant;
				}
				free(p); // libera o "p"
				return; // sai da função
			}
			p = p->prox;
		}
	}
}


// Função que verifica se a lista está vazia
int vazia(){
	if(!ultimo) // se o último for NULL, então a lista está vazia
		return 1;
	return 0;
}


// Função que verifica se um elemento existe pela posição (linha e coluna)
t_celula* existe_celula(int lin, int col){
	t_celula* p = primeiro;

	while(p){ // percorre a lista
		// verifica a linha e a coluna
		if(lin == p->lin && col == p->col)
			return p; // retorna a célula
		p = p->prox;
	}
	return NULL;
}


// Função que imprime os lementos da lista
void imprimir_elementos(){
	t_celula* p = primeiro;

	while(p){ // percorre a lista
		printf("(%d,%d)\n", p->lin, p->col);
		p = p->prox;
	}
}	

// Função para imprimir a matriz convencional
// "0" indica o elemento repetido da matriz esparsa 
void imprimir_matriz(){
	if(!vazia()) // verifica se a lista NÃO está vazia
	{
		
		int total_linhas = 6; // corresponde ao estacionamento físico
		int total_colunas = 6; // corresponde ao estacionamento físico
		t_celula* p = primeiro;
		for(int i = 0; i < total_linhas; i++){
			for(int j = 0; j < total_colunas; j++){
				// busca por um elemento passando a linha e coluna
				t_celula* cel = existe_celula(i + 1, j + 1);

				if(cel) // testa se encontrou o elemento
					printf("%d\t", cel->valor);
				else // se nao existe o elemento, entao imprime '0'
					printf("0\t");
			}
			printf("\n");
		}
	}
	else
		printf("\n\n");
}
// alocando a memoria do array ini
void alocaM() {
     ini =(int*) malloc(sizeof(int) * 10);
   
}

// puxando os valores iniciais do arquivo ini.txt
void iniciar() {
   FILE* f; 
   f = fopen("ini.txt", "r");

   if(f == 0) {
    printf("Erro na leitura\n");
    exit(1);
   }

   alocaM();

   for (int i = 0 ; i <= 9; i++) {

    fscanf(f, "%d", &ini[i]);
}

    fclose(f);
}


int main(int argc, char * argv[]){
   	int opcao = 0;
    	int lin;
    	int col;
    
    	printf ("\n-----------------------------------------------?\n");
    	printf("BEM VINDO AO SISTEMA DE MONITORAMENTO DE ESTACIONAMENTO\n");
    	printf("O ESTACIONAMENTO CONTA COM 36 VAGAS \n");
	printf ("\n-----------------------------------------------?\n\n");
    
    
        iniciar();
	
	t_celula *vet[MAX]; // vetor de células

	// passa a linha e coluna da célula
	vet[0] = criar_celula(2, 1, ini[0]);
	vet[1] = criar_celula(1, 2, ini[1]);
	vet[2] = criar_celula(6, 3, ini[2]);
	vet[3] = criar_celula(1, 1, ini[3]);
	vet[4] = criar_celula(3, 2, ini[4]);
	vet[5] = criar_celula(2, 6, ini[5]);
	vet[6] = criar_celula(3, 1, ini[6]);
	vet[7] = criar_celula(4, 2, ini[7]);
	vet[8] = criar_celula(3, 3, ini[8]);
	vet[9] = criar_celula(6, 6, ini[8]);
	for(int i = 0; i < MAX; i++)
		inserir(vet[i]);
	imprimir_matriz();
	
	do {
	    int scan = 0;
	    printf("\n==== MENU ====\n");
        printf("1. Mostrar estacionamento\n");
        printf("2. Estacionar na vaga\n");
        printf("3. Liberar/sair da vaga\n");
        printf("4. Exibir vagas ocupadas\n");
        printf("5. Encerrar o programa\n");
        printf("==============\n");
        printf("\nDigite a opcao desejada: ");
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ função para impedir que usuario digite letra e trave o programa
        scan = scanf ("%d",&opcao);
        if (scan < 1 || scan > 5){
            opcao = 6;
            scan = 6;
        }
        int gc;
        do {
            gc = getchar();
            if (gc == EOF) exit(EXIT_FAILURE);
        } while (gc != '\n');
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        switch (opcao) {
            
            case 1://Mostrar estacionamento
            
                if(vazia()) // verifica se a lista está vazia
                    printf("\nO estacionamento está vazio!\n");
                else
                    printf("\n\n");
                    imprimir_matriz();
                break;  
            //-----------------------------------------------------
            
            case 2://Estacionar na vaga
                printf("\nInsira o numero referente a linha na qual deseja estacionar: ");
                do{
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ função para impedir que usuario digite letra e trave o programa
                    scan = scanf ("%d",&lin);
                    if (scan < 1 || scan > 6)
                        lin = 7;
                    int gc;
                    do {
                     gc = getchar();
                        if (gc == EOF) exit(EXIT_FAILURE);
                    } while (gc != '\n');
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    if(lin < 1 || lin > 6) {
                        printf("Valor invalido. Favor inserir um numero de 1 e 6: ");
                    }
                } while(lin < 1 || lin > 6);
                printf("\nInsira o numero referente a coluna na qual deseja estacionar: ");
                do{
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ função para impedir que usuario digite letra e trave o programa
                    scan = scanf ("%d",&col);
                    if (scan < 1 || scan > 6)
                        col = 7;
                    int gc;
                    do {
                     gc = getchar();
                        if (gc == EOF) exit(EXIT_FAILURE);
                    } while (gc != '\n');
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
                    if(col < 1 || col > 6) {
                        printf("Valor invalido. Favor inserir um numero de 1 e 6: ");
                    }
                } while(col < 1 || col > 6);
                vet[9] = criar_celula(lin, col, 1);
		inserir(vet[9]);
                
                
                break;
            //-----------------------------------------------------
            
            case 3://Liberar/sair da vaga
                printf("\nInsira o numero referente a linha na qual deseja sair: ");
                do{
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ função para impedir que usuario digite letra e trave o programa
                    scan = scanf ("%d",&lin);
                    if (scan < 1 || scan > 5)
                        lin = 7;
                    int gc;
                    do {
                     gc = getchar();
                        if (gc == EOF) exit(EXIT_FAILURE);
                    } while (gc != '\n');
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    if(lin < 1 || lin > 6) {
                        printf("Valor invalido. Favor inserir um numero de 1 e 6: ");
                    }
                } while(lin < 1 || lin > 6);
                printf("\nInsira o numero referente a coluna na qual deseja sair: ");
                do{
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ função para impedir que usuario digite letra e trave o programa
                    scan = scanf ("%d",&col);
                    if (scan < 1 || scan > 5)
                        col = 7;
                    int gc;
                    do {
                     gc = getchar();
                        if (gc == EOF) exit(EXIT_FAILURE);
                    } while (gc != '\n');
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
                    if(col < 1 || col > 6) {
                        printf("Valor invalido. Favor inserir um numero de 1 e 6: ");
                    }
                } while(col < 1 || col > 6);
                deletar(lin, col);
                printf("\nVaga na linha %d , coluna %d, se encontra livre\n\n", lin, col);
                
                
                break;
            //-----------------------------------------------------
            
            case 4://Exibir vagas ocupadas
                printf("\nAs seguintes vagas estao ocupadas:\n");
	            imprimir_elementos(); // imprime a matriz
	        break;     
            
            //-----------------------------------------------------
            
            case 5://Encerrar o programa
                    printf("Encerrando o programa...\n");
            break;
             
            //-----------------------------------------------------       
            case 6://Opção invalida
                printf("\n\nOpcao invalida. Tente novamente.\n");
            break;
            //-----------------------------------------------------
            default:
                printf("\n\nOpcao invalida. Tente novamente.\n");
                break;
        
        }   
	} while (opcao != 5);
	//limpando a memoria do array
	free(ini);

	return 0;
}
