#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int criarTempo (float tempo[1001]){ // Cria o vetor com os dados de tempo. A estrutura é igual a das próximas 2 funções, por isso vou comentar só nesse. (Tentamos fazer um função só mas não deu certo)
    /* Variáveis que serão usadas durante a função */
    int i = 0;
    char caracteres;
    FILE *varTempo;
	
	varTempo = fopen("Tempo.txt","r"); // Abre o arquivo em modo de leitura

	if (varTempo == NULL){ // Verifica se o arquivo existe, se não existir ele avisa e encerra o programa
		printf("Arquivo 'Tempo.txt' nao encontrado\n");
		exit(0);
	}

	fscanf(varTempo,"%f",&tempo[i]); // Lê e armazena a linha inteira como float
	caracteres = fgetc(varTempo); // Lê um caractere e avança o o ponteiro para o próximo
	i++;

	while (caracteres != EOF){ // Lê todo o arquivo e armazena cada linha em um campo do vetor 
		if (caracteres == '\n'){
			fscanf(varTempo,"%f",&tempo[i]);
			i++;
		}
	    caracteres = fgetc(varTempo);
	}

	fclose(varTempo); // Fecha o arquivo
}
int criarX(float posX[1001]){
    int i = 0;
    char caracteres;
    FILE *BolaX;
    
	BolaX = fopen("BolaX.txt","r");

	if (BolaX == NULL){
		printf("Arquivo 'BolaX.txt' nao encontrado\n");
		exit(0);
	}

	
	fscanf(BolaX,"%f",&posX[i]);
	caracteres = fgetc(BolaX);
	i++;

	while (caracteres != EOF){
		if (caracteres == '\n'){
			fscanf(BolaX,"%f",&posX[i]);
			i++;
		}
	caracteres = fgetc(BolaX);

	}
	fclose(BolaX);
}

int criarY(float posY[1001]){
    int i = 0;
    char caracteres;
    FILE *BolaY;
	
    
	BolaY = fopen("BolaY.txt","r");

	if (BolaY == NULL){
		printf("Arquivo 'BolaY.txt' nao encontrado\n");
		exit(0);
	}

	
	fscanf(BolaY,"%f",&posY[i]);
    caracteres = fgetc(BolaY);
    i++;

	while (caracteres != EOF){
		if (caracteres == '\n'){
            fscanf(BolaY,"%f",&posY[i]);
			i++;
		}
	caracteres = fgetc(BolaY);

	}
	fclose(BolaY);
}

