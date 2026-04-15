#include <stdio.h>
int main()
{
float x1,y1,x2,y2,x3,y3;
printf("x1?\t");
printf("y1?\t");
printf("x2?\t");
printf("y2?\t");
printf("x3?\t");
printf("y3?\n");
scanf("%f",&x1);
scanf("%f",&y1);
scanf("%f",&x2);
scanf("%f",&y2);
scanf("%f",&x3);
scanf("%f",&y3);

int x_higher_bound,y_higher_bound,x_lower_bound,y_lower_bound;

/*Checking for the controlling coordinates.*/
x_higher_bound=x1>x2?x1:x2;
x_lower_bound=x1>x2?x2:x1;
y_higher_bound=y1>y2?y1:y2;
y_lower_bound=y1>y2?y2:y1;

if ((x3>=x_lower_bound && y3>=y_lower_bound) && (x3<=x_higher_bound && y3<=y_higher_bound))
{
if ((x3==x_lower_bound || y3==y_lower_bound || y3==y_higher_bound || x3==x_higher_bound ))
{
printf("Points are on the edge of the rectangle.\n");
}
else
{
printf("Points are inside the rectangle.\n");
}
}
else{
printf("Points are outside the rectangle.\n");
}
return 0;
}
