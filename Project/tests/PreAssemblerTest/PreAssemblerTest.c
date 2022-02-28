#include "../../PreAssemblyStage/PreAssembler.h"

int main(int argc, char *argv[])
{
	int i = 0;
	
	for (i = 1; i < argc; ++i)
	{
		CreateAmFile(argv[i]);
	}
	
	return 0;
}
