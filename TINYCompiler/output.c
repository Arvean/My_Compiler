#include <stdlib.h>
#include <stdio.h>
int main(void){
int x;
int foobar; 
x=5;
if (x==5){
x=x+1;
}
while (x<10){
x=x+1;
}
printf("%i\n", x);
scanf("%i", &foobar);
printf("%i\n", foobar);
return 0;
}
