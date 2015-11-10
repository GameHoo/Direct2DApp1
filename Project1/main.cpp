// in/out
#include<iostream>
#include<stdio.h>
int main()
{
	int *p, a;
	a = 5;
	p = &a;
	printf("&p=%x &a=%x", &p, &a);
	
	getchar();
	return 0;
}