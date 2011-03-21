#include <stdio.h>


class IMatrix 
{
public:
	class IMatrixLine 
	{
		IMatrix * master;
		int line;
	public:
		int operator[] (int idx)
		{
			return (line == idx);
		}

		IMatrixLine (int ind)
		{
			line = ind;
		}
	};

	IMatrixLine operator[] (int line)
	{
		return IMatrixLine(line);
	}

};


int main ()
{
	IMatrix E;
	int i, j;

	i = 4;
	j = 3;

	printf ("E[%d][%d]=%d", i, j , E[i][j]);
}
