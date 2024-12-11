#ifndef Unit3H
#define Unit3H

#include <System.Classes.hpp>
#include <string>
#include <iostream>


class TProduct {
public:
	int code;
	char name[50];
	int quantity;
	double price;
	int current;
	int voltage;

	virtual int GetPowerOrCapacity() const = 0;
};

class TPowerSource : public TProduct {
private:
	int capacity;
public:
	int FindCapacity ()
		{  capacity = static_cast<int>(current * voltage * 0.4) ;
		return capacity;
		}

		int GetPowerOrCapacity() const override { return capacity; }
};

class TPowerConsumer : public TProduct {
private:
	int power;
public:
	  int FindPower ()
		  {
			 power = static_cast<int>(current*voltage*0.9);
		  return power ;
		  }

		  int GetPowerOrCapacity() const override { return power; }
};

const int MAX_PRODUCTS = 1000;
extern TProduct* products[MAX_PRODUCTS];
extern int productCount;

#endif
