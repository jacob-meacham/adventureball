#pragma once

class Vector
{
	public:
		int m_x, m_y;
	
	//! trivial ctor
	Vector() {}

	//! setting ctor
	Vector(const int x0, const int y0): m_x(x0), m_y(y0) {}

	//! function call operator
	void operator ()(const int x0, const int y0) 
	{	m_x= x0; m_y= y0;	}

	//! test for equality
	bool operator==(const Vector &v)
	{	return (m_x==v.m_x && m_y==v.m_y);	}

	//! test for inequality
	bool operator!=(const Vector &v)
	{	return (m_x!=v.m_x || m_y!=v.m_y);	}
	
	//! set to value
	const Vector &operator =(const Vector &v)
	{	
		m_x= v.m_x; m_y= v.m_y;			
		return *this;
	}
	
	//! negation
	const Vector operator -(void) const
	{	return Vector(-m_x, -m_y);	}

	//! addition
	const Vector operator +(const Vector &v) const
	{	return Vector(m_x+v.m_x, m_y+v.m_y);	}
	
	//! subtraction
	const Vector operator -(const Vector &v) const
	{   return Vector(m_x-v.m_x, m_y-v.m_y);	}
		
	//! uniform scaling
	const Vector operator *(const T num) const
	{
		Vector temp(*this);			
		return temp*=num;
	}

	//! uniform scaling
	const Vector operator /(const T num) const
	{
		Vector temp(*this);			
		return temp/=num;
	}		

	//! addition
	const Vector &operator +=(const Vector &v)
	{
		m_x+=v.m_x;	m_y+=v.m_y;						
		return *this;
	}

	//! subtraction
	const Vector &operator -=(const Vector &v) 
	{ 
		m_x-=v.m_x;	m_y-=v.m_y;						
		return *this;	
	}

	//! uniform scaling
	const Vector &operator *=(const T num)
	{
		m_x*=num;	m_y*=num;										
		return *this;
	}
	
	//! uniform scaling
	const Vector &operator /=(const T num)
	{
		m_x/=num;	m_y/=num;										
		return *this;
	}

	//! dot product
	T operator *(const Vector &v) const
	{	return m_x*v.m_x + m_y*v.m_y;	}	
};
