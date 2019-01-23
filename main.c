#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//calcula y_in
void calcula_yin7x63(float b [], float entrada[], float weight [][63], float y[]){
    float y_aux=0;
    int i=0;
    int j=0;
    for(; i < 7; i++){
        for(;j<63;j++){
            y_aux=(float)y_aux+(float)entrada[j]*weight[i][j];
        }
        j=0;
        // printf("%.1f ",y_aux);
        y[i]= (float)b[i]+y_aux;
        y_aux=0;
    }

}

void zera(float entrada[], int n){
    for(int j =0; j < n;j++){
        entrada[j]=0;
    }


}

//calcula y_out
void y_out7x7(float yi [] ,float y_out[],float thresh){

        for(int i=0; i < 7; i++ ) {
            if (yi[i] > thresh) {
                y_out[i] = (float) 1;

            } else if (((-1 * thresh) <= yi[i]) && (yi[i] <= thresh)) {

                y_out[i]= (float) 0;

            } else if (yi[i] < (-1 * thresh)) {
                y_out[i] = (float) -1;
            }
        }
     }


//atualiza os pesos1
void update_w7x63(float entrada [], float wi [][63], float bias [] ,float t [] , float tx){
    for(int i=0; i < 7 ;i++){
        for(int j=0; j< 63; j++){
            wi[i][j]=wi[i][j] + (t[i]*entrada[j]*tx);

        }
        bias[i]= bias[i] + ( t[i] * tx);
    }

}

//atualiza os pesos2
void update_w7x63_2(float entrada [], float wi [], float bias [] ,float t  , float tx,int i){

        for(int j=0; j< 63; j++){
            wi[j]=wi[j] + (t*entrada[j]*tx);

        }
        bias[i]= bias[i] + ( t * tx);
    }




int match7(float e [], float *p){
    int aux=0;
    for(int i =0;i <7;i++){

        if(e[i] == p[i]){

            aux++;

        }else{

            aux--;
        }

    }
    if(aux == 7 ){
        aux=1;
    }

    return aux;
}

//print Saida

void resultado_character(float yo [], float tr [][7]){
    int n=3;
    int aux=1;
    for(int i = 0; i < 7; i++){

        n=match7(yo,tr[i]);
       // printf("%d \n",n);
        if(n == 1){
            switch(i) {
                case 0:
                    printf("A ");
                    break;
                case 1:
                    printf("B ");
                    break;
                case 2:
                    printf("C ");
                    break;
                case 3:
                    printf("D ");
                    break;
                case 4:
                    printf("E ");
                    break;
                case 5:
                    printf("J ");
                    break;
                case 6:
                    printf("K ");
                    break;
                default:
                    printf("Erro");
                    break;
            }
            aux=0;

        }


    }
    if(aux){

        printf("Nenhum caracter reconhecdo\n");
    }


}

//set input, função q carrega padrões de um arquivo txt(21 caracteres  9X7)
void get_inputx63(FILE *prt,float v_input [21] [63]){
    int x=0;
    float c =0;
    int count=0;

    for(int i =0;i<21;i++) {
        while (count != 63) {
            x = fgetc(prt);
            if ((x != ' ') && (x != '\n')) {


                if(x == '#'){
                    c=(float)1;

                    v_input[i][count]=c;
                }
                else if(x == '.'){
                    c=(float)-1;
                    v_input[i][count]=c;
                } else if(x == '@'){
                    c=(float)1;
                    v_input[i][count]=c;
                }else if(x=='O'){
                    c=(float)-1;
                    v_input[i][count]=c;
                }
                count++;

            }

            if (count % 7 == 0) {


            }


        }

        count=0;
    }



}
//get entrada misturada
void get_input42x63(FILE *prt,float v_input [42] [63]){
    int x=0;
    float c =0;
    int count=0;

    for(int i =0;i<42;i++) {
        while (count != 63) {
            x = fgetc(prt);
            if ((x != ' ') && (x != '\n')) {


                if(x == '#'){
                    c=(float)1;

                    v_input[i][count]=c;
                }
                else if(x == '.'){
                    c=(float)-1;
                    v_input[i][count]=c;
                } else if(x == '@'){
                    c=(float)1;
                    v_input[i][count]=c;
                }else if(x=='O'){
                    c=(float)-1;
                    v_input[i][count]=c;
                }
                count++;

            }

            if (count % 7 == 0) {


            }


        }

        count=0;
    }



}

