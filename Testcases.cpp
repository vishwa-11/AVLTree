//
// Created by Vishweshwar Ramanakumar on 6/18/21.
//
#define CONFIG_CATCH_MAIN
#include "../Testcases/catch.h"
#include "main.cpp"

TEST_CASE("insert") {
    AVL* tree = new AVL();
    REQUIRE(tree->insert("Vishwa", "11111111") == "successful");
}
