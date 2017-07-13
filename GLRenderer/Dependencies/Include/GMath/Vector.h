//Colin Vaughan 
//December 2016

#pragma once
#include <initializer_list>
#include <assert.h>
#include <complex>

namespace GMath
{

	template<unsigned int N, typename T>
	class vector
	{
	public:

		vector();
		explicit vector(std::initializer_list<T> numbers);
		explicit vector(const T dat[]);

		~vector();

		T& operator[](const int index);
		const T& operator[](const int index) const;

		vector operator+(vector RHS);
		vector operator-(vector RHS);
		vector operator*(float Scalar);

		void operator=(vector RHS);

		void operator+=(vector RHS);
		void operator-=(vector RHS);
		void operator*=(float Scalar);

		float Length();
		void Normalize();


	private:
		T values[N];
	};

	typedef vector<2, int>    vec2i;
	typedef vector<2, float>  vec2f;
	typedef vector<3, int>    vec3i;
	typedef vector<3, float>  vec3f;
	typedef vector<4, float>  vec4f;



	template<unsigned int N, typename T>
	T& vector<N, T>::operator[](const int index)
	{
		return values[index];
	}

	template <unsigned N, typename T>
	const T& vector<N, T>::operator[](const int index) const
	{
		return values[index];
	}

	template<unsigned int N, typename T>
	void vector<N, T>::operator*=(float Scalar)
	{
		for (int i = 0; i < N; i++)
		{
			values[i] *= Scalar;
		}
	}

	template <unsigned N, typename T>
	float vector<N, T>::Length()
	{
		T temp = 0;

		for(int i = 0; i < N; i++)
		{
			temp += values[i] * values[i];
		}
		return std::sqrt(temp);
	}

	template <unsigned N, typename T>
	void vector<N, T>::Normalize()
	{
		float length = Length();
		if(length != 0)
		{
			for(int i=0; i<N; ++i)
			{
				values[i] = values[i] / length;
			}
		}
	}

	template<unsigned int N, typename T>
	void vector<N, T>::operator-=(vector<N, T> RHS)
	{
		for (int i = 0; i < N; i++)
		{
			values[i] -= RHS[i];
		}
	}

	template<unsigned int N, typename T>
	void vector<N, T>::operator+=(vector<N, T> RHS)
	{
		for (int i = 0; i < N; i++)
		{
			values[i] += RHS[i];
		}
	}

	template<unsigned int N, typename T>
	void vector<N, T>::operator=(vector<N, T> RHS)
	{
		for (int i = 0; i < N; i++)
		{
			values[i] = RHS[i];
		}
	}

	template<unsigned int N, typename T>
	vector<N, T> vector<N, T>::operator*(float Scalar)
	{
		vector<N, T> temp;
		for(int i = 0; i<N; ++i)
		{
			temp[i] = values[i] * Scalar;
		}

		return temp;
	}

	template <unsigned int N, typename T>
	vector<N, T> vector<N, T>::operator-(vector<N, T> RHS)
	{
		vector<N, T> temp;
		for(int i=0; i<N; ++i)
		{
			temp[i] = values[i] - RHS[i];
		}
		return temp;
	}

	template <unsigned int N, typename T>
	vector<N, T> vector<N, T>::operator+(vector<N, T> RHS)
	{
		vector<N, T> temp;
		for(int i=0; i<N; ++i)
		{
			temp[i] = values[i] - RHS[i];
		}
		return temp;
	}

	template<unsigned int N, typename T>
	vector<N, T>::~vector()
	{

	}

	template<unsigned int N, typename T>
	vector<N, T>::vector()
	{
		for (int i = 0; i < N; ++i)
		{
			values[i] = 0;
		}
	}

	template <unsigned N, typename T>
	vector<N, T>::vector(std::initializer_list<T> numbers)
	{
		assert(numbers.size() == N);

		int i = 0;
		for (auto itr = numbers.begin(); itr != numbers.end(); ++itr)
		{
			values[i] = *itr;
			++i;
		}
	}

	template <unsigned N, typename T>
	vector<N, T>::vector(const T dat[])
	{
		for (int i = 0; i < N; i++)
		{
			values[i] = (T)dat[i];
		}
	}

	inline vec3f Cross(vec3f vec1, vec3f vec2)
	{
		vec3f result;

		result[0] = (vec1[1] * vec2[2]) - (vec1[2] * vec2[1]);
		result[1] = (vec1[2] * vec2[0]) - (vec1[0] * vec2[2]);
		result[2] = (vec1[0] * vec2[1]) - (vec1[1] * vec2[0]);

		return result;
	}

	inline float DotProduct(vec3f vec1, vec3f vec2)
	{
		return (vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2]);
	}

	inline void Set(vec3f& vector, float x, float y, float z)
	{
		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
	}

	inline void Set(vec2f& vector, float x, float y)
	{
		vector[0] = x;
		vector[1] = y;
	}
}
