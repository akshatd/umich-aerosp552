#include "Tree.h"

Tree::Tree() { head_ = nullptr; }

Tree::Tree(const Tree &t) {
	head_           = nullptr;
	int *temp_array = t.toarray();
	int  size       = t.size();
	int  middle     = size / 2;
	int  offset     = 0;
	int  left, right;
	while (offset <= middle) {
		left = middle - offset;
		if (left >= 0) insert(temp_array[left]);
		right = middle + offset;
		if (right < size) insert(temp_array[right]);
		++offset;
	}
	delete temp_array;
}

Tree::~Tree() {
	delRec(head_);
	delete (head_);
	head_ = nullptr;
}

void Tree::insert(int i) {
	if (!head_) {
		head_ = new treeNode({i, nullptr, nullptr});
	} else {
		insertRec(head_, i);
	}
}

void Tree::insertRec(treeNode *node, int i) {
	if (i == node->root) return;

	if (i < node->root) {
		if (node->left) {
			insertRec(node->left, i);
		} else {
			node->left = new treeNode({i, nullptr, nullptr});
		}
	}

	if (i > node->root) {
		if (node->right) {
			insertRec(node->right, i);
		} else {
			node->right = new treeNode({i, nullptr, nullptr});
		}
	}
}

void Tree::erase(int i) {
	treeNode **node = &head_;
	// find the correct node
	while (*node) {
		if (i == (*node)->root) {
			// get the left and right data before deleting
			treeNode *left_node = (*node)->left;
			int       left_size;
			int      *left_arr   = toArrayRec(left_node, left_size);
			treeNode *right_node = (*node)->right;
			int       right_size;
			int      *right_arr = toArrayRec(right_node, right_size);
			// delete node
			delete *node;
			*node = nullptr;
			// insert other elements back
			for (int j = 0; j < left_size; ++j) {
				insert(left_arr[j]);
			}
			for (int j = 0; j < right_size; ++j) {
				insert(right_arr[j]);
			}
			// cleanup
			delete left_arr;
			delRec(left_node);
			delete (left_node);
			left_node = nullptr;
			delete right_arr;
			delRec(right_node);
			delete (right_node);
			right_node = nullptr;
			return;
		} else if (i < (*node)->root) {
			node = &((*node)->left);
		} else if (i > (*node)->root) {
			node = &((*node)->right);
		}
	}
}

void Tree::delRec(treeNode *node) {
	if (!node) return;
	if (node->left) {
		delRec(node->left);
		delete node->left;
		node->left = nullptr;
	}
	if (node->right) {
		delRec(node->right);
		delete node->right;
		node->right = nullptr;
	}
	// delete node;
	// node = nullptr;
}

bool Tree::member(int i) const {
	treeNode *node = head_;
	// find the correct node
	while (node) {
		if (i == node->root) {
			return true;
		} else if (i < node->root) {
			node = node->left;
		} else if (i > node->root) {
			node = node->right;
		}
	}
	return false;
}

int Tree::size() const { return sizeRec(head_); }

int Tree::sizeRec(treeNode *node) const {
	if (!node) return 0;
	return sizeRec(node->left) + sizeRec(node->right) + 1;
}

int *Tree::toarray() const {
	int size_notused;
	return toArrayRec(head_, size_notused);
}

int *Tree::toArrayRec(treeNode *node, int &size) const {
	if (!node) {
		size = 0;
		return nullptr;
	}
	int  left_size      = 0;
	int *left_array     = toArrayRec(node->left, left_size);
	int  right_size     = 0;
	int *right_array    = toArrayRec(node->right, right_size);
	size                = left_size + right_size + 1;
	int *combined_array = new int(size);
	// cant use std::memcpy or std::copy because those headers arent allowed :[
	for (int i = 0; i < left_size; ++i) {
		combined_array[i] = left_array[i];
	}
	combined_array[left_size] = node->root;
	for (int i = 0; i < right_size; ++i) {
		combined_array[i + left_size + 1] = right_array[i];
	}
	delete left_array;
	delete right_array;
	return combined_array;
}

std::ostream &operator<<(std::ostream &os, const Tree &t) {
	t.printRec(t.head_, os);
	return os;
}
void Tree::printRec(treeNode *node, std::ostream &os) const {
	if (!node) return;
	printRec(node->left, os);
	os << node->root << " ";
	printRec(node->right, os);
}

Tree Tree::treeUnion(const Tree a, const Tree b) const {
	Tree return_tree(a);
	int *tree_b_arr = b.toarray();
	for (int i = 0; i < b.size(); ++i) {
		return_tree.insert(tree_b_arr[i]);
	}
	delete tree_b_arr;
	return return_tree;
}

Tree Tree::intersection(const Tree a, const Tree b) const {
	Tree return_tree;
	int *a_arr = a.toarray();
	for (int i = 0; i < a.size(); ++i) {
		if (b.member(a_arr[i])) return_tree.insert(a_arr[i]);
	}
	delete a_arr;
	return return_tree;
}
