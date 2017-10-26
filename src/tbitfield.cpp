// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	BitLen = len;
	MemLen = len / sizeof (TELEM) +1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i<MemLen; i++)
		pMem[i] =0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n / sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	TELEM a;
	a = 1;
	a = a << (n % sizeof(TELEM));
	return a;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n<0) || n >= BitLen) 
	{
		throw ("ERROR");
	}
	int i = GetMemIndex (n);
	TELEM mask = GetMemMask (n);
	pMem [i] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
		if ((n<0) || n >= BitLen) 
	{
		throw ("ERROR");
	}
	int i = GetMemIndex (n);
	TELEM mask = GetMemMask (n);
	mask = ~mask;
	pMem[i]&=mask;

}

int TBitField::GetBit(const int n) const // получить значение бита
{
		if ((n<0) || n >= BitLen) 
	{
		throw ("ERROR");
	}
	int i = GetMemIndex (n);
	TELEM mask = GetMemMask (n);
	mask &= pMem [i];
	if (mask ==0)
	return 0;
	else return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (MemLen != bf.MemLen) {
		delete [] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM [MemLen];
	}
	BitLen =bf.BitLen;
	for (int i =0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (bf.BitLen != BitLen)
		return 0;
	else
		for (int i = 0; i < MemLen-1; i++)
		if (bf.pMem[i]!=this ->pMem[i])
			return 0;
	for (int i = BitLen - 1; i>BitLen - BitLen%32; i--)
		if (bf.GetBit(i) != this->GetBit(i))
			return 0;
	return 1; 
		
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  		if (bf.BitLen != BitLen)
		return 1;
	else 
		for (int i = 0; i < MemLen-1; i++)
		if (bf.pMem[i]!=this ->pMem[i])
			return 1;
	for (int i = BitLen - 1; i>BitLen - BitLen%32; i--)
		if (bf.GetBit(i) != this->GetBit(i))
			return 1;
	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int max = BitLen;
	if (BitLen < bf.BitLen)
		max = bf.BitLen;
	TBitField temp (max);
	for (int i =0; i<temp.MemLen ; i++)
	{
		temp.pMem [i] = pMem [i] | bf.pMem [i];
	}
		return temp;
}
	

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int max = BitLen;
	if (BitLen < bf.BitLen)
		max = bf.BitLen;
	TBitField temp(max);
	for (int i =0; i<temp.MemLen ; i++)
	{
		temp.pMem [i] = pMem [i] & bf.pMem [i];
	}
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(BitLen);
	for (int i = 0; i < BitLen; i++) 
	if (GetBit(i) == 0) 
	{
		temp.SetBit(i);
	}
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char sym;
	istr >> sym;
	while ((sym=='0') || (sym =='1'))
	{
		if (sym == '0')
			bf.ClrBit(i);
		else
			bf.SetBit(i);
		i++;
		istr>>sym;
	}
	return istr;

}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for ( int i = 0; i<len; i++)
		if (bf.GetBit(i)>0)
			ostr<<1;
		else
			ostr << 0;
	return ostr;
}
