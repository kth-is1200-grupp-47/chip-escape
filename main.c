#include <pic32mx.h>
#include "data/macros.h"

USE_IMAGE(test);

int main() {
	bool test = IMAGE_BIT(test, 10, 10);
	return test;
}