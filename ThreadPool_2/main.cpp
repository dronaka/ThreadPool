#include "threadPool.h";
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

void factorization(int curNumber)
{
	int thisNumber = curNumber;
	int divider = 2;

	std::vector<int> multipliers;
	multipliers.clear();
	while (divider <= sqrt(curNumber)) {
		if (curNumber % divider == 0) {
			multipliers.push_back(divider);
			curNumber /= divider;
		}
		else divider++;
	}
	if (curNumber != 1) {
		multipliers.push_back(curNumber);
	}

	std::string result;
	result = std::to_string(thisNumber) + " = " + std::to_string(multipliers[0]);
	for (size_t i = 1; i < multipliers.size(); i++) {
		result = result + " * " + std::to_string(multipliers[i]);
	};
	mutex mutex;
	unique_lock<std::mutex> lock(mutex);
	cout << result << "\n";
	lock.unlock();
}

int main() {
	ThreadPool pool(4);
	fstream in("D:\\in.txt");
	int a;
	while (in >> a) {
		function<void()> work = bind(factorization,a);
		pool.addTask(work);
	}
	system("pause");
	return 0;
}