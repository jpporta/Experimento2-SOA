/*******************************************************************************
*
* Este programa faz parte do curso sobre tempo real do Laboratorio Embry-Riddle
*
* Seguem os comentarios originais:
*
* Experiment #3: Shared Resources, Measureing Message Queue Transfer Time
*
*    Programmer: Eric Sorton
*          Date: 2/11/97
*           For: MSE599, Special Topics Class
*
*       Purpose: The purpose of this program is to measure the time it takes
*                a message to be transfered across a message queue.  The
*                total time will include the time to make the call to msgsnd(),
*                the time for the system to transfer the message, the time
*                for the context switch, and finally, the time for the other
*                end to call msgrcv().
*
*                The algorithm for this program is very simple:
*
*                   o The parent creates the message queue
*                   o The parents starts two children
*                   o The first child will:
*                         - Receive a message on the queue
*                         - Call gettimeofday() to get the current time
*                         - Using the time in the message, calculate
*                              the difference and store it in an array
*                         - Loop (X number of times)
*	              - Display the results
*                   o The second child will:
*                         - Call gettimeofday() to get the current time
*                         - Place the time in a message
*                         - Place the message on the queue
*                         - Pause to allow the other child to run
*                         - Loop (X number of times)
*                   o The parent waits for the children to finish
*
* Traduzindo:
*
*     Proposito: O proposito deste programa é a medicao do tempo que leva
*                uma mensagem para ser transferida por uma fila de mensagens.
*                O tempo total incluira o tempo para realizar a chamada
*                msgsnd(), o tempo para o sistema transferir a mensagem, o
*                tempo para troca de contexto e, finalmente, o tempo para,
*                na outra ponta, ocorrer a chamada msgrcv().
*
*                O algoritmo para este programa e bastante simples:
*
*                   o O pai cria a fila de mensagens
*                   o O pai inicializa dois filhos
*                   o O primeiro filho:
*                         - Recebe uma mensagem pela fila
*                         - Chama gettimeofday() para obter o tempo atual
*                         - Usando o tempo existente na mensagem, calcula
*                              a diferenca
*                         - Repete (numero X de vezes)
*				  - Exibe os resultados
*                   o O segundo filho:
*                         - Chama gettimeofday() para obter o tempo atual
*                         - Coloca o tempo em uma mensagem
*                         - Coloca a mensagem na fila
*                         - Realiza uma pausa para permitir a execucao do irmao
*                         - Repete (numero X de vezes)
*                   o O pai espera os filhos terminarem
*
*******************************************************************************/

/*
 * Includes Necessarios
 */
#include <sys/time.h>       /* for gettimeofday() */
#include <stdio.h>      /* for printf() */
#include <unistd.h>     /* for fork() */
#include <sys/types.h>      /* for wait(), msgget(),msgctl() */
#include <wait.h>       /* for wait() */
#include <sys/ipc.h>            /* for msgget(), msgctl() */
#include <sys/msg.h>            /* for msgget(), msgctl() */
#include <math.h>
#include <stdlib.h>

/*
 * NO_OF_ITERATIONS corresponde ao numero de mensagens que serao enviadas.
 * Se este numero cresce, o tempo de execucao tambem deve crescer.
 */
#define NO_OF_ITERATIONS    500

/*
 * MICRO_PER_SECOND define o numero de microsegundos em um segundo
 */
#define MICRO_PER_SECOND    1000000

/*
 * MESSAGE_QUEUE_ID eh uma chave arbitraria, foi escolhido um numero qualquer,
 * que deve ser unico. Se outra pessoa estiver executando este mesmo programa
 * ao mesmo tempo, o numero pode ter que ser mudado!
 */
#define MESSAGE_QUEUE_ID1   3102
#define MESSAGE_QUEUE_ID2   3301

/*
 * Constantes
 */
#define SENDER_DELAY_TIME   10
#define MESSAGE_MTYPE       1

#define NO_OF_CHILDREN 3

/*
 * Filhos
 */
void Receiver1(int queue_idREC, int queue_idENV);
void Receiver2(int queue_idREC);
void Sender(int queue_idENV);
/*
 * Pergunta 1: O que eh um prot�tipo? Por qual motivo eh usado?
 */
