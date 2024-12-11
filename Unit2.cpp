//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
 #include "Unit1.h"
 #include "Unit3.h"

 #include <cstring>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TForm1* form1, TComponent* Owner)
	: TForm(Owner)
	{
		this->Form1 = form1;
	}
//---------------------------------------------------------------------------

void __fastcall TForm2::ButtonSaveClick(TObject *Sender)
{

	if (productCount >= MAX_PRODUCTS) {
		ShowMessage("Массив продуктов заполнен");
		return;
	}

   TProduct *product = nullptr;
	AnsiString name = EditName->Text;

	if (ComboBoxType->ItemIndex == 0) { // Источник тока
		TPowerSource *source = new TPowerSource;
		source->code = StrToInt(EditCode->Text);
		size_t length = name.Length();
		memcpy(source->name, name.c_str(), length);
		source->name[length] = '\0';
		source->quantity = StrToInt(EditQuantity->Text);
		source->current = StrToInt(EditCurrent->Text);
		source->price = StrToFloat(EditPrice->Text);
		source->voltage = StrToInt(EditVoltage->Text);
		source->FindCapacity();
		product = source;
	} else { // Потребитель
		TPowerConsumer *consumer = new TPowerConsumer;
		consumer->code = StrToInt(EditCode->Text);
        size_t length = name.Length();
		memcpy(consumer->name, name.c_str(), length);
		consumer->name[length] = '\0';
        consumer->quantity = StrToInt(EditQuantity->Text);
		consumer->current = StrToInt(EditCurrent->Text);
		consumer->price = StrToFloat(EditPrice->Text);
        consumer->voltage = StrToInt(EditVoltage->Text);
        consumer->FindPower();
		product = consumer;
	}

	products[productCount++] = product;
	Form1->ProdView();

	EditCode->Text = "";
	EditName->Text = "";
	EditQuantity->Text = "";
	EditCurrent->Text = "";
    EditPrice->Text = "";
    EditVoltage->Text = "";
	ComboBoxType->ItemIndex = -1;

	Close();
  }

//---------------------------------------------------------------------------