int main(int argc, char** argv) {


    float *vector_treinamento;
    vector_treinamento = (float*)  calloc(63,sizeof(float)); //teste

    float inputx63  [21][63]; //para 21 entradas
    float input41   [42] [63]; //para 42 entradas
    int  nrow_entrada = 9;
    int ncol_entrada = 7;

    float target [7][7]={{1,-1,-1,-1,-1,-1,-1}, //A
                         {-1,1,-1,-1,-1,-1,-1}, // B
                         {-1,-1,1,-1,-1,-1,-1}, //C
                         {-1,-1,-1,1,-1,-1,-1}, // D
                         {-1,-1,-1,-1,1,-1,-1}, // E
                         {-1,-1,-1,-1,-1,1,-1}, // J
                         {-1,-1,-1,-1,-1,-1,1}};//K

    const int tamanho_target=7;
    float threshold = 0.0;
    float  tx_l=1;
    float *bias;
    bias = (float*) calloc(7,sizeof(float));
    int nrow_w =7 ;
    int ncol_w = 63;
    float w [7] [63];
    int has_change=0;
    float *y_in;
    y_in = (float*)calloc(7, sizeof(float));

    float *y;
    y= (float*)calloc(7, sizeof(float));
    int count=0;
 int n_epocas=0;
    //Arquivos
    //treinamento.txt = arquivo de treinamento da rede figura 2.20 21 letras
    //entrada.txt   = arquivo de entrada com ruido figura 2.22    21 letras
    //entrada_mix.txt = arquivo com entrada misturada   21 letras
    FILE *ptr;
    if ((ptr = fopen("treinamento.txt", "r")) == NULL)
    {
        printf("Error! Verifique se o arquivo esta no diretorio corrente");
        // Program exits if file pointer returns NULL.
        exit(1);
    }
    //get  file
    get_inputx63(ptr,inputx63);
    fclose(ptr);

     //inicia matriz de pesos com 0
    for(int i =0; i<7;i++){
        for(int j =0; j < 63;j++){
            w[i][j]=0.0;
        }


    }
    zera(bias,7);
    zera(y_in,7);
    zera(y,7);

    //Procedimento
    int flag =0;
    int aux=0;

    printf("Questao 01\n");
    while(1){
        count=0;
        for(int g=0;g<21;g++){

            calcula_yin7x63(bias,inputx63[g],w,y_in);
            y_out7x7(y_in,y,threshold);
            //combinacao entre padrão e target

            if(aux % 7 == 0){
                aux=0;
            }

           // printf("%d a\n",aux);
            for(int p=0;p<7;p++){
                if(y[p] != target[aux][p]){
             //       printf("G:%d Aux:%d P:%d T:%.1f Y_o:%.1f\n",g,aux,p,target[aux][p],y[p]);
                    update_w7x63_2(inputx63[g],w[p],bias,target[aux][p],tx_l,p);

                }else{
                    count++;

                }

            }
            aux++;

        }
        aux=0;



        n_epocas++;
        printf("Epoca: %d\n",n_epocas);

        if(count == 147)break;
    }


    
  

    //Testes apos treinamento
    //Teste   (letra A fonte 1)
    printf("Teste treinamento, Entrada A:\n");
    calcula_yin7x63(bias,inputx63[0],w,y_in);
    y_out7x7(y_in,y,threshold);
    resultado_character(y,target);
    /*
    //Teste  (letra B fonte 2)
    printf("Entrada B:\n");
    calcula_yin7x63(bias,inputx63[8],w,y_in);
    y_out7x7(y_in,y,threshold);
    resultado_character(y,target);
    //Teste (letra C fonte 3)
    printf("Entrada C:\n");
    calcula_yin7x63(bias,inputx63[16],w,y_in);
    y_out7x7(y_in,y,threshold);
    resultado_character(y,target);

*/
    printf("Questao 2:\nReconhencendo com base na ordem da figura 2.22, 21 entradas com ruidos de 0 ate 20\n");

    if ((ptr = fopen("entrada.txt", "r")) == NULL)
    {
        printf("Error! Certifique-se que o arquivo entrada.txt esta no diretorio");
        // Program exits if file pointer returns NULL.
        exit(1);
    }
    //get  file
    get_inputx63(ptr,inputx63);
    fclose(ptr);

    for(int v=0;v<21;v++) {
        printf("Entrada[%d]\n",v);
        calcula_yin7x63(bias, inputx63[v], w, y_in);
        y_out7x7(y_in, y, threshold);
        resultado_character(y,target);
        printf("\n");
    }

    printf("Questao 3:\nTreinando para 42 entradas com figuras misturadas 2.20 e 2.21, uma apos outra\n");

    if ((ptr = fopen("entrada_mix.txt", "r")) == NULL)
    {
        printf("Error! Certifique-se que o arquivo entrada_mix.txt esta no diretorio");
        // Program exits if file pointer returns NULL.
        exit(1);
    }
    //get  file
    get_input42x63(ptr,input41);
    fclose(ptr);
     flag =0;
     aux=0;
    n_epocas=0;
    while(1){
        count=0;
        for(int g=0;g<42;g++){

            calcula_yin7x63(bias,input41[g],w,y_in);
            y_out7x7(y_in,y,threshold);
            //combinacao entre padrão e target

            if (g < 21){
                if(aux % 7 == 0){
                    aux = 0;
                }
            }else {
                if (aux % 7 == 6) {
                    aux = 0;
                }
            }
            // printf("%d a\n",aux);
            for(int p=0;p<7;p++){
                if(y[p] != target[aux][p]){
                    //       printf("G:%d Aux:%d P:%d T:%.1f Y_o:%.1f\n",g,aux,p,target[aux][p],y[p]);
                    update_w7x63_2(input41[g],w[p],bias,target[aux][p],tx_l,p);

                }else{
                    count++;

                }

            }
            aux++;

        }
        aux=0;



        n_epocas++;
        printf("Epoca: %d\n",n_epocas);

        if(count == 294)break;
    }
    printf("Reanalizando entradas com ruidos\n" );
    for(int v=0;v<21;v++) {
        printf("Entrada[%d]\n",v);
        calcula_yin7x63(bias, inputx63[v], w, y_in);
        y_out7x7(y_in, y, threshold);
        resultado_character(y,target);
        printf("\n");
    }
    char cr;
    scanf("Enter para sair",&cr);
    free(vector_treinamento);
    free(bias);
    free(y_in);
    free(y);


}