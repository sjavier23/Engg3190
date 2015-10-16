
#include<stdio.h>
#include<string.h>
#include<math.h>

int main(void){

char * string = "1001";
int sum, j, s;
int? num = NULL;
int zero = 0;

for(sum=0, j=0, s=strlen(string)-1; s >= 0; s--, ++j){
    if(string[s] == '1'){
        sum = sum + pow(2,j);
    }
}
printf("%d\n",sum);

if(num==zero){
	printf("Null is 0");
}

return 0;
}