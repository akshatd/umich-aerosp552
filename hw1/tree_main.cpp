#include <iostream>

#include "Tree.h"

int main(void) {
	Tree t1;
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.insert(1);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.insert(2);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.insert(3);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	std::cout << "is 3 a member? " << t1.member(3) << '\n';
	std::cout << "is 4 a member? " << t1.member(4) << '\n';
	t1.erase(1);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.erase(1);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.insert(3);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.insert(4);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.insert(5);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.insert(1);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.insert(6);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	t1.erase(2);
	std::cout << "Tree 1 is " << t1 << " with size " << t1.size() << '\n';
	std::cout << "is 6 a member? " << t1.member(6) << '\n';

	int *arr = t1.toarray();
	std::cout << "t1 from array: ";
	for (int i = 0; i < t1.size(); ++i) {
		std::cout << arr[i] << ",";
	}
	delete arr;

	Tree t2(t1);
	std::cout << "\nTree 2 is " << t2 << '\n';

	Tree t3;
	t3.insert(5);
	t3.insert(10);
	t3.insert(3);

	std::cout << "intersection: " << t3.intersection(t3, t2) << '\n';
	std::cout << "intersection: " << t3.intersection(t2, t3) << '\n';
	std::cout << "union: " << t3.treeUnion(t3, t2) << '\n';
	std::cout << "union: " << t3.treeUnion(t2, t3) << '\n';
}