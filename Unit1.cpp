//---------------------------------------------------------------------------
 #include <System.Classes.hpp>
#include <memory>
#include <vcl.h>
#pragma hdrstop
#include <cstdio>
 #include <iostream>
 #include <cstring>
 #include <cmath>

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit5.h"
#include "Unit6.h"
using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::addClick(TObject *Sender)
{

	Form2 = new TForm2(this, this);
	Form2->Show();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::OpenaFileClick(TObject *Sender)
{
	sort_asc = true;
	ProdRead();
	ProdView();
}

void TForm1::ProdRead() {
	FILE* file = fopen("product.txt", "r");
	if (file == nullptr) {
		ShowMessage("Невозможно открыть файл product.txt для чтения");
		return;
	}
	ArrayClear();

	int type, code, quantity, current, voltage;
	double price;
	char name[50];

	while (!feof(file)&& productCount < MAX_PRODUCTS) {
		memset(name, 0, sizeof(name));
		int ret = fscanf(file, "%d:%d:%49[^:]:%d:%lf:%d:%d", &type, &code, name, &quantity, &price, &current, &voltage);
		if (ret == EOF || ret < 7) break;

		TProduct* product;
		if (type == 1) { // Источник тока
			TPowerSource *source = new TPowerSource;
			source->code = code;
			strcpy(source->name, name);
			source->quantity = quantity;
			source->price = price;
			source->current = current;
			source->voltage = voltage;
			source->FindCapacity();
			product = source;
		} else { // Потребитель тока
			TPowerConsumer *consumer = new TPowerConsumer;
			consumer->code = code;
			strcpy(consumer->name, name);
			consumer->quantity = quantity;
			consumer->price = price;
			consumer->current = current;
			consumer->voltage = voltage;
			consumer->FindPower();
			product = consumer;
		}
		products[productCount++] = product;
	}
	fclose(file);
}


void TForm1::ProdView() {
	ListView1->Items->BeginUpdate();
	ListView1->Items->Clear();

	for (int i = 0; i < productCount; ++i) {
		TProduct* product = products[i];
		TListItem* item = ListView1->Items->Add();
		item->Caption = IntToStr(product->code);
		item->SubItems->Add(product->name);
		item->SubItems->Add(IntToStr(product->quantity));
		item->SubItems->Add(FloatToStrF(product->price, ffFixed, 8, 2));
		item->SubItems->Add(IntToStr(product->current));
		item->SubItems->Add(IntToStr(product->voltage));
		item->SubItems->Add(IntToStr(product->GetPowerOrCapacity()));
	}

	ListView1->Items->EndUpdate();
}

void TForm1::ArrayClear() {
	for (int i = 0; i < productCount; ++i) {
	delete products[i];
	}
	productCount = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteClick(TObject *Sender)
{
	TListItem *item = ListView1->Selected;

	if (item != nullptr) {
		int index = item->Index;

		if (index >= 0 && index < productCount) {
			delete products[index];
			for (int i = index; i < productCount - 1; ++i) {
				products[i] = products[i + 1];
			}
			productCount--;

			ListView1->Items->Delete(index);
		}
	} else {
		ShowMessage("Пожалуйста, выберите элемент для удаления.");
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveClick(TObject *Sender)
{
	 FILE *file = fopen("products1.txt", "w+");
	if (file == NULL)
	{
		ShowMessage("Невозможно открыть файл для записи.");
		return;
	}

	  for(int i = 0; i < productCount ; i++)
   {
		fprintf(file, "%d:%-20s:%d:%.2f:%d:%d:%d\n",
		 products[i]->code,  products[i]->name,  products[i]->quantity,  products[i]->price,
		 products[i]->current,  products[i]->voltage, products[i]->GetPowerOrCapacity() );
   }

	fclose(file);
	ShowMessage("Данные успешно сохранены в файл.");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChangeClick(TObject *Sender)
{
	TListItem *item = ListView1->Selected;

	if (item != nullptr) {
		int index = item->Index;

		if (index >= 0 && index < productCount) {
			Form5 = new TForm5(this, this);
			Form5->EditProduct(products[index], index);
			Form5->Show();
		}
	} else {
		ShowMessage("Пожалуйста, выберите элемент для редактирования.");
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SortChange(TObject *Sender)
{
	  if (Sort->ItemIndex == 0) {
		SortByCode();
	} else if (Sort->ItemIndex == 1) {
		SortByName();
	} else if (Sort->ItemIndex == 2) {
		SortByQuantity();
	} else if (Sort->ItemIndex == 3) {
		SortByPrice();
	} else if (Sort->ItemIndex == 4) {
		SortByCurrent();
	} else if (Sort->ItemIndex == 5) {
		SortByVoltage();
	} else if (Sort->ItemIndex == 6) {
		SortByPowerOrCapacity();
	}

	  ProdView();
}

void  TForm1::SortByCode()
{

	for (int i = 1; i < productCount; i++)
	{
		TProduct *tmp = products[i];
		int j ;

		for (j = i; j > 0 &&  tmp->code < products[j-1]->code; j--)
		 products[j]=products[j-1];

		products[j] = tmp;

	}
}

void  TForm1::SortByName()
{

     for (int i = 1; i < productCount; i++) {
		TProduct* tmp = products[i];
		int j=i;
		while (j >= 0 && strcmp(products[j-1]->name, tmp->name) > 0) {
			products[j] = products[j-1];
   			j--;
        }
		products[j] = tmp;
	}
}

void  TForm1::SortByQuantity()
{

	for (int i = 1; i < productCount; i++)
	{
		TProduct *tmp = products[i];
		int j ;

		for (j = i; j > 0 &&  tmp->quantity < products[j-1]->quantity; j--)
		 products[j]=products[j-1];

		products[j] = tmp;

	}
}

void  TForm1::SortByPrice()
{

	for (int i = 1; i < productCount; i++)
	{
		TProduct *tmp = products[i];
		int j ;

		for (j = i; j > 0 &&  tmp->price < products[j-1]->price; j--)
		 products[j]=products[j-1];

		products[j] = tmp;

	}
}


void  TForm1::SortByCurrent()
{

	for (int i = 1; i < productCount; i++)
	{
		TProduct *tmp = products[i];
		int j ;

		for (j = i; j > 0 &&  tmp->current < products[j-1]->current; j--)
		 products[j]=products[j-1];

		products[j] = tmp;

	}
}

void  TForm1::SortByVoltage()
{

	for (int i = 1; i < productCount; i++)
	{
		TProduct *tmp = products[i];
		int j ;

		for (j = i; j > 0 &&  tmp->voltage < products[j-1]->voltage; j--)
		 products[j]=products[j-1];

		products[j] = tmp;

	}
}

void TForm1::SortByPowerOrCapacity()
{

	for (int i = 1; i < productCount; i++)
	{
		TProduct *tmp = products[i];
		int j ;

		for (j = i; j > 0 &&  tmp->GetPowerOrCapacity() < products[j-1]->GetPowerOrCapacity(); j--)
		 products[j]=products[j-1];

		products[j] = tmp;

	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CreateOrderClick(TObject *Sender)
{

	Form6 = new TForm6(this);

    Form6->LoadDataFromForm1(ListView1);

	Form6->Show();
}
//---------------------------------------------------------------------------
bool TForm1::isEqual(double a, double b, double epsilon)
	{
	
		return fabs(a - b) < epsilon;
		
	}
	
void __fastcall TForm1::SearchClick(TObject *Sender)
{
	char  KeyWord[50];

   memset(KeyWord, 0, sizeof(KeyWord));
   strcpy(KeyWord, AnsiString(Edit1->Text.c_str()).c_str());

   int key = atoi(AnsiString(Edit1->Text.c_str()).c_str());

	ListView1->Items->BeginUpdate();
	ListView1->Items->Clear();

	double KeyPrice =  atof(AnsiString(Edit1->Text.c_str()).c_str());

	for (int i = 0; i < productCount; ++i) {
		 if(products[i]->code == key  || strcmp(KeyWord, products[i]->name) == 0  
		 || products[i]->quantity == key || isEqual(products[i]->price, KeyPrice, 1e-6)
		 || products[i]->current == key || products[i]->voltage == key || products[i]->GetPowerOrCapacity() == key) 
			{
		
			TListItem *ListItem = ListView1->Items->Add();
			ListItem->Caption = IntToStr(products[i]->code);
			ListItem->SubItems->Add(products[i]->name); 
			ListItem->SubItems->Add(IntToStr(products[i]->quantity)); 
			ListItem->SubItems->Add(FloatToStrF(products[i]->price, ffFixed, 8, 2)); 
			ListItem->SubItems->Add(IntToStr(products[i]->current)); 
			ListItem->SubItems->Add(IntToStr(products[i]->voltage)); 
			ListItem->SubItems->Add(IntToStr(products[i]->GetPowerOrCapacity()));
		
	
				ListView1->Items->EndUpdate();   
			}
			Edit1->Text = "";
		}
}

//---------------------------------------------------------------------------

