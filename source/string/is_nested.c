/*
    This function checks if the given path is nested
*/
int strnst(char *src, char c)
{
	for(int i = 0;i<(int)strlen(src);i++)
	{
		if(src[i]=='/')
		{
			return 1;
		}
	}
    return 0;
}