long tamanho;
/*
 * Programa principal
 */
int main( int argc, char *argv[] )
{
	/*
	 * Algumas variaveis necessarias
	 */
	pid_t rtn;
	int count = 10;

	/*
	 * Variaveis relativas a fila, id e key
	 */
	int queue_id; // NAO SEI ESSO AKI
	key_t key1 = MESSAGE_QUEUE_ID1;
	key_t key2 = MESSAGE_QUEUE_ID2;

	int queue_idREC;
	int queue_idENV;

	do {
		printf("%s", "Insira o tamanho da mensagem (0 a 10): ");
		scanf("%ld", &tamanho);
	} while(tamanho < 1 || tamanho > 10);
	tamanho = tamanho*512; // tamanho = 0.5KB * numero
	/*
	 * Pergunta 2: O que significa cada um dos digitos 0666? R: Deixa o cara criar com permisaao de leitura escrita
	 * Pergunta 3: Para que serve o arquivo stderr?
	 * Pergunta 4: Caso seja executada a chamada fprintf com o handler stderr, onde aparecer� o seu resultado?
	 * Pergunta 5: Onde stderr foi declarado?
	 */

	/*
	 * Pergunta 6: Explicar o que s�o e para que servem stdin e stdout.
	 */

	/*
	 * Inicializa dois filhos
	 */
	rtn = (int)1;
	for( count = 0; count < NO_OF_CHILDREN; count++ ) {
		if( (int)rtn != 0 ) {
			rtn = fork();
		} else {
			break;
		}
	}

	/*
	 * Verifica o valor retornado para determinar se o processo eh pai ou filho
	 *
	 * OBS:  o valor de count no loop anterior indicara cada um dos filhos
	 *       count = 1 para o primeiro filho, 2 para o segundo, etc.
	 */
	if( rtn == 0 ) {
		if( (queue_idREC = msgget(key1, IPC_CREAT | 0666)) == -1 ) {
			fprintf(stderr,"Impossivel criar a fila de mensagens!\n");
			exit(1);
		}
		if( (queue_idENV = msgget(key2, IPC_CREAT | 0666)) == -1 ) {
			fprintf(stderr,"Impossivel criar a fila de mensagens!\n");
			exit(1);
		}

		if(count == 1) {
			Receiver1(queue_idREC, queue_idENV);
			if( msgctl(queue_idREC,IPC_RMID,NULL) == -1 ) {
				fprintf(stderr,"Impossivel remover a fila! REC\n");
				exit(1);
			}
			exit(0);
		}
		if(count == 2) {
			Sender(queue_idREC);
			exit(0);
		}
		if(count == 3) {
			Receiver2(queue_idENV);
			if( msgctl(queue_idENV,IPC_RMID,NULL) == -1 ) {
				fprintf(stderr,"Impossivel remover a fila! ENV\n");
				exit(1);
			}
			exit(0);
		}
	}

	else
	{
		/*
		 * Sou o pai aguardando meus filhos terminarem
		 */
		//printf("Pai aguardando ...\n");
		wait(0);
		wait(0);
		wait(0);

		/*
		 * Pergunta 7: O que ocorre com a fila de mensagens, se ela n�o � removida e os
		 * processos terminam?
		 */
		exit(0);
	}
}

/*
 * O tipo de dados seguinte pode ser usado para declarar uma estrutura que
 * contera os dados que serao transferidos pela fila.  A estrutura vai conter
 * um numero de mensagem (msg_no) e o tempo de envio (send_time).  Para filas
 * de mensagens, o tipo da estrutura pode definir qualquer dado que seja necessario.
 */
typedef struct {
	unsigned int msg_no;
	struct timeval send_time;
} data_t1;
typedef struct {
	double tempMax;
	double tempMed;
	double tempMin;
	double tempTotal;
} data_t2;

