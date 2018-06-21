#include <vector>
#include <iostream>

constexpr auto cache_alignment = size_t { 64 };

struct alignas ( cache_alignment ) vector3
{
	float x;
	float y;
	float z;
};

int main ( )
{
	while ( true )
	{
		std::cout << "Testing to see if vector would allocate 12 objects according to their specified alignment.\n";
		std::cout << '\n';
		std::vector<vector3> my_vector;

		my_vector.resize ( 12 );

		const auto starting_address = reinterpret_cast<std::size_t> ( &my_vector.front ( ) );

		if ( starting_address %  alignof ( vector3 ) != 0 )
		{
			std::cout << "Failure!" << '\n';
			std::system ( "pause" );
			break;
		}
		else
		{
			std::cout << "Success!\nEnter a blank to try again: ";
			int num = 0;
			char line [ 256 ];
			std::cin.getline ( line, 10 );

			if ( line [ 0 ] != '\0' )
			{
				break;
			}
		}
	};

	return 0;
}
