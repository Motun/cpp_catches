/* 
 * https://quuxplusone.github.io/blog/2019/01/03/const-is-a-contract/ 
 */

#include <iostream>

/* Define a global int for experimental purposes. */
int global = 2;

/* A function taking a reference to a const int. */
/* That is, we promise to not modify the variable arg. */
void taking_const_ref(const int& arg)
{
    printf("%d ", arg);
    //arg = 3 // Compiler error because of the promise
    global = 3;
    printf("%d\n", arg);
}

int main()
{
    /* cint -defined as const- is truly const, no matter what. */
    const int cint = 47;
    std::cout << "cint: " << cint << std::endl;
    //cint++; // Compiler error

    /* A reference for const_cast: https://en.cppreference.com/w/cpp/language/const_cast */
    /* In fact, it's not allowed to const_cast a variable if the variable is actually const. 
    Refer to this stackoverflow answer: https://stackoverflow.com/a/19554871/3160184 */
    const_cast<int&>(cint) = 7; // Undefined behaviour
    std::cout << "cint: " << cint << std::endl;

    //int *p_cint = &cint; // Compiler error
    //const int *p_cint = &cint; // Okay
    //int const *p_cint = &cint; // Same as above

    /* i isn't const, but we can (kind of) make it so through a reference or a pointer */
    int i = 17; // Mutable
    std::cout << "i initial: " << i << std::endl;
    const int& ref_i = i; // Reference to a const int (target isn't actually const)
    const int* ptr_i = &i; // Pointer to a const int (target isn't actually const)

    //ref_i = 19; // Compiler error
    //*ptr_i = 19; // Compiler error;

    /* Since the real target (i) isn't const, we can strip away these consts. */
    const_cast<int&>(ref_i) = 19;
    std::cout << "i after const_cast<int&>: " << i << std::endl;
    *const_cast<int*>(ptr_i) = 11;
    std::cout << "i after const_cast<int*>: " << i << std::endl;

    /* Function arguments */
    std::cout << "global before func call: " << global << std::endl;
    taking_const_ref(global);
    std::cout << "global after func call: " << global << std::endl;

    return 0;
}