/*
 * O conteudo de uma estrutura com o seguinte tipo de dados sera enviado
 * atraves da fila de mensagens. O tipo define dois dados.  O primeiro eh
 * o tipo da mensagem (mtype) que sera como uma identificacao de mensagem.
 * Neste exemplo, o tipo eh sempre o mesmo. O segundo eh um vetor com tamanho
 * igual ao definido pelo tipo declarado anteriormente. Os dados a serem
 * transferidos sao colocados nesse vetor. Na maioria das circunstancias,
 * esta estrutura nao necessita mudar.
 */
typedef struct {
	long mtype;
	char mtext[8192];
} msgbuf_t;

/*
 * Esta funcao executa o recebimento das mensagens
 */
void Receiver2(int queue_idREC)
{
	msgbuf_t message_buffer;

	int count;

	data_t2 *data_ptrENV = (data_t2 *)(message_buffer.mtext);
	if( msgrcv(queue_idREC,(struct msgbuf_t *)&message_buffer,tamanho,MESSAGE_MTYPE,0) == -1 ) {
		fprintf(stderr, "Impossivel receber mensagem!\n");
		exit(0);
		// printf("Not RECIVED\n");
		// sleep(2);
	}
	printf("%lf,%lf,%lf,%lf\n", data_ptrENV->tempMin, data_ptrENV->tempMed,
	       data_ptrENV->tempMax,data_ptrENV->tempTotal);

}
void Receiver1(int queue_idREC, int queue_idENV)
{
	/*
	 * Variaveis locais
	 */
	int count;
	struct timeval receive_time;
	double delta;
	double max = 0;
	double total = 0;
	double min = INFINITY;

	msgbuf_t message_bufferREC;
	msgbuf_t message_bufferENV;

	data_t1 *data_ptrREC = (data_t1 *)(message_bufferREC.mtext);
	data_t2 *data_ptrENV = (data_t2 *)(message_bufferENV.mtext);

	for( count = 0; count < NO_OF_ITERATIONS; ++count ) {

		usleep(10000); //MELHOR VISUALIZACAO NO WATCH IPCS
		if( msgrcv(queue_idREC,(struct msgbuf_t *)&message_bufferREC,tamanho,MESSAGE_MTYPE,0) == -1 ) {
			fprintf(stderr, "Impossivel receber mensagem!\n");
			exit(1);
		}

		gettimeofday(&receive_time,NULL);

		/*
		 * Calcula a diferenca
		 */
		delta = receive_time.tv_sec  - (data_ptrREC->send_time).tv_sec;
		delta += (receive_time.tv_usec - (data_ptrREC->send_time).tv_usec)/(float)MICRO_PER_SECOND;
		total += delta;

		// * Salva o tempo maximo e minimo

		if (delta < min) min = delta;
		if (delta > max) max = delta;
	}

	//Mensagem para o Reciver2
	message_bufferENV.mtype = MESSAGE_MTYPE;
	data_ptrENV->tempTotal = total;
	data_ptrENV->tempMed = (total / NO_OF_ITERATIONS);
	data_ptrENV->tempMin = min;
	data_ptrENV->tempMax = max;
	if( msgsnd(queue_idENV,(struct msgbuf_t *)&message_bufferENV,tamanho,IPC_NOWAIT) == -1 ) {
		// fprintf(stderr, "Impossivel enviar mensagem!\n");
		// exit(1);
		usleep(500);
		//printf("NOT SENDING\n");
	}
	return;
}

/*
 * Esta funcao envia mensagens
 */
void Sender(int queue_idENV)
{
	//int queue_idENV;
	int count;
	struct timeval send_time;

	msgbuf_t message_buffer;

	data_t1 *data_ptr = (data_t1 *)(message_buffer.mtext);

	/*
	 * Inicia o loop
	 */
	for( count = 0; count < NO_OF_ITERATIONS; count++ ) {

		gettimeofday(&send_time,NULL);

		message_buffer.mtype = MESSAGE_MTYPE;
		data_ptr->msg_no = count;
		data_ptr->send_time = send_time;

		if( msgsnd(queue_idENV,(struct msgbuf_t *)&message_buffer,tamanho,0) == -1 ) {
			fprintf(stderr, "Impossivel enviar mensagem!\n");
			exit(1);
		}

		usleep(SENDER_DELAY_TIME);
	}
	return;
}

