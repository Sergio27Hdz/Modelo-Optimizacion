#include <stdio.h>
#include <conio.h>
#include <windows.h>

//Variables Globales.
int cantFil, cantCol;
double simplex[100][100]; //cambiamos double por que los valores eran muy grandes


//FUNCION PARA MANDAR IMPRIMIR LAS MATRICES(TABLAS).
void imprimirMatriz(){
	system("cls");
	for(int i=0; i<cantFil; i++){
		for(int j=0; j<cantCol; j++){
			printf("%.3f\t", simplex[i][j]);
		}
		printf("\n");
	}
	getch();
}






//FUNCION PRINCIPAL MAIN
int main(){
	
	//Declaramos variables
	int cantVarX, cantVarHolg, cantVarExc, cantRes, cantVarArt, modo, a = 0;
	
	//Damos color a nuestras letras de fondo y es una funcion de biblioteca windows.h.
	system("color c");

	//Encabezado del Simplex.
	printf("|---------------------------------------------------------------|");
	printf("\n|                        METODO SIMPLEX                         |");
	printf("\n|---------------------------------------------------------------|\n");




	//Pedimos al usuario la cantidad de variables de nuestra funcion objetivo.
	printf("Cantidad de variables X: ");
	scanf("%d", &cantVarX);
	//Pedimos al usuario la cantidad de variables de holgura que tendran las restricciones.
	printf("\nCantidad de variables de Holgura: ");
	scanf("%d", &cantVarHolg);
	//Pedimos al usuario la cantidad de variables de excedente que tendran las restricciones.
	printf("\nCantidad de variables de Excedente: ");
	scanf("%d",&cantVarExc);
	//Pedimos al usuario el total de las restricciones.
	printf("\nDigite el total de las restricciones: ");
	scanf("%d", &cantRes);
	//Pedimos al usuario el total de las variables artificiales.
	printf("\nCantidad de variables Artificiales: ");
	scanf("%d", &cantVarArt); 





		
	//Do-while para que el usuario no ponga valores que no son.
	do{ 
		//Decide el tipo de proceso.
		printf("\nMaximizar (1) o Minimizar (0)?: "); 
		scanf("%d", &modo);
	}while(modo != 1 && modo != 0);
	
	//Establecemos el tama�o de nuestra matriz.	
	cantFil = cantRes+1;
	cantCol = cantVarX+cantVarHolg+cantVarExc+cantVarArt+2;
	//Borrado de pantalla
	system("cls");	 



	//For para rellenar nuestra tabla simplex.
	for(int i=0; i<cantFil; i++){
		system("cls");
		for(int j=0; j<cantCol; j++){
			printf("\nIngrese el valor[%d][%d]: ",i,j);
			scanf("%lf", &simplex[i][j]);		
		}
	}	

	//Borrado de pantalla 
	system("cls");
		
	//Funcion para mandar a imprimir nuestra tabla.
	imprimirMatriz();



	
	//Para realizar el metodo de la M Grande.
	double m_grande = simplex[0][cantCol-2]; //cambiamos
	//La variable suma necesita sumar 1 extra para llegar al Valor A
	int suma = 1 + cantVarX + cantVarHolg + cantVarExc;
	int Posicion [cantVarArt];
	int k=0;
	if(cantVarArt > 0){
		//Encuentra las filas por la que hay que multiplicar 
		for(int i=suma; i<=cantCol-2; i++){
			for(int j=1; j<=cantRes; j++){
				if(simplex[j][i] == 1){
					Posicion[k]= j;
				}
			}
			k++;
		}
		
		//Realiza la operaci�n M-Grande dentro del rengl�n 0
		m_grande= m_grande*-1;
		 k=0;		
		for (int i=0; i<cantFil; i++){
			for(int j=0; j<cantCol; j++){
				if(Posicion[k] == i){
					simplex [0][j] = simplex[0][j] + (simplex[i][j]*m_grande); 
					if (j == cantCol-1){
						k++;
					}
				}
			}
		}
	}
	
	imprimirMatriz();
		
	//Decision para correr el proceso de maximizacion y minimizacion.
	if(modo == 0){	//MINIMIZACION.
		for(int i=1; i<cantCol; i++){
			simplex[0][i] = simplex[0][i] * -1;
		}
	}
	


	//Funcion para mandar a imprimir nuestra tabla.
	imprimirMatriz();
	system("color d");
	
	
	
	//Do-While para volver a repetir el proceso del numero menor.
	do{
		//For para encontrar la variable de la X, mas peque�o para despues hacer la prueba del cociente.
		double menor = 0;
		int pivCol = 0;
		for(int i=1; i<suma; i++){	//Terminamos el recorrido hasta donde llegan las excedente
			if(simplex[0][i] < menor){
				menor = simplex[0][i];
				pivCol = i;
			}
		}
		
		a = 0;
		//Para mandar hacer la division y sacar el menor en la prueba del cociente.
		double pruebaCociente[cantFil]; //cambiamos
		pruebaCociente[0]=0;
		for(int i=1; i < cantFil; i++){
			if(simplex[i][pivCol] <= 0){
				a++;
				pruebaCociente[i] = 0;
			}
			else{
				pruebaCociente[i] = simplex[i][cantCol-1]/simplex[i][pivCol];
				
				//checa si el resultado en la prueba coeficiente da cero
				if(pruebaCociente[i]==0){
					a++;
				}
			}
		}	
		
		//Para mandar a una solucion no acotada	
		if (a==cantRes){
			printf("\nSolucion NO ACOTADA!");
			exit(-1);
		}
		
		//Para encontrar el valor mas peque�o de la prueba de cociente
		menor = 999999999999; //Para compararlo y sacar el menor de la prueba de cociente.
		int pivFil = 1;
		for(int i=1; i < cantFil; i++){
			if (pruebaCociente[i] < menor && pruebaCociente[i] > 0){
				menor = pruebaCociente[i];
				pivFil = i;
			}
		}
		
		//Divide la fila pivote entre el pivote, para convertirlo en 1.
		double pivote = simplex[pivFil][pivCol]; //cambiamos
		for(int i=0; i<cantCol; i++){
			simplex[pivFil][i] = simplex[pivFil][i] / pivote;
		}
		
		//For para hacer cero la columna del pivote.
		double pivTemporal; //double
		
		for(int i=0; i<=cantFil; i++){
			if(i != pivFil){
				pivTemporal = simplex[i][pivCol] * -1;
				for(int j=0; j<cantCol; j++){
					simplex[i][j] = simplex[i][j] + (simplex[pivFil][j] * pivTemporal);
				}
			}		
		}
			
			
			
		imprimirMatriz();
		 
		 //Para saber si existen variables o coeficientes negativos en el renglon 0
		 a = 0;
		 for(int i=1; i<suma; i++){
		 	if(simplex[0][i] < 0){
		 		a++;
			 }	
		 }
		 
	}while(a>0);
	
	imprimirMatriz();
	
	//for para verificar si en el renglon cero existen mas ceros que restricciones entonces es solucion multiple.
	a = 0;	
	for(int i=1; i<suma; i++){	//Se agrego suma para que no cuente variables artificiales
		if(simplex[0][i] == 0){
			a++;
		}
	}
	
	int prueba[cantVarArt];	


	//Proceso para identificar si es No factible o no.
	if(cantVarArt > 0){

		for(int i=suma; i<=cantCol-2; i++){
			for(int j=1; j<=cantRes; j++){
				if(simplex[j][i] == 1 && Posicion[j-1] == j){
					if(simplex[0][i]==0 && simplex[j][cantCol-1]>0){
						printf("\n Solucion NO FACTIBLE!!");
						exit(-1);
					}
				}
			}
		}	
	}






	//Esto permite regresar la estabilidad del metodo simplex cuando fue multipicada por -1
	if(modo==0){
		for(int i=1;i<cantCol;i++){
			simplex[0][i]=simplex[0][i]*-1;
		}
		imprimirMatriz();	
	}





	//If para ver si la solucion del problema es solucion optima o multiple.	
	if(a > cantRes){
		printf("\nSolucion Optima MULTIPLE!");
		//Para impirmir el valor de Z con su resultado de b.
		printf("\nZ = %.2f", simplex[0][cantCol-1]);
	} else {
		printf("\nSolucion Optima");
		//Para impirmir el valor de Z con su resultado de b.
		printf("\nZ = %.2f", simplex[0][cantCol-1]);
	}
	

	

	 			
	getch();
	return 0;
}

