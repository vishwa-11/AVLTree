#include <iostream>
using namespace std;
#include <vector>
#include <sstream>


//structure for Nodes in the tree
struct Student {
    long GatorID;
    string ID_string;
    string name;
    int height;
    int left_height;
    int right_height;
    int balanceFactor;
    Student* left;
    Student* right;

    Student() {
        name = "unused";
        GatorID = 00000000;
        ID_string = to_string(GatorID);
        height = 0;
        left_height = 0;
        right_height = 0;
        balanceFactor = 0;
        left = nullptr;
        right = nullptr;
    }
    Student(string name, string GatorID) {
        this->name = name;
        this->GatorID = stoi(GatorID);
        ID_string = GatorID;
        height = 0;
        left_height = 0;
        right_height = 0;
        balanceFactor = 0;
        left = nullptr;
        right = nullptr;
    };

};

//structure for AVL tree
//Note: despite only needing the print statement from each method, I added
//a return type with the same data type as my print statement. This helped
//in the extra credit portion of the assignment while using the Catch Framework
//Also note that the height is 1 based, e.g. a tree of just a root has a height of one
struct AVL {
    Student* root;

    //default constructor
    AVL() {
        root = nullptr;
    }

    //inserts a node into the tree
    string insert(string name, string GatorID) {

        //We check the validity of the inputted name
        for (int i = 0; i < name.length(); i++) {
            if (!isalpha(name[i]) && name[i] != ' ') {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }
        }

        //We check the validity of the inputted ID
        if (GatorID.length() != 8) {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        for (int i = 0; i < GatorID.length(); i++) {
            if (!isdigit(GatorID[i])) {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }
        }

        Student* student = new Student (name, GatorID);

        //if root is nullptr, this is the first node in the tree
        if (root == nullptr) {
            root = student;
            cout << "successful" << endl;
            return "successful";
        }

        //checks if Gator ID is unique
        if (!isUnique(GatorID)) {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        Student* node = root;
        bool inserted = false;


        //inserts the new node in the right spot
        while (!inserted) {

            if (student->GatorID > node->GatorID) {
                if (node->right != nullptr) {
                    if (node->left == nullptr) {
                    }
                    node = node->right;
                }
                else {
                    node->right = student;
                    if (node->left == nullptr) {
                    }
                    inserted = true;
                }

            }

            else if (student->GatorID < node->GatorID) {
                if (node->left != nullptr) {
                    if (node->right == nullptr) {
                    }
                    node = node->left;
                }
                else {
                    node->left = student;
                    if (node->right == nullptr) {
                    }
                    inserted = true;

                }
            }
        }

        this->setHeights();

        //Now we balance the tree if needed
        if (!CheckIfBalanced()) {
            Balance();
        }
        cout << "successful" << endl;
        return "successful";
    }

    //deletes a node from the tree
    string remove(string GatorID) {
        if (root == nullptr) {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        //we need to see if the input is a valid ID
        if (GatorID.size() != 8) {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        for (int i = 0; i < GatorID.size(); i++) {
            if (!isdigit(GatorID[i])) {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }
        }

        long ID = stoi(GatorID);
        Student* student = root;
        Student* parent = new Student();
        Student* right_child = new Student();
        Student* left_child = new Student();


        //traverse through the tree to find the node to delete
        //if applicable, save the node's parents and/or children
        while (student->GatorID != ID) {
            //in case the node to be deleted is not the root, we need to get the parent node of the node to be
            //deleted. Once we get the parent, we can get the student as well.
            if ((student->right != nullptr && student->right->GatorID == ID) ||
                (student->left != nullptr && student->left->GatorID == ID)) {
                parent = student;
                if (parent->right->GatorID == ID) {
                    student = parent->right;
                }
                else if (parent->left->GatorID == ID) {
                    student = parent->left;
                }
                if (student->right != nullptr) {
                    right_child = student->right;
                }
                if (student->left != nullptr) {
                    left_child = student->left;
                }
                break;
            }

            //if there's no match, then we continue traversing
            if (ID > student->GatorID) {
                if (student->right != nullptr) {
                    student = student->right;
                    continue;
                }
                else {
                    cout << "unsuccessful" << endl;
                    return "unsuccessful";
                }
            }

            else if (ID < student->GatorID) {
                if (student->left != nullptr) {
                    student = student->left;
                    continue;
                }
                else {
                    cout << "unsuccessful" << endl;
                    return "unsuccessful";
                }
            }
        }

        //need to initialize left and right children in case the student node is the root
        //because in this case, we never enter the above while loop
        if (student == root) {
            if (student->right != nullptr) {
                right_child = student->right;
            }
            if (student->left != nullptr) {
                left_child = student->left;
            }
        }

        //if the node to remove has two children
        if (right_child->name != "unused" && left_child->name != "unused") {
            if (parent->name != "unused") {
                if (parent->left == student) {
                    parent->left = right_child;
                }
                else if (parent->right == student) {
                    parent->right = right_child;
                }
            }
            Student* node3 = student->right;
            Student* node4 = new Student();
            if (node3 != nullptr) {
                while (node3->left != nullptr) {
                    if (node3->left != nullptr) {
                        if (node3->left->left != nullptr) {
                            node4 = node3;
                        }
                    }
                    node3 = node3->left;
                }
            }

            //node3 becomes the replacement as it is the inorder successor
            node4->left = nullptr;
            if (node3 != right_child) {
                node3->right = right_child;
            }
            node3->left = left_child;

            if (student == root) {
                root = node3;
            }

            delete student;
        }

            //if the node to remove only has a right child
        else if (student->right != nullptr && student->left == nullptr) {
            if (parent->name != "unused") {
                if (parent->left == student) {
                    parent->left = right_child;
                }
                else if (parent->right == student) {
                    parent->right = right_child;
                }
            }
            else if (root == student) {
                root = right_child;
            }
            delete student;
        }

            //if the node to remove only has a left child
        else if (student->right == nullptr && student->left != nullptr) {
            if (parent->name != "unused") {
                if (parent->left == student) {
                    parent->left = left_child;
                }
                else if (parent->right == student) {
                    parent->right = left_child;
                }
            }
            else if (root == student) {
                root = left_child;
            }
            delete student;
        }


        //if the node to remove has no children
        else if (student->right == nullptr && student->left == nullptr) {
            if (root == student) {
                root = nullptr;
            }
            else if (parent->left != nullptr && parent->left == student) {
                cout << "yo";
                parent->left = nullptr;
                delete student;
            }
            else if (parent->right != nullptr && parent->right == student) {
                parent->right = nullptr;
                delete student;
            }
        }

        //I reset the heights
        setHeights();

        //Now I balance the tree
        while (!CheckIfBalanced()) {
            Balance();
        }

        cout << "successful" << endl;
        return "successful";
    }

    //searches for a node by GatorID
    string search(string GatorID) {
        //we need to see if the input is a valid ID
        if (GatorID.size() != 8) {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        for (int i = 0; i < GatorID.size(); i++) {
            if (!isdigit(GatorID[i])) {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }
        }

        //this method is a simple tree traversal
        Student* student = root;

        long ID = stoi(GatorID);
        //we traverse the tree till the value is found or till we reach the end of the tree
        while (student != nullptr) {
            if (student->GatorID == ID) {
                cout << student->name << endl;
                return student->name;
            }
            else {
                if (ID > student->GatorID) {
                    student = student->right;
                }
                else {
                    student = student->left;
                }
            }
        }
        cout << "unsuccessful" << endl;
        return "unsuccessful";
    }

    //searched for all the nodes with a certain name
    //Takes the vector returned from the PreOrder method and prints its values
    //if there are any
    vector<string> search2(string name) {
        vector<string> answer = PreOrder(root, name);

        //we check if the name only has alphabets
        for (int i = 0; i < name.length(); i++) {
            if (!isalpha(name[i]) && name[i] != ' ') {
                cout << "unsuccessful" << endl;
                answer.push_back("unsuccessful");
                return answer;
            }
        }

        if (answer.empty()) {
            cout << "unsuccessful" << endl;
            answer.push_back("unsuccessful");
            return answer;
        }
        else {
            //I need to iterate through the vector and print the values
            for (int i = 0; i < answer.size(); i++) {
                cout << answer[i] << endl;
            }
            return answer;
        }
    }

    //A method that prints out nodes InOrder using
    //a private recursive method
    string printInorder(Student* student) {
        string answer;
        vector<string> list = printInorderpriv(student);

        //we run through the vector, append it's elements to
        //the string and add commas

        for (int i = 0; i < list.size(); i++) {
            answer += list[i];
            if (i != list.size() - 1) {
                answer += ", ";
            }
        }
        cout << answer;
        return answer;
    }

    //A method that prints out nodes PreOrder using
    //a private recursive method
    string printPreorder(Student* student) {
        string answer;
        vector<string> list = printPreorderpriv(student);

        //we run through the vector, append it's elements to
        //the string and add commas

        for (int i = 0; i < list.size(); i++) {
            answer += list[i];
            if (i != list.size() - 1) {
                answer += ", ";
            }
        }
        cout << answer;
        return answer;
    }

    //A method that prints out nodes PostOrder using
    //a private recursive method
    string printPostorder(Student* student) {
        string answer;
        vector<string> list = printPostorderpriv(student);

        //we run through the vector, append it's elements to
        //the string and add commas

        for (int i = 0; i < list.size(); i++) {
            answer += list[i];
            if (i != list.size() - 1) {
                answer += ", ";
            }
        }
        cout << answer;
        return answer;
    }

    //Uses the GetNodes method to get a double vector of all the nodes
    //in the tree. Each vector in the double vector represents a level
    //by counting the size of the double vector, we get the number of layers.
    int printLevelCount() {
        if (root == nullptr) {
            cout << 0 << endl;
            return 0;
        }
        else {
            vector<vector<Student*>> layers = getNodes();
            cout << layers.size() << endl;
            return layers.size();
        }
    }

    //Uses a combination of the remove method and the GetNodes method to
    //remove the Nth node in terms of inorder traversal.
    string removeInorder(long N) {

        //we need to count the number of nodes in the tree using
        //GetNodes and make sure that N isn't out of bounds
        vector<vector<Student*>> layers = getNodes();
        int size = 0;
        for (int i = 0; i < layers.size(); i++) {
            size += layers[i].size();
        }
        if (size <= N) {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        //now that we have a vector of nodes in an inorder format
        //we can get the Nth index node from that vector and
        //call the remove method on that node
        vector<Student*> inOrder = Nth_node(root);
        return remove(to_string(inOrder[N]->GatorID));
    }

    //This method is used to test each method
    void testPrint() {
        vector<vector<Student*>> layers = this->getNodes();
        for (int i = 0; i < layers.size(); i++) {
            for (int j = 0; j < layers[i].size(); j++) {
                cout << layers[i][j]->name << ", " << layers[i][j]->ID_string << ", " << layers[i][j]->height << ", " << layers[i][j]->balanceFactor << " || ";
            }
            cout << endl;
        }
    }

private:
    //recurrent method for printInorder
    vector<string> printInorderpriv(Student* student) {
        vector<string> answer;

        if (student == nullptr) {
            return answer;
        }

        vector<string> left = printInorderpriv(student->left);

        for (int i = 0; i < left.size(); i++) {
            answer.push_back(left[i]);
        }

        answer.push_back(student->name);

        vector<string> right = printInorderpriv(student->right);

        for (int i = 0; i < right.size(); i++) {
            answer.push_back(right[i]);
        }

        return answer;
    }

    //recurrent method for printPreorder
    vector<string> printPreorderpriv(Student* student) {
        vector<string> answer;

        if (student == nullptr) {
            return answer;
        }

        answer.push_back(student->name);

        vector<string> left = printPreorderpriv(student->left);

        for (int i = 0; i < left.size(); i++) {
            answer.push_back(left[i]);
        }

        vector<string> right = printPreorderpriv(student->right);

        for (int i = 0; i < right.size(); i++) {
            answer.push_back(right[i]);
        }

        return answer;
    }

    //recurrent method for printPostorder
    vector<string> printPostorderpriv(Student* student) {
        vector<string> answer;

        if (student == nullptr) {
            return answer;
        }

        vector<string> left = printPostorderpriv(student->left);

        for (int i = 0; i < left.size(); i++) {
            answer.push_back(left[i]);
        }

        vector<string> right = printPostorderpriv(student->right);

        for (int i = 0; i < right.size(); i++) {
            answer.push_back(right[i]);
        }

        answer.push_back(student->name);

        return answer;
    }

    //A recurrent method that finds the Nth node in the inorder traversal
    //I use a similar method to the PreOrder method. I get a vector of
    //nodes in the inorder traversal order. I can use this in the
    //removeInorder method and iterate through the returned vector N times
    vector<Student*> Nth_node(Student* student) {
        vector<Student*> answer;

        if (student == nullptr) {
            return answer;
        }

        vector<Student*> left = Nth_node(student->left);


        for (int i = 0; i < left.size(); i++) {
                answer.push_back(left[i]);
        }



        answer.push_back(student);

        vector<Student*> right = Nth_node(student->right);


        for (int i = 0; i < right.size(); i++) {
                answer.push_back(right[i]);
        }



        return answer;
    }

    //this method does a preorder traversal and returns a vector of
    //all the values corresponding to the passed in name.
    //It is a recursive method
    vector<string> PreOrder(Student* student, string name) {
        vector<string> answer;

        if (student == nullptr) {
            return answer;
        }

        if (student->name == name) {
            answer.push_back(student->ID_string);
        }

        vector<string> left = PreOrder(student->left, name);

        for (int i = 0; i < left.size(); i++) {
            answer.push_back(left[i]);
        }

        vector<string> right = PreOrder(student->right, name);

        for (int i = 0; i < right.size(); i++) {
            answer.push_back(right[i]);
        }

        return answer;
    }

    //this method balances the tree
    void Balance() {
        //if the tree isn't balanced, we need to find the node that isn't balanced first.
        vector<vector<Student*>> layers = getNodes();
        Student* unbalanced = new Student();
        Student* unbalanced_parent = new Student();
        for (int i = 0; i < layers.size(); i++) {
            for (int j = 0; j < layers[i].size(); j++) {

                if (layers[i][j]->right != nullptr && layers[i][j]->right->balanceFactor > 1) {
                    unbalanced_parent = layers[i][j];
                }
                if (layers[i][j]->left != nullptr && layers[i][j]->left->balanceFactor > 1) {
                    unbalanced_parent = layers[i][j];
                }
                if (layers[i][j]->balanceFactor > 1) {
                    unbalanced = layers[i][j];
                    break;
                }
            }
        }

        //Now we take the node, check what rotation is needed
        //then perform that rotation
        int imbalanceType = TypeofImbalance(unbalanced);
        if (imbalanceType == -1) {
            Student* node1 = unbalanced->right;
            if (node1->left != nullptr) {
                unbalanced->right = node1->left;
            }
            else {
                unbalanced->right = nullptr;
            }
            node1->left = unbalanced;
            //In each if-else branch, the unbalanced parent is used in case the node about which
            //we balance the tree is not the root node. The unbalanced parent makes sure that the rotations don't sever
            //the connection of the nodes from the tree
            if (unbalanced_parent->name != "unused") {
                if (unbalanced_parent->right == unbalanced) {
                    unbalanced_parent->right = node1;
                }
                else if (unbalanced_parent->left == unbalanced) {
                    unbalanced_parent->left = node1;
                }
            }
            if (unbalanced == root) {
                root = node1;
            }
        }

        else if (imbalanceType == -2) {
            //we need to do a right-left twist
            Student* node1 = unbalanced->right;
            Student* node2 = unbalanced->right->left;
            unbalanced->right = node2;
            if (node2->right != nullptr) {
                node1->left = node2->right;
            }
            else {
                node1->left = nullptr;
            }
            node2->right = node1;
            if (node2->left != nullptr) {
                unbalanced->right = node2->left;
            }
            else {
                unbalanced->right = nullptr;
            }
            node2->left = unbalanced;
            if (unbalanced_parent->name != "unused") {
                if (unbalanced_parent->right == unbalanced) {
                    unbalanced_parent->right = node2;
                }
                else if (unbalanced_parent->left == unbalanced) {
                    unbalanced_parent->left = node2;
                }
            }
            if (unbalanced == root) {
                root = node2;
            }
        }

        else if (imbalanceType == -3) {
            //we need to do a right twist
            Student* node1 = unbalanced->left;
            if (node1->right != nullptr) {
                unbalanced->left = node1->right;
            }
            else {
                unbalanced->left = nullptr;
            }
            node1->right = unbalanced;
            if (unbalanced_parent->name != "unused") {
                if (unbalanced_parent->right == unbalanced) {
                    unbalanced_parent->right = node1;
                }
                else if (unbalanced_parent->left == unbalanced) {
                    unbalanced_parent->left = node1;
                }
            }
            if (unbalanced == root) {
                root = node1;
            }
        }

        else if (imbalanceType == -4) {
            //we need to do a left-right twist
            Student* node1 = unbalanced->left;
            Student* node2 = unbalanced->left->right;
            unbalanced->left = node2;
            if (node2->left != nullptr) {
                node1->right = node2->left;
            }
            else {
                node1->right = nullptr;
            }
            node2->left = node1;
            if (node2->right != nullptr) {
                unbalanced->left = node2->right;
            }
            else {
                unbalanced->left = nullptr;
            }
            node2->right = unbalanced;
            if (unbalanced_parent->name != "unused") {
                if (unbalanced_parent->right == unbalanced) {
                    unbalanced_parent->right = node2;
                }
                else if (unbalanced_parent->left == unbalanced) {
                    unbalanced_parent->left = node2;
                }
            }
            if (unbalanced == root) {
                root = node2;
            }
        }
        setHeights();
    }

    //checks the type of imbalance on a node
    //Key:
    //-1 is right-right
    //-2 is right-left
    //-3 is left-left
    //-4 is left-right
    static int TypeofImbalance(Student* student) {
        if (student->right_height > student->left_height) {
            if (student->right != nullptr && student->right->right_height > student->right->left_height) {
                return -1;
            }
            else {
                return -2;
            }
        }
        else {
            if (student->left != nullptr && student->left->right_height < student->left->left_height) {
                return -3;
            }
            else {
                return -4;
            }
        }
    }

    //checks if the tree is balances by iteratively
    //checking if each node is balanced or not
    bool CheckIfBalanced() {
        vector<vector<Student*>> layers = getNodes();

        for (int i = 0; i < layers.size(); i++) {
            for (int j = 0; j < layers[i].size(); j++) {
                if (layers[i][j]->balanceFactor > 1) {
                    return false;
                }
            }
        }
        return true;
    }

    //this method gets a double vector of all nodes in the tree
    //each vector in the double vector represents a layer
    vector<vector<Student*>> getNodes() {
        vector<vector<Student*>> layers;
        vector<Student*> layer1;
        vector<Student*> layer2;

        if (root == nullptr) {
            return layers;
        }

        layer1.push_back(root);
        layers.push_back(layer1);

        while (!layer1.empty()) {
            for (int i = 0; i < layer1.size(); i++) {
                if (layer1[i]->left != nullptr) {
                    layer2.push_back(layer1[i]->left);
                }
                if (layer1[i]->right != nullptr) {
                    layer2.push_back(layer1[i]->right);
                }
            }
            if (!layer2.empty()) {
                layers.push_back(layer2);
            }

            layer1 = layer2;

            layer2.clear();
        }
        return layers;
    }

    //checks if an ID number is unique
    bool isUnique(string ID) {
        if (root == nullptr) {
            return true;
        }


        //This creates a vector of a vector of all nodes in the AVL tree
        vector<vector<Student*>> layers = getNodes();

        //Now I use a double for loop to iterate through the double vector
        //and try to see if there's a duplicate.

        for (int i = 0; i < layers.size(); i++) {
            for (int j = 0; j < layers[i].size(); j++) {
                if (layers[i][j]->ID_string == ID) {
                    return false;
                }
            }
        }
        return true;
    }

    //sets the height of both the left and right subtrees of a node
    //helps in finding the balancing factor of each node
    static void setLeftRightHeights(Student* student) {
        vector<vector<Student*>> left_levels;
        vector<vector<Student*>> right_levels;

        vector<Student*> left_level1;
        vector<Student*> right_level1;

        vector<Student*> level2;

        if (student->left != nullptr) {

            left_level1.push_back(student->left);

            left_levels.push_back(left_level1);
            while (!left_level1.empty()) {
                for (int i = 0; i < left_level1.size(); i++) {
                    if (left_level1[i]->right != nullptr) {
                        level2.push_back(left_level1[i]->right);
                    }
                    if (left_level1[i]->left != nullptr) {
                        level2.push_back(left_level1[i]->left);
                    }

                }
                if (!level2.empty()) {
                    left_levels.push_back(level2);
                }
                left_level1 = level2;
                level2.clear();
            }
        }

        level2.clear();

        if (student->right != nullptr) {

            right_level1.push_back(student->right);

            right_levels.push_back(right_level1);
            while (!right_level1.empty()) {
                for (int i = 0; i < right_level1.size(); i++) {
                    if (right_level1[i]->right != nullptr) {
                        level2.push_back(right_level1[i]->right);
                    }
                    if (right_level1[i]->left != nullptr) {
                        level2.push_back(right_level1[i]->left);
                    }

                }
                if (!level2.empty()) {

                    right_levels.push_back(level2);
                }
                right_level1 = level2;
                level2.clear();
            }
        }


        student->right_height = right_levels.size() + 1;
        student->left_height = left_levels.size() + 1;
    }

    //This method gets a double vector of all nodes and calculates
    //the left and right heights of each. Then it calculates
    //the height of each node by checking whether the right or left subtree heights are greater
    //the larger of the heights dictates the node's height
    //The method also sets each node's balance factors
    void setHeights() {
        vector<vector<Student*>> layers = getNodes();
        for (int i = 0; i < layers.size(); i++) {
            for (int j = 0; j < layers[i].size(); j++) {
                setLeftRightHeights(layers[i][j]);
                if (layers[i][j]->right_height >= layers[i][j]->left_height) {
                    layers[i][j]->height = layers[i][j]->right_height;
                }
                else {
                    layers[i][j]->height = layers[i][j]->left_height;
                }

                layers[i][j]->balanceFactor = abs(layers[i][j]->left_height - layers[i][j]->right_height);
            }
        }
    }
};

int main() {
    AVL* tree = new AVL();

    //we pull the number of commands in the program

    int noOfCommands;
    cin >> noOfCommands;

    string command = "";
    string input = "";

    getline(cin, command);

    stringstream X(command);

    vector<string> inputs;

    while (getline(X, input, ' ')) {
        inputs.push_back(input);
    }

    for (int i = 0; i < noOfCommands; i++) {
        getline(cin, command);

        stringstream X(command);

        vector<string> inputs;

        while (getline(X, input, ' ')) {
            inputs.push_back(input);
        }

        if (inputs[0] == "insert") {
            //the full name can have a first, middle and/or last name.
            //if the second element of the vector only has one quote, we need to
            //find which subsequent element has the last quote in order to
            //set the whole name
            int quote_count = 0;
            string full_name = "";
            int index = 0;

            while (quote_count < 2) {
                index++;
                for (int i = 0; i < inputs[index].length(); i++) {
                    if (inputs[index][i] == '"') {
                        quote_count++;
                    }
                }
                full_name += inputs[index];
                full_name += " ";
            }

            //we need to remove the quotes
            full_name = full_name.substr(1, full_name.length() - 3);

            long ID = stoi(inputs[index + 1]);

            tree->insert(full_name, to_string(ID));
        }

        else if (inputs[0] == "remove") {
            tree->remove(inputs[1]);
        }

        else if (inputs[0] == "search") {
            if (isdigit(inputs[1][0])) {
                tree->search(inputs[1]);
            }

            else {
                int quote_count = 0;
                string full_name = "";
                int index = 0;

                while (quote_count < 2) {
                    index++;
                    for (int i = 0; i < inputs[index].length(); i++) {
                        if (inputs[index][i] == '"') {
                            quote_count++;
                        }
                    }
                    full_name += inputs[index];
                    full_name += " ";
                }

                //we need to remove the quotes
                full_name = full_name.substr(1, full_name.length() - 3);

                tree->search2(full_name);
            }
        }

        else if (inputs[0] == "printInorder") {
            tree->printInorder(tree->root);
            cout << endl;
        }

        else if (inputs[0] == "printPreorder") {
            tree->printPreorder(tree->root);
            cout << endl;
        }

        else if (inputs[0] == "printPostorder") {
            tree->printPostorder(tree->root);
            cout << endl;
        }

        else if (inputs[0] == "printLevelCount") {
            tree->printLevelCount();
        }

        else if (inputs[0] == "removeInorder") {
            tree->removeInorder(stoi(inputs[1]));
        }
    }

    return 0;
}