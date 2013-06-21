template<typename Iterator, typename T> inline
T* binarySearch(Iterator begin, Iterator end, const T& value)
{
	std::vector<T>::iterator iter = std::lower_bound(begin, end, value);
	if (iter != end && (*iter == value))
	{
		return &(*iter);
	}
	else
	{
		return NULL;
	}
}