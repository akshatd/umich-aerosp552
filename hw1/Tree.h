#ifndef TREE_H
#define TREE_H

#include <iostream>

class Tree {
	private:
		struct treeNode {
				int              root;
				struct treeNode* left;
				struct treeNode* right;
		};

		treeNode* head_;

		void insertRec(treeNode* node, int i);
		void delRec(treeNode* node);
		int  sizeRec(treeNode* node) const;
		int* toArrayRec(treeNode* node, int& size) const;
		void printRec(treeNode* node, std::ostream& os) const;

	public:
		Tree();
		Tree(const Tree& t);
		~Tree();

		void                 insert(int i);
		void                 erase(int i);
		bool                 member(int i) const;
		int                  size() const;
		int*                 toarray() const;
		friend std::ostream& operator<<(std::ostream& os, const Tree& t);
		// 	t.printRec(t.head_, os);
		// 	return os;
		// }
		Tree treeUnion(const Tree a, const Tree b) const;
		Tree intersection(const Tree a, const Tree b) const;
};

#endif