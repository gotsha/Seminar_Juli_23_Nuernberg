#include <iostream>

void even(int val) {
	if ((val % 2) == 0) {
		std::cout << val << " is even" << std::endl;
	}
	else {
		std::cout << val << " is not even" << std::endl;
	}

}

int mainLambda() {
	even(4);
	even(5);

	std::cout << "====== LAMBDA =====" << std::endl;
	auto lambda = [](int val) { if ((val % 2) == 0) {
		std::cout << val << " is even" << std::endl;
	}
	else {
		std::cout << val << " is not even" << std::endl;
	} };
	lambda(4);
	lambda(5);



	auto lambdaClosure = [](int val, int divisor) { if ((val % divisor) == 0) {
		std::cout << val << " is even" << std::endl;
	}
	else {
		std::cout << val << " is not even" << std::endl;
	} };
	lambdaClosure(4, 2);
	lambdaClosure(5, 2);



	int divisor = 2;
	auto lambdaCapture = [=](int val) { if ((val % divisor) == 0) {
		std::cout << val << " is even" << std::endl;
	}
	else {
		std::cout << val << " is not even" << std::endl;
	} };
	lambdaCapture(4);
	lambdaCapture(5);

	return 0;
}