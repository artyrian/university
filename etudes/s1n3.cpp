#include <stdio.h>
#include <math.h>

class my_vector
{
	int len;	// dim of vector;
	int * v;	// ptr  to elements array of vector;
public:
	my_vector (int l, int *arr)
	{
		v = new int [l]; 
		len = l;

		int i;
		for (i = 0; i < len; i++)
		{
			v[i] = arr[i];
		}
	}

	my_vector ()
	{	// constructor po ymolchani. HZ
	}
	
	my_vector(const my_vector & a)
	{
		v = new int [len];
		for (int i = 0; i < len; i++)
		{
			v[i] = a.v[i];
		}
	
	}

	~my_vector()
	{
		delete [] v;
	}
	
	my_vector next()
	{
		for (int i = 0; i < len; i++)
		{
			v[i]++;	
		}
		return my_vector(len, v);
	}
	
	double lenght()
	{
		int sum = 0;
		for (int i = 0; i < len; i++)
		{
			
			sum += v[i]*v[i];
		}
		return sqrt(sum);
	}
	
	void print ()
	{
		int i;
		printf ("{");
		for (i = 0; i < len; i++)
		{
			printf ("%d", v[i]);
			if (i != len - 1)
				printf (",");
		}
		printf ("}");
	}

};



int main (int argc, char * argv[])
{
	int arr[2] = {1, 0};	
	my_vector v (2, arr);		

	printf ("Start program.\n");

	printf ("Vector v is: ");
	v.print();
	printf (".\n");

	printf ("Lenght of vector v is %f.\n", v.lenght() );
	
	v.next().next();
	printf ("Vector v is: ");
	v.print();
	printf (".\n");


	printf ("End program.\n");

	return 0;
}

