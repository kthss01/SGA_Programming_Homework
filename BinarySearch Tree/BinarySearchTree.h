#pragma once

#include <iostream>
using namespace std;

template <typename T>
class Tree;

template <typename T>
class TreeNode {
	friend class Tree<T>;
private:
	T data;
	TreeNode* left;
	TreeNode* right;
public:
	TreeNode(T data = 0, TreeNode* left = NULL, TreeNode* right = NULL) {
		this->data = data;
		this->left = left;
		this->right = right;
	}

	friend ostream& operator<<(ostream& os, const TreeNode<T>& node) {
		os << "[data]" << node.data << endl;
		if (node.left != NULL) {
			os << "[left]" << node.left->data << endl;
		}
		if (node.right != NULL) {
			os << "[right]" << node.right->data << endl;
		}
		return os;
	}
};

template <typename T>
class Tree {
private:
	TreeNode<T>* root;
public:
	Tree(T data = 0) {
		root = new TreeNode<T>(data);
	}

	void InsertNode(TreeNode<T>* node) {
		// 중복이 없을 때
		if (Search(root, node->data) == NULL) {
			TreeNode<T>* parent = NULL;
			TreeNode<T>* current = root;
			// 작으면 왼쪽, 크면 오른쪽으로 이동,
			// 새 노드의 부모가 정해짐
			while (current != NULL) {
				parent = current;
				if (node->data < parent->data) {
					current = current->left;
				}
				else {
					current = current->right;
				}
			}
			if (node->data < parent->data) {
				parent->left = node;
			}
			else {
				parent->right = node;
			}
			cout << "Inserted " << node->data << endl;
		}
	}

	TreeNode<T>* GetRoot() {
		return root;
	}

	void PreOrder(TreeNode<T>* current) {
		if (current != NULL) {
			Visit(current);
			PreOrder(current->left);
			PreOrder(current->right);
		}
	}

	void InOrder(TreeNode<T>* current) {
		if (current != NULL) {
			InOrder(current->left);
			Visit(current);
			InOrder(current->right);
		}
	}

	void PostOrder(TreeNode<T> * current) {
		if (current != NULL) {
			PostOrder(current->left);
			PostOrder(current->right);
			Visit(current);
		}
	}

	TreeNode<T>* Search(TreeNode<T>* current, T data) {
		if (current == NULL) return NULL;
		if (data == current->data) {
			return current;
		}
		else if (data < current->data) {
			Search(current->left, data);
		}
		else {
			Search(current->right, data);
		}
	}

	void Visit(TreeNode<T>* current) {
		cout << current->data << " ";
	}
};