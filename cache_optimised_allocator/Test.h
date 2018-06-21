#pragma once

template<typename Type>
class Test
{
	void execute ( );
};

// Not needed
void Test<int>::execute ( )
{
	return;
}

template<typename Type>
void GetId ( )
{
	return 0;
}

// Needed
template<>
void GetId<int> ( )
{
	return 1;
}