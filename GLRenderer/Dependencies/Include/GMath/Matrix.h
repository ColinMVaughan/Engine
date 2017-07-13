//Colin Vaughan 
//December 2016

#pragma once
#include <initializer_list>
#include "Vector.h"


namespace GMath
{
	//IMPORTANT: This matrix class is implemenmted as column major
	template <unsigned int C, unsigned int R, typename T>
	class matrix
	{
	public:

		matrix();
		matrix(T num);
		matrix(std::initializer_list<T> numbers);

		T& operator() (unsigned column, unsigned row);
		T& operator()(unsigned index);

		int getColumnSize();
		int getRowSize();
		T* GetData();
		
		matrix<C, R, T> operator+(matrix<C, R, T> &RHS);
		matrix<C, R, T> operator-(matrix<C, R, T> &RHS);
		matrix<C, R, T> operator*(matrix<C, R, T> &RHS);
		matrix<C, R, T> operator/(matrix<C, R, T> &RHS);
		void operator=(matrix<C, R, T> &RHS);

		vector<R, T> operator*(vector<R, T> RHS);

		bool operator==(matrix<C, R, T> &RHS) const ; 
		bool operator==(const matrix<C, R, T> &RHS) const;

	private:
		T data[C * R];
	};
	typedef matrix<4, 4, float> mat4f;


	template <unsigned C, unsigned R, typename T>
	matrix<C, R, T>::matrix()
	{
		for(int i =0; i< C*R; ++i)
		{
			data[i] = 0;
		}
	}

	template <unsigned C, unsigned R, typename T>
	matrix<C, R, T>::matrix(T num)
	{
		for (int i = 0; i< C*R; ++i)
		{
			data[i] = num;
		}
	}

	template <unsigned C, unsigned R, typename T>
	matrix<C, R, T>::matrix(std::initializer_list<T> numbers)
	{
		assert(numbers.size() == C*R);

		int i = 0;
		for (auto itr = numbers.begin(); itr != numbers.end(); ++itr)
		{
			data[i] = *itr;
			++i;
		}
	}

	template <unsigned C, unsigned R, typename T>
	T& matrix<C, R, T>::operator()(unsigned column, unsigned row)
	{
		//static_assert((row <= R && column <= C), "The index is out of bounds");
		return data[column*C + row];
	}

	template <unsigned C, unsigned R, typename T>
	T& matrix<C, R, T>::operator()(unsigned index)
	{
		//static_assert((index <= C*R), "The index is out of bounds");
		return data[index];
	}

	template <unsigned C, unsigned R, typename T>
	int matrix<C, R, T>::getColumnSize()
	{
		return C;
	}

	template <unsigned C, unsigned R, typename T>
	int matrix<C, R, T>::getRowSize()
	{
		return R;
	}

	template <unsigned C, unsigned R, typename T>
	T* matrix<C, R, T>::GetData()
	{
		return data;
	}

	template <unsigned C, unsigned R, typename T>
	matrix<C, R, T> matrix<C, R, T>::operator+(matrix<C, R, T>& RHS)
	{
		matrix<C, R, T> temp;
		for(int i = 0; i< C*R; i++)
		{
			temp(i) = data[i] + RHS(i);
		}

		return temp;
	}

	template <unsigned C, unsigned R, typename T>
	matrix<C, R, T> matrix<C, R, T>::operator-(matrix<C, R, T>& RHS)
	{
		matrix<C, R, T> temp;
		for (int i = 0; i< C*R; i++)
		{
			temp(i) = data[i] - RHS(i);
		}

		return temp;
	}

	template <unsigned C, unsigned R, typename T>
	matrix<C, R, T> matrix<C, R, T>::operator*(matrix<C, R, T>& RHS)
	{
		matrix<C, R, T> temp;
		T tempVal;

		for(int i = 0; i<R; ++i)
		{
			for (int j = 0; j<RHS.getColumnSize(); ++j)
			{
				temp(j, i) = 0;
				for (int in = 0; in<RHS.getRowSize(); ++in) //not sure
				{
					temp(j, i) += data[in*C + i] * RHS(j, in);
				}
			}
		}

		return temp;
	}

	template <unsigned C, unsigned R, typename T>
	matrix<C, R, T> matrix<C, R, T>::operator/(matrix<C, R, T>& RHS)
	{
	}

	template <unsigned C, unsigned R, typename T>
	void matrix<C, R, T>::operator=(matrix<C, R, T>& RHS)
	{
		for(int i = 0; i<C*R; ++i)
		{
			data[i] = RHS(i);
		}
	}

	template <unsigned C, unsigned R, typename T>
	vector<R, T> matrix<C, R, T>::operator*(vector<R, T> RHS)
	{
		vector<R, T> temp;
		for(int i=0; i<C; ++i)
		{
			for(int j=0; j<R; ++j)
			{
				temp[i] += (data[C*j +i] * RHS[j]);
			}
		}

		return temp;
	}

	template <unsigned C, unsigned R, typename T>
	bool matrix<C, R, T>::operator==(matrix<C, R, T>& RHS) const
	{
			for(int i=0; i < C*R; ++i)
			{
				if (data[i] != RHS(i))
					return false;
			}
		
			return true;
	}

	template <unsigned C, unsigned R, typename T>
	bool matrix<C, R, T>::operator==(const matrix<C, R, T>& RHS) const
	{
		for (int i = 0; i < C*R; ++i)
		{
			if (data[i] != RHS(i))
				return false;
		}

		return true;
	}

}