int main(void){ // Função principal que chama as outras e roda o código de forma organizada
    /* Cria as variáveis e vetores que serão usados durante o código */
    // Parâmetros do robô
    int size = 1001;
    float tempo[size];
    criarTempo(tempo);
    float raio = 0.125;
    float velMax = 2.3;
    float aceleracao = 2.6;
    // Vetores com as informações da trajetória da bola
    float bolaX[size];
    float bolaY[size];
    criarX(bolaX);
    criarY(bolaY);
    float velBolaY[size];
    float velBolaX[size];
    float acelBolaX[size];
    float acelBolaY[size];
    // Vetores com as informações da trajetória do robô
    float roboX[size];
    float roboY[size];
    float velRobo[size];
    float velRoboX[size];
    float velRoboY[size];
    float acelRobo[size];
    float acelRoboX[size];
    float acelRoboY[size];
    // Vetores com outras informações
    float distancia[size]; // Vetor onde ficará a distância entre a bola e o robô conforme o tempo
    float cos[size];
    float sen[size];
    float varTempo;

    int i;

	printf("Insira a posicao inicial do robo X: ");
	scanf("%f", &roboX[0]);
    printf("Insira a posicao inicial do robo Y: ");
	scanf("%f", &roboY[0]);
	printf("As posicoes escolhidas foram: X = %.2f e Y = %.2f\n", roboX[0], roboY[0]);

    if (0 >= roboX[0] > 9){ // Verifica se o usuário digitou posições válidas
        if(0 >= roboY[0] > 6){        
    		printf("Insira uma posicao valida!\n");
		    exit(0);
        }
	}

    for (i=0; i<size; i++){ // Preenche os vetores de aceleração e velocidade da bola
      // A aceleração nos eixos é constante
      acelBolaX[i] = -0.01;
      acelBolaY[i] = -0.016;

      // A velocidade nos eixos varia por causa da aceleração
      velBolaX[i] = (-0.01*tempo[i]) + 0.5;
      velBolaY[i] = (-0.016*tempo[i]) + 0.4;
    }

    for (i=0; i<size; i++){ // Preenche os vetores de aceleração e velocidade do robô
      acelRobo[i] = 2.6;
      velRobo[i] = acelRobo[i] * tempo[i];

      // Limita a velocidade do robo
      if (velRobo[i] >= velMax){
          velRobo[i] = velMax;
          acelRobo[i] = 0;
      }
    }

    for(i=0; i<1002; i++){
        distancia[i] = sqrt( pow((bolaX[i] - roboX[i]), 2) + pow((bolaY[i] - roboY[i]), 2) );  // Pitágoras para encontrar a distancia entre o robo e a bola
        
        cos[i] = (bolaX[i] - roboX[i]) / distancia[i]; // cos = adjascente/hipotenusa
        sen[i] = (bolaY[i] - roboY[i]) / distancia[i]; // sen = oposto/hipotenusa

        velRoboX[i] = velRobo[i] * (cos[i]); // adjascente = hipotenusa * cos
        velRoboY[i] = velRobo[i] * (sen[i]); // oposto = hipotenusa * sen

        acelRoboX[i] = acelRobo[i] * (cos[i]); // adjascente = hipotenusa * cos
        acelRoboY[i] = acelRobo[i] * (sen[i]); // oposto = hipotenusa * sen

        roboX[i+1] = roboX[i] + (velRoboX[i] * 0.02) + (acelRoboX[i] * 0.0002); // Fórmula Sovetão
        roboY[i+1] = roboY[i] + (velRoboY[i] * 0.02) + (acelRoboY[i] * 0.0002); // Fórmula Sovetão
        // O 0.0002 é o delta T(0.02) ao quadrado (fica 0.0004) dividido por 2.
    
        // Para o código quando o robô intercepta a bola 
        if (roboX[i] >= bolaX[i] - raio && roboX[i] <= bolaX[i] + raio && roboY[i] >= bolaY[i] - raio && roboY[i] <= bolaY[i] + raio){
            printf ("O robo encontrou com a Bola! \n");
            printf("Infromacoes do Robo:\nRoboX: %.3f / RoboY: %.3f\nVelocidadeX: %.3f / VelocidadeY: %.3f\n", roboX[i], roboY[i], velRoboX[i], velRoboY[i]);
            printf("Informacoes da Bola:\nBolaX: %.3f / BolaY: %.3f\nVelocidadeX: %.3f / VelocidadeY: %.3f\n", bolaX[i], bolaY[i], velBolaX[i], velBolaY[i]);
            printf("Tempo decorrido: %.3fs\n", tempo[i]);
        
            break;
        }
        // Enquanto o robô não intercepta a bola, ele mostra as informações do robô e da bola
        else{
            printf("Distancia: %.3f\n", distancia[i]);
            printf("Infromacoes do Robo:\nRoboX: %.3f / RoboY: %.3f\nVelocidadeX: %.3f / VelocidadeY: %.3f\n", roboX[i], roboY[i], velRoboX[i], velRoboY[i]);
            printf("Informacoes da Bola:\nBolaX: %.3f / BolaY: %.3f\nVelocidadeX: %.3f / VelocidadeY: %.3f\n", bolaX[i], bolaY[i], velBolaX[i], velBolaY[i]);
            printf("Tempo decorrido: %.3fs\n", tempo[i]);     
            printf("\n_________________\n");      
        }
    }
    system("pause");
}
