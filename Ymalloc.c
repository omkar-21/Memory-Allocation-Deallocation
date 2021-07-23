#include<stdio.h>
#include<stdlib.h>
#include"Yalloc.h"

int main(){
	memory=(char *)malloc(40000*(sizeof(char)));
	freelist=(void *)memory; 	 
	char *array,*array2,*array3;
	int i;
	array = (char *)Yalloc(5*sizeof(char));
	for (i = 0; i < 5; i++) {

		printf("address 1= %p\n",(array+i));		
	}
	for(i=0;i<5;i++)
	{
		scanf("%s",&array[i]);
	}

	array2 = (char *)Yalloc(5*sizeof(char));

	for (i = 0; i < 5; i++) {

		printf("address 2= %p\n",(array2+i));
	}

	for(i=0;i<5;i++)
	{
		scanf("%s",&array2[i]);
	}
	
	Yfree(array2);
    
	array3 = (char *)Yalloc(5*sizeof(char));
	for (i = 0; i < 5; i++) {

		printf("address 3= %p\n",(array3+i));		
	}
	for(i=0;i<5;i++)
	{
		scanf("%c",&array3[i]);
	}

	Yfree(array);
	Yfree(array3);

	return 0;

}


