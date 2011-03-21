#include <stdio.h>
#include <string.h>

#define MAXCHAR 16



class fridge
{
	char color[MAXCHAR];
	int lenght;
	int widht;
	int height;
public:
	fridge(char t_color[MAXCHAR], int t_lenght, int t_widht, int t_height);
	fridge(int t_all);
	const fridge& operator=(const fridge& f);
	fridge operator++(int);
	int& operator[](int index);
	char * get_color(){
		return color;
	}
	int get_lenght() const{
		return lenght;
	}
	int get_widht() const{
		return widht;
	}
	int get_height() const{
		return height;
	}
	void print();
};


fridge::fridge(char t_color[MAXCHAR], int t_lenght, int t_widht, int t_height)
	//: color(t_color), lenght(t_lenght), widht(t_widht), height(t_height)
{
	strcpy(color, t_color);
	lenght = t_lenght;
	widht = t_widht;
	height = t_height;
} 


fridge::fridge(int t_all)
	//: color("white"), lenght(t_all), widht(t_all), height(t_all)
{
	strcpy(color, "white");
	lenght = t_all;
	widht = t_all;
	height = t_all;
}


const fridge& fridge::operator=(const fridge& f)
{
	strcpy (color, f.color);
	lenght = f.lenght;
	widht = f.widht;
	height = f.height;
	return *this;
}
	
fridge fridge::operator++(int)
{
	fridge tmp(*this);
	lenght++;
	widht++;
	height++;	
	return tmp;	
}

fridge operator+(fridge a, const fridge& b)
{
	return fridge(a.get_color(), a.get_lenght() + b.get_lenght(), a.get_widht() + b.get_widht(), a.get_height() + b.get_height());
}

int& fridge::operator[](int index)
{
	switch (index) {
		case 1: return lenght;
		case 2: return widht;
		case 3: return height;
		default: printf ("error");
	}
}

void fridge::print()
{
	printf ("Color:%s, lenght:%d, widht:%d, height:%d\n", color, lenght, widht, height);
}



int main()
{
	printf ("Start program.\n");

	fridge R("blue",7,1,12);	//цвет, длина, ширина, высота
	fridge G(7);			//холодильник белого цвета 7х7х7

	fridge Res = R++; 		
	// оператор ++ увеличивает на 1 все размеры
	// Res – копия R до увеличения размеров

	R.print();
	G.print();
	Res.print();

	Res = G+R; 
	// цвет – любой, каждый размер = сумме размеров
	// холодильников-слагаемых

	Res=4+G;//та же функция сложения, что и в предыдущем операторе

	R.print();
	G.print();
	Res.print();

	Res[1]=Res[2]+10;//длина холодильника = ширина холодильника+10

	R.print();
	G.print();
	Res.print();

	printf ("End program.\n");

	return 0;
}

