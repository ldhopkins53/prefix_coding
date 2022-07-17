#include <iostream>
#include <string>
#include <stdexcept>

class Node
{
	public:
		explicit Node(char new_character) : character{new_character}, left{nullptr}, right{nullptr} {}

		bool isCharNode() { return character != '*'; }
		char getChar() { return character; }

		Node* getLeft() { return left; }
		Node* getRight() { return right; }

		void setLeft(Node* new_left) { left = new_left; }
		void setRight(Node* new_right) { right = new_right; }

	private:
		char character;
		Node *left;
		Node *right;
};

class BinaryTree
{
	public:
		explicit BinaryTree(const std::string &preorder_traversal_string) :
			root{nullptr},
			preorder_traversal(preorder_traversal_string) {}

		Node *generateTreeRecursive(std::size_t &index); // e.g. "*a**!*dc*rb"
		void generateTree();

		void inOrder(Node *node);

		Node *getRoot() { return root; }

		std::string decodePrefixCode(const std::string& prefix_code) const; // "0111110010110101001111100100"

		void generatePreorderNotation(Node *node, std::string &output_string);
		std::string generatePreorderNotation();

	private:
		Node *root;
		const std::string preorder_traversal;
};

void BinaryTree::inOrder(Node *node)
{
	if (node != nullptr)
	{
		inOrder(node->getLeft());
		if (node->getChar() != '*')
		{
			std::cout << node->getChar() << std::endl;
		}
		inOrder(node->getRight());
	}
}

Node* BinaryTree::generateTreeRecursive(std::size_t &index)
{
	std::size_t current_index = index;

	if (current_index == preorder_traversal.length())
	{
		return nullptr;
	}

	Node *tmp = new Node(preorder_traversal[index]);
	++index;

	if (preorder_traversal[index-1] == '*')
	{
		tmp->setLeft( generateTreeRecursive(index) );
		tmp->setRight( generateTreeRecursive(index) );
	}

	return tmp;
}

void BinaryTree::generateTree()
{
	std::size_t index = 0;
	root = generateTreeRecursive(index);
}


std::string BinaryTree::decodePrefixCode(const std::string& prefix_code) const
{
	if (root == nullptr)
	{
		throw std::logic_error("generateTree has to be called before decodePrefixCode");
	}

	std::string output_message{""};
	Node * current_node = root;
	for (const char &code_value : prefix_code)
	{
		// Continue traversal
		switch( code_value )
		{
			case '0':
				current_node = current_node->getLeft();
				break;
			case '1':
				current_node = current_node->getRight();
				break;
			default:
				std::cerr << "We found an invalid character in the prefix code: " << code_value << std::endl;
				throw std::invalid_argument("Invalid input in the prefix code");
		}

		// If we have hit an endpoint, add the value and move to the top again
		if (current_node->isCharNode())
		{
			output_message += current_node->getChar();
			current_node = root;
		}
	}

	return output_message;
}

void BinaryTree::generatePreorderNotation(Node *node, std::string &output_string)
{
	if (node != nullptr)
	{
		output_string += node->getChar();
		generatePreorderNotation(node->getLeft(), output_string);
		generatePreorderNotation(node->getRight(), output_string);
	}
}

std::string BinaryTree::generatePreorderNotation()
{
	std::string preorder_notation;
	generatePreorderNotation(root, preorder_notation);
	return preorder_notation;
}

int main()
{
	const std::string preorder_traversal_string = "*a**!*dc*rb";
	BinaryTree tree(preorder_traversal_string);

	std::cout << "Generating the tree from preorder traversal: " << preorder_traversal_string << std::endl;
	tree.generateTree();

	tree.inOrder(tree.getRoot());

	const std::string prefix_code = "0111110010110101001111100100";
	std::cout << "Decoding the prefix code message" << std::endl;
	const std::string decoded_message = tree.decodePrefixCode(prefix_code);

	std::cout << "The prefix code was: " << prefix_code << std::endl;
	std::cout << "The decoded message is: " << decoded_message << std::endl;

	std::string reconstructed_preorder_notation = tree.generatePreorderNotation();
	std::cout << "Reconstructed preorder notation: " << reconstructed_preorder_notation << std::endl;

	return 0;
}
