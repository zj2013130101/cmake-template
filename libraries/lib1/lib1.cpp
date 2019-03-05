#include <project_namespace/lib_namaspace/hello.hpp>

namespace eosio { namespace chain {
	
	void printMessage(char* message)
	{
		printf("Hello world!\n");
		printf("%s\n", message);
	}

	}
}
