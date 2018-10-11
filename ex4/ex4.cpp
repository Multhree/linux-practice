#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE *input;
	FILE *output;
	char ch;
	char str[100];
	int strlen = 0;
	char word[100];
	int wordlen = 0;
	input = fopen("input.txt","wt+");
	if(input == NULL)
	{
		printf("Cannot open file strike any key exit!");
        exit(1);
	}
	else
	{
		printf("input a string:\n");
	    ch = getchar();
	    while(ch != '\n')
	    {
	    	fputc(ch,input);
	    	ch = getchar();
		}
		fputc('\n',input);
		rewind(input);
		while(ch != EOF)
		{
			ch = fgetc(input);
			str[strlen++] = ch;
		}
		fclose(input);
		output = fopen("output.txt","wt+"); 
		for(int i = strlen-1;i >= 0;i--)
		{
			if((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
			{
				word[wordlen++] = str[i];
			}
			else if(str[i] == ' ')
			{
				for(int j = wordlen-1;j >= 0;j--)
				{
					fputc(word[j],output);
				}
				wordlen = 0;
				fputc('/',output);
			}
			else
			{
				fputc(str[i],output);
			}
		}
		for(int j = wordlen-1;j >= 0;j--)
		{
			fputc(word[j],output);
		}
		fclose(output);
	}
	return 0;
}
