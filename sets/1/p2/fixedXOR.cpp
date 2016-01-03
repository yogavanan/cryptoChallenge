#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

const char hextable[] =
{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

char hextodec(char hex)
{
	char ret = 0XFF;
	if (hex >= '0' && hex <= '9')
	{
		ret = hex - '0';
	}
	else if (hex >= 'A' && hex <= 'F')
	{
		ret = hex - 'A' + 10;
	}
	else if (hex >= 'a' && hex <= 'f')
	{
		ret = hex - 'a' + 10;
	}

	return ret;
}


char * stringXOR(char * input1, char * input2)
{
	char * result = NULL;
	int i = 0;
	int length = strlen(input1);
	int temp = 0;

	result = (char *) malloc(length+1);
	memset(result, 0, length+1);

	for (i = 0; i < length; i++)
	{
		temp = hextodec(input1[i]) ^ hextodec(input2[i]);

		if (temp > 15)
		{
			temp = 15;
		}
		result[i] = hextable[temp];
	}

	return result;
}

int main(int argc, char ** argv)
{
	int retVal = 0;
	char * input1 = NULL, * input2 = NULL;
	char * result = NULL;

	do
	{
		if (argc != 3)
		{
			printf("USAGE: %s <String 1> <String 2>\n", argv[0]);
			retVal = -EINVAL;
			break;
		}

		input1= argv[1];
		input2 = argv[2];

		if (strlen(input1) != strlen(input2))
		{
			printf("ERROR: Enter two equal length buffers\n");
			printf("USAGE: %s <String 1> <String 2>\n", argv[0]);
			retVal = -EINVAL;
			break;
		}

		result = stringXOR(input1, input2);

		printf("%s\n", result);

		free(result);

	} while (0);
	return retVal;
}
