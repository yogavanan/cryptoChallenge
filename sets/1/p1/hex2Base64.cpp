#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

static int mod_table[] = {0, 2, 1};

char * base64Encode(char * input)
{
	unsigned int temp[3] = {0,};
	unsigned int input_length = strlen(input);
	unsigned int triple = 0;
	int ouputLength = 4 * ((input_length + 2) / 3);
	int outputCnt = 0;

	char *output = (char *) malloc(ouputLength);

	for (int i = 0; i < input_length;)
	{
		triple = 0;

		temp[0] = i < input_length ? (unsigned char)input[i++] : 0;
		temp[1] = i < input_length ? (unsigned char)input[i++] : 0;
		temp[2] = i < input_length ? (unsigned char)input[i++] : 0;

		triple = (temp[0] << 0x10) + (temp[1] << 0x08) + temp[2];

		output[outputCnt++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		output[outputCnt++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		output[outputCnt++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		output[outputCnt++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}

    for (int i = 0; i < mod_table[input_length % 3]; i++)
    	output[ouputLength - 1 - i] = '=';

    return output;
}


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

int main(int argc, char ** argv)
{
	int retVal = 0;
	char * input = NULL;
	char * actualString = NULL;
	char * base64 = NULL;
	char temp[2] = {0,};
	char hexTemp[2] = {0,};
	int inputLength = 0;
	int actualLength = 0;

	do
	{
		if (argc != 2)
		{
			printf("USAGE: %s <Hex String>\n", argv[0]);
			retVal = -EINVAL;
			break;
		}

		input = argv[1];
		inputLength  = strlen(input);
		char byte = 0;

		actualString = (char *) malloc(inputLength);
		memset(actualString, 0, inputLength);

		for (int i = 0; i < inputLength;)
		{
			memset(temp, 0 , 2);
			byte = 0;

			hexTemp[0] = i < inputLength ? (unsigned char)(input[i++]) : 0;
			hexTemp[1] = i < inputLength ? (unsigned char)(input[i++]) : 0;

			temp[0] = hextodec(hexTemp[0]);
			temp[1] = hextodec(hexTemp[1]);

			if (temp[0] == 0XFF || temp[1] == 0xFF)
			{
				retVal = -EINVAL;
				break;
			}

			byte = (temp[0] << 4) + (temp[1]);

			actualString[actualLength] = (char) (byte & 0xFF);
			actualLength++;
		}

		if (retVal != 0)
		{
			free(actualString);
			break;
		}

		base64 = base64Encode(actualString);
		printf("%s\n", base64);

		free(actualString);
		free(base64);
	} while (0);
	return retVal;
